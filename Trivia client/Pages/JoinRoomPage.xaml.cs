using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Xml.Linq;
using Trivia_client.CommunicationHandling;

namespace Trivia_client.Pages
{
    public partial class JoinRoomPage : Window
    {
        static class Constants
        {
            public const int TEXT_MINIMUM_WIDTH = 150;
            public const int TEXT_HEIGHT = 68;
            public const int TEXT_PLAYER_COUNT_HEIGHT_MULTIPLIER = 26;
            public const int TEXT_FONT_SIZE = 20;
            public const int BUTTON_HEIGHT_ADDITION = 1;
            public const int NO_ROOM_TEXT_WIDTH = 600;
            public const int NO_ROOM_TEXT_FONT_SIZE = 24;
            public const int ONE_SECOND = 1000;
            public const int FAILURE_RESPONSE = 0;
        }

        /// Private members for the state of the page, user information, and socket handling
        private bool finished;
        private string username;
        private SocketHandler socketHandler;
        private bool logout = false;
        private Thread refreshThread;

        /// <summary>
        /// Constructor for the JoinRoomPage window
        /// </summary>
        /// <param name="username">The username of the user</param>
        public JoinRoomPage(string username)
        {
            InitializeComponent();
            this.username = username;
            socketHandler = SocketHandler.getInstance();
            Closing += new System.ComponentModel.CancelEventHandler(ClosingHandler);
            finished = false;
            refreshThread = new Thread(() => RefreshClick(this, new RoutedEventArgs()));
            refreshThread.Start();
        }

        /// <summary>
        /// Method to refresh the list of available rooms
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void RefreshClick(object sender, RoutedEventArgs e)
        {
            while (!finished)
            {
                this.Dispatcher.Invoke(() =>
                {
                    int gridSize = 0;
                    myGrid.Children.Clear();

                    socketHandler.Send("", (byte)CommunicationHandling.CODES.GET_ROOMS_REQUEST);
                    ResponseInfo info = socketHandler.Receive();
                    try
                    {
                        GetRoomsResponse response = JsonPacketDeserializer.DeserializeGetRoomsResponse(info.Data);
                        for (int i = 0; i < response.Rooms.Count; i++)
                        {
                            StackPanel roomPanel = new StackPanel();
                            roomPanel.Orientation = Orientation.Horizontal;
                            RoomData room = response.Rooms[i];
                            if (room.state == (int)RoomState.STARTED) continue;

                            GetPlayersInRoomRequest req = new GetPlayersInRoomRequest { roomId = room.id };
                            socketHandler.Send(JsonPacketSerializer.SerializeGetPlayersRequest(req), (byte)CommunicationHandling.CODES.GET_PLAYERS_IN_ROOM_REQUEST);
                            ResponseInfo responseInfo = socketHandler.Receive();

                            string players = "";
                            int playerCount = 0;
                            try
                            {
                                GetPlayersInRoomResponse playersResponse = JsonPacketDeserializer.DeserializeGetPlayersInRoomResponse(responseInfo.Data);
                                foreach (string player in playersResponse.players)
                                {
                                    players += player + "\n";
                                    playerCount++;
                                }
                            }
                            catch
                            {
                                players = string.Empty;
                            }

                            TextBlock name = new TextBlock
                            {
                                Text = "\"" + room.name + "\"\nPlayers:\n" + players,
                                Background = room.state == (int)RoomState.OPENED && playerCount < response.Rooms[i].maxPlayers ? Brushes.Green : Brushes.Gray,
                                HorizontalAlignment = HorizontalAlignment.Center,
                                VerticalAlignment = VerticalAlignment.Center,
                                TextAlignment = TextAlignment.Justify,
                                MinWidth = Constants.TEXT_MINIMUM_WIDTH,
                                Height = Constants.TEXT_HEIGHT + playerCount * Constants.TEXT_PLAYER_COUNT_HEIGHT_MULTIPLIER,
                                FontSize = Constants.TEXT_FONT_SIZE
                            };
                            roomPanel.Children.Add(name);

                            gridSize += (int)name.Height;

                            Button join = new Button
                            {
                                Content = "\nJoin :)",
                                Background = Brushes.LightGreen,
                                Height = name.Height,
                                HorizontalAlignment = HorizontalAlignment.Center,
                                VerticalAlignment = VerticalAlignment.Center,
                                VerticalContentAlignment = VerticalAlignment.Top
                            };
                            join.Click += (s, e) => { JoinClick(room); };
                            roomPanel.Children.Add(join);
                            roomPanel.Height = name.Height + Constants.BUTTON_HEIGHT_ADDITION;

                            myGrid.Children.Add(roomPanel);
                        }

                        if (myGrid.Children.Count == 0)
                        {
                            TextBlock textBlock = new TextBlock
                            {
                                Width = Constants.NO_ROOM_TEXT_WIDTH,
                                FontSize = Constants.NO_ROOM_TEXT_FONT_SIZE,
                                HorizontalAlignment = HorizontalAlignment.Center,
                                VerticalAlignment = VerticalAlignment.Top,
                                Text = "No Available rooms right now :(\nYou can create your own room :)"
                            };
                            myGrid.Children.Add(textBlock);
                        }

                        myGrid.Height = gridSize;
                    }
                    catch
                    {
                    }
                });
                Thread.Sleep(Constants.ONE_SECOND);
            }
        }

        /// <summary>
        /// Event handler for the "Back" button click
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void BackClick(object sender, RoutedEventArgs e)
        {
            logout = true;
            new MenuPage(username).Show();
            this.Close();
        }

        //// Method to handle joining a selected room
        private void JoinClick(RoomData roomData)
        {
            JoinRoomRequest request = new JoinRoomRequest { roomId = roomData.id };
            socketHandler.Send(JsonPacketSerializer.SerializeJoinRoomRequest(request), (byte)CommunicationHandling.CODES.JOIN_ROOM_REQUEST);
            ResponseInfo info = socketHandler.Receive();
            try
            {
                JoinRoomResponse response = JsonPacketDeserializer.DeserializeJoinRoomResponse(info.Data);
                if (response.status != Constants.FAILURE_RESPONSE)
                {
                    logout = true;
                    new WaitForGamePage(username, false, roomData).Show();
                    this.Close();
                }
                else
                {
                    MessageBox.Show("Can't join this room");
                }
            }
            catch
            {
                ErrorResponse response = JsonPacketDeserializer.DeserializeErrorResponse(info.Data);
                MessageBox.Show(response.Message);
            }
        }

        /// <summary>
        /// Event handler for the window closing event
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void ClosingHandler(object sender, System.ComponentModel.CancelEventArgs e)
        {
            finished = true;
            if (!logout)
            {
                socketHandler.Send("", (byte)CommunicationHandling.CODES.LOGOUT_REQUEST);
                ResponseInfo info = socketHandler.Receive();
                try
                {
                    LogoutResponse lr = JsonPacketDeserializer.DeserializeLogoutResponse(info.Data);
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
