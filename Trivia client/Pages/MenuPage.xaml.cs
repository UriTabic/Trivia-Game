using System;
using System.Windows;
using System.Windows.Controls;
using Trivia_client.CommunicationHandling;

namespace Trivia_client.Pages
{
    public partial class MenuPage : Window
    {
        static class Constants
        {
            public const int HEADLINE_FONT_SIZE = 24;
            public const int FAILURE_RESPONSE = 0;
        }

        string username;
        SocketHandler socketHandler;
        bool logout = false;

        /// <summary>
        /// Constructor to initialize the MenuPage for a specific user.
        /// </summary>
        /// <param name="username">The username of the user</param>
        public MenuPage(string username)
        {
            InitializeComponent();
            this.username = username;
            socketHandler = SocketHandler.getInstance();
            headline.Content = "Hello " + username;
            headline.FontSize = Constants.HEADLINE_FONT_SIZE;
            headline.FontWeight = FontWeights.Bold;
            Closing += new System.ComponentModel.CancelEventHandler(ClosingHandler);
        }

        /// <summary>
        /// Handles the Signout button click event.
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void SignoutClick(object sender, RoutedEventArgs e)
        {
            MessageBoxButton button = MessageBoxButton.YesNo;
            MessageBoxResult result = MessageBox.Show("Are you sure you want to signout?", "Page says:", button);

            if (result == MessageBoxResult.Yes)
            {
                socketHandler.Send("", (byte)CommunicationHandling.CODES.LOGOUT_REQUEST);
                ResponseInfo info = socketHandler.Receive();
                try
                {
                    LogoutResponse lr = JsonPacketDeserializer.DeserializeLogoutResponse(info.Data);
                    if (lr.status != Constants.FAILURE_RESPONSE)
                    {
                        logout = true;
                        LoginPage loginPage = new LoginPage();
                        loginPage.Show();
                        this.Close();
                    }
                    else
                    {
                        MessageBox.Show("Error trying to logout. please try again.");
                    }
                }
                catch
                {
                    ErrorResponse response = JsonPacketDeserializer.DeserializeErrorResponse(info.Data);
                    MessageBox.Show(response.Message);
                }
            }
        }

        /// <summary>
        /// Handles the Statistics button click event.
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void StatisticsClick(object sender, RoutedEventArgs e)
        {
            logout = true;
            StatisitcsPage statPage = new StatisitcsPage(username);
            this.Close();
            statPage.Show();
        }

        /// <summary>
        /// Handles the Create Room button click event.
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void CreateRoomClick(object sender, RoutedEventArgs e)
        {
            logout = true;
            new CreateRoomPage(username).Show();
            this.Close();
        }

        /// <summary>
        /// Handles the Join Room button click event.
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void JoinRoomClick(object sender, RoutedEventArgs e)
        {
            logout = true;
            new JoinRoomPage(username).Show();
            this.Close();
        }

        /// <summary>
        /// Handles the window closing event to ensure proper logout.
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void ClosingHandler(object sender, System.ComponentModel.CancelEventArgs e)
        {
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
