using System;
using System.Windows;
using Trivia_client.CommunicationHandling;

namespace Trivia_client.Pages
{
    public partial class StatisitcsPage : Window
    {
        static class Constants
        {
            public const int FAILURE = 0;
        }

        string username;
        SocketHandler socketHandler;
        bool logout = false;
        
        /// <summary>
        /// Constructor to initialize the StatisticsPage and obtain a SocketHandler instance.
        /// </summary>
        /// <param name="username">The username of the user</param>
        public StatisitcsPage(string username)
        {
            InitializeComponent();
            this.username = username;
            socketHandler = SocketHandler.getInstance();
            Closing += new System.ComponentModel.CancelEventHandler(ClosingHandler);
        }

        /// <summary>
        /// Retrieves and displays the user's personal statistics.
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void MyStatisticsClick(object sender, RoutedEventArgs e)
        {
            stats_data.Visibility = Visibility.Visible;
            stats_label.Visibility = Visibility.Visible;

            stats_label.Content = "My stats:";
            socketHandler.Send("", (byte)CommunicationHandling.CODES.GET_PERSONAL_STATS_REQUEST);
            ResponseInfo info = socketHandler.Receive();
            try
            {
                GetPersonalStatsResponse data = JsonPacketDeserializer.DeserializeGetPersonalStatsResponse(info.Data);
                if (data.status == Constants.FAILURE)
                {
                    stats_data.Text = "Error getting data from server.\nPlease try again.";
                    return;
                }
                stats_data.Text = "";
                foreach (string s in data.statistics)
                {
                    stats_data.Text += s;
                    stats_data.Text += "\n";
                }
            }
            catch
            {
                ErrorResponse response = JsonPacketDeserializer.DeserializeErrorResponse(info.Data);
                MessageBox.Show(response.Message);
            }
        }

        /// <summary>
        /// Retrieves and displays the top games statistics.
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void TopGamesClick(object sender, RoutedEventArgs e)
        {
            stats_data.Visibility = Visibility.Visible;
            stats_label.Visibility = Visibility.Visible;

            stats_label.Content = "Top Games:";
            socketHandler.Send("", (byte)CommunicationHandling.CODES.GET_HIGH_SCORE_REQUEST);
            ResponseInfo info = socketHandler.Receive();
            GetHighScoreResponse data = JsonPacketDeserializer.DeserializeHighScoreResponse(info.Data);
            stats_data.Text = "";
            if (data.status == Constants.FAILURE)
            {
                stats_data.Text = "Error getting data from server.\nPlease try again.";
                return;
            }
            foreach (string s in data.statistics)
            {
                stats_data.Text += s;
                stats_data.Text += "\n";
            }
        }

        /// <summary>
        /// Handles the Back button click event to return to the MenuPage.
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void BackClick(object sender, RoutedEventArgs e)
        {
            logout = true;
            new MenuPage(username).Show();
            this.Close();
        }

        /// <summary>
        /// Handles the closing event of the window to perform logout if needed.
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
