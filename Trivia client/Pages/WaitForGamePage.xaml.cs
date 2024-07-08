using System;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using Trivia_client.CommunicationHandling;

namespace Trivia_client.Pages
{
    public partial class WaitForGamePage : Window
    {
        static class Constants
        {
            public const int PLAYERS_TEXT_MINIMUM_WIDTH = 130;
            public const int PLAYERS_TEXT_HEIGHT = 26;
            public const int PLAYERS_TEXT_FONT_SIZE = 18;
            public const int SEPERATOR_HEIGHT = 1;
            public const int PLAYER_TEXT_FONT_SIZE = 20;
            public const int ONE_SECOND = 1000;
            public const int FAILURE = 0;
        }

        bool finished;
        string username;
        bool isAdmin;
        SocketHandler socketHandler;
        bool logout = false;
        Thread refreshThread;
        RoomData mRoomData;

        /// <summary>
        /// Constructor to initialize the WaitForGamePage with username, isAdmin status, and RoomData.
        /// </summary>
        /// <param name="username">The username of the user</param>
        /// <param name="isAdmin">True if the user is admin and false if not</param>
        /// <param name="roomData">The data of the room</param>
        public WaitForGamePage(string username, bool isAdmin, RoomData roomData)
        {
            socketHandler = SocketHandler.getInstance();
            this.username = username;
            this.isAdmin = isAdmin;
            mRoomData = roomData;
            InitializeComponent();
            Label.Content = "Welcome to room \"" + mRoomData.name + "\"";
            Closing += new System.ComponentModel.CancelEventHandler(ClosingHandler);
            finished = false;
            refreshThread = new Thread(() => Refresh());
            refreshThread.Start();

            if (isAdmin)
            {
                LeaveGame.Visibility = Visibility.Hidden;
                CloseRoom.Visibility = Visibility.Visible;
                StartGame.Visibility = Visibility.Visible;
            }
        }

        /// <summary>
        /// Periodically refreshes the room state and updates the UI with player information.
        /// </summary>
        private void Refresh()
        {
            while (!finished)
            {
                this.Dispatcher.Invoke(() =>
                {
                    Players.Children.Clear(); // Clear the existing player list UI

                    TextBlock n = new TextBlock();
                    n.FontWeight = FontWeights.Bold;
                    n.Background = Brushes.Green;
                    n.HorizontalAlignment = HorizontalAlignment.Center;
                    n.VerticalAlignment = VerticalAlignment.Center;
                    n.TextAlignment = TextAlignment.Justify;
                    n.MinWidth = Constants.PLAYERS_TEXT_MINIMUM_WIDTH;
                    n.Height = Constants.PLAYERS_TEXT_HEIGHT;
                    n.FontSize = Constants.PLAYERS_TEXT_FONT_SIZE;
                    n.TextAlignment = TextAlignment.Center;

                    /// Request room state from server
                    socketHandler.Send("", (byte)CommunicationHandling.CODES.GET_ROOM_STATE_REQUEST);
                    ResponseInfo info = socketHandler.Receive();
                    try
                    {
                        /// Deserialize response
                        GetRoomStateResponse response = JsonPacketDeserializer.DeserializeGetRoomStateResponse(info.Data);

                        /// Handle room state
                        if (response.state == (uint)RoomState.CLOSED)
                        {
                            logout = true;
                            finished = true;
                            MenuPage menuPage = new MenuPage(username);
                            menuPage.Show();
                            this.Close();
                            return;
                        }
                        else if (response.state == (uint)RoomState.STARTED)
                        {
                            logout = true;
                            finished = true;
                            new GamePage(username, mRoomData).Show();
                            this.Close();
                            return;
                        }

                        /// Update UI with player information
                        n.Text = "Players " + response.players.Count + "/" + mRoomData.maxPlayers + ":";
                        Players.Children.Add(n);

                        foreach (string player in response.players)
                        {
                            TextBlock seperator = new TextBlock();
                            seperator.Height = Constants.SEPERATOR_HEIGHT;
                            Players.Children.Add(seperator);

                            TextBlock name = new TextBlock();
                            name.Text = player;
                            name.Background = Brushes.Blue;
                            name.HorizontalAlignment = HorizontalAlignment.Center;
                            name.VerticalAlignment = VerticalAlignment.Center;
                            name.TextAlignment = TextAlignment.Justify;
                            name.MinWidth = Constants.PLAYERS_TEXT_MINIMUM_WIDTH;
                            name.Height = Constants.PLAYERS_TEXT_HEIGHT;
                            name.FontSize = Constants.PLAYER_TEXT_FONT_SIZE;
                            name.TextAlignment = TextAlignment.Center;
                            Players.Children.Add(name);
                        }

                        /// Adjust height of Players stack panel based on children
                        Players.Height = (Players.Children.Count * Constants.PLAYERS_TEXT_HEIGHT) + Players.Children.Count - 1;

                        /// Update additional room information
                        QuestionCount.Text = "Questions: " + response.questionCount;
                        TimePerQuestion.Text = "Time per question: " + response.answerTimeout;
                    }
                    catch
                    {
                        /// Handle any exceptions from deserialization or UI updates
                    }
                });

                /// Wait before refreshing again
                Thread.Sleep(Constants.ONE_SECOND);
            }
        }

        /// <summary>
        /// Handles the Leave Game button click event.
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void LeaveGameClick(object sender, RoutedEventArgs e)
        {
            logout = true;
            finished = true;
            socketHandler.Send("", (byte)CommunicationHandling.CODES.LEAVE_ROOM_REQUEST);
            ResponseInfo info = socketHandler.Receive();
            try
            {
                LeaveRoomResponse response = JsonPacketDeserializer.DeserializeLeaveRoomResponse(info.Data);
                if (response.status != Constants.FAILURE)
                {
                    MenuPage menuPage = new MenuPage(username);
                    menuPage.Show();
                    this.Close();
                }
                else
                {
                    MessageBox.Show("Error trying to leave the room. please try again.");
                }
            }
            catch
            {
                ErrorResponse response = JsonPacketDeserializer.DeserializeErrorResponse(info.Data);
                MessageBox.Show(response.Message);
            }
        }

        /// <summary>
        /// Handles the Close Room button click event (only visible to admin).
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void CloseRoomClick(object sender, RoutedEventArgs e)
        {
            logout = true;
            finished = true;
            socketHandler.Send("", (byte)CommunicationHandling.CODES.CLOSE_ROOM_REQUEST);
            ResponseInfo info = socketHandler.Receive();
            try
            {
                CloseRoomResponse response = JsonPacketDeserializer.DeserializeCloseRoomResponse(info.Data);
                if (response.status != Constants.FAILURE)
                {
                    MenuPage menuPage = new MenuPage(username);
                    menuPage.Show();
                    this.Close();
                }
                else
                {
                    MessageBox.Show("Error trying to close the room. please try again.");
                }
            }
            catch
            {
                ErrorResponse response = JsonPacketDeserializer.DeserializeErrorResponse(info.Data);
                MessageBox.Show(response.Message);
            }
        }

        /// <summary>
        /// Handles the Start Game button click event (only visible to admin).
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void StartGameClick(object sender, RoutedEventArgs e)
        {
            socketHandler.Send("", (byte)CODES.START_GAME_REQUEST);
            ResponseInfo info = socketHandler.Receive();
            logout = true;
            finished = true;
            new GamePage(username, mRoomData).Show();
            this.Close();
        }

        /// <summary>
        /// Handles the closing event of the window to perform cleanup and logout if needed.
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void ClosingHandler(object sender, System.ComponentModel.CancelEventArgs e)
        {
            finished = true;

            /// Perform logout actions if logout flag is not set
            if (!logout)
            {
                if (isAdmin)
                {
                    /// Close room if admin
                    socketHandler.Send("", (byte)CommunicationHandling.CODES.CLOSE_ROOM_REQUEST);
                    ResponseInfo info = socketHandler.Receive();
                    try
                    {
                        CloseRoomResponse response = JsonPacketDeserializer.DeserializeCloseRoomResponse(info.Data);
                    }
                    catch
                    {
                        ErrorResponse response = JsonPacketDeserializer.DeserializeErrorResponse(info.Data);
                        MessageBox.Show(response.Message);
                    }

                    /// Logout
                    socketHandler.Send("", (byte)CommunicationHandling.CODES.LOGOUT_REQUEST);
                    info = socketHandler.Receive();
                    try
                    {
                        LogoutResponse response = JsonPacketDeserializer.DeserializeLogoutResponse(info.Data);
                    }
                    catch
                    {
                        ErrorResponse response = JsonPacketDeserializer.DeserializeErrorResponse(info.Data);
                        MessageBox.Show(response.Message);
                    }
                }
                else
                {
                    /// Leave room if not admin
                    socketHandler.Send("", (byte)CommunicationHandling.CODES.LEAVE_ROOM_REQUEST);
                    ResponseInfo info = socketHandler.Receive();
                    try
                    {
                        LeaveRoomResponse response = JsonPacketDeserializer.DeserializeLeaveRoomResponse(info.Data);
                    }
                    catch
                    {
                        ErrorResponse response = JsonPacketDeserializer.DeserializeErrorResponse(info.Data);
                        MessageBox.Show(response.Message);
                    }

                    /// Logout
                    socketHandler.Send("", (byte)CommunicationHandling.CODES.LOGOUT_REQUEST);
                    info = socketHandler.Receive();
                    try
                    {
                        LogoutResponse response = JsonPacketDeserializer.DeserializeLogoutResponse(info.Data);
                    }
                    catch
                    {
                        ErrorResponse response = JsonPacketDeserializer.DeserializeErrorResponse(info.Data);
                        MessageBox.Show(response.Message);
                    }
                }
            }
        }
    }
}
