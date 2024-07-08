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
using Trivia_client.CommunicationHandling;
using static System.Formats.Asn1.AsnWriter;

namespace Trivia_client.Pages
{

    public partial class FinalResults : Window
    {
        static class Constants
        {
            public const int TEXT_WIDTH = 500;
            public const int TEXT_FONTSIZE = 25;
            public const int FIRST_PLACE = 1;
            public const int SECOND_PLACE = 2;
            public const int THIRD_PLACE = 3;
        }

        /// Private members for user information and socket handling
        private string username;
        private bool logout = false;
        private SocketHandler socketHandler;

        /// <summary>
        /// Constructor for the FinalResults window
        /// </summary>
        /// <param name="username">The username of the user</param>
        public FinalResults(string username)
        {
            socketHandler = SocketHandler.getInstance(); /// Initialize the socket handler
            InitializeComponent();
            this.username = username;
            Closing += new System.ComponentModel.CancelEventHandler(ClosingHandler); /// Attach the closing event handler
            UpdateLeaderboard(); /// Update the leaderboard upon initialization
        }

        /// <summary>
        /// Event handler for the "Back to Menu" button click
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void BackToMenu(object sender, RoutedEventArgs e)
        {
            logout = true; /// Set logout flag to true
            new MenuPage(username).Show(); /// Navigate to the MenuPage
            this.Close(); /// Close the current window
        }

        /// <summary>
        /// Method to update the leaderboard with player scores
        /// </summary>
        private void UpdateLeaderboard()
        {
            Dictionary<string, uint> scores = new Dictionary<string, uint>();
            int pos = 1;

            /// Send a request to get game results
            socketHandler.Send("", (byte)CommunicationHandling.CODES.GET_GAME_RESULTS_REQUEST);
            ResponseInfo info = socketHandler.Receive();

            try
            {
                /// Deserialize the game results response
                GetGameResultsResponse response = JsonPacketDeserializer.DeserializeGetGameResultsResponse(info.Data);

                /// Calculate and store scores for each player
                foreach (PlayerResults results in response.results)
                {
                    uint score = (uint)((((double)results.correctAnswerCount / (double)(results.correctAnswerCount + results.wrongAnswerCount)) / (double)results.averageAnswerTime) * 10000.0);
                    scores.Add(results.username, score);
                }
            }
            catch
            {
                /// Handle error response if deserialization fails
                ErrorResponse response = JsonPacketDeserializer.DeserializeErrorResponse(info.Data);
                MessageBox.Show(response.Message);
            }

            /// Sort and display the scores on the leaderboard
            foreach (KeyValuePair<string, uint> score in scores.OrderBy(key => -key.Value))
            {
                StackPanel panel = new StackPanel();
                panel.Orientation = Orientation.Horizontal;

                TextBlock text = new TextBlock();
                text.Text = score.Key;
                text.Width = Constants.TEXT_WIDTH;
                text.FontSize = Constants.TEXT_FONTSIZE;

                TextBlock points = new TextBlock();
                points.Text = score.Value.ToString();
                points.FontSize = Constants.TEXT_FONTSIZE;

                panel.Children.Add(text);
                panel.Children.Add(points);

                /// Set background color based on position
                switch (pos)
                {
                    case Constants.FIRST_PLACE:
                        panel.Background = Brushes.Gold;
                        break;
                    case Constants.SECOND_PLACE:
                        panel.Background = Brushes.Silver;
                        break;
                    case Constants.THIRD_PLACE:
                        panel.Background = new SolidColorBrush(Color.FromRgb(205, 127, 50)); /// Bronze color
                        break;
                }

                Leaderboard.Children.Add(panel);
                pos++;
            }
        }

        /// <summary>
        /// Event handler for the window closing event
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void ClosingHandler(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (!logout)
                new MenuPage(username).Show(); /// Navigate to the MenuPage if not logging out
        }
    }
}
