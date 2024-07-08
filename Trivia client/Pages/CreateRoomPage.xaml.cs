using Trivia_client.CommunicationHandling;
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
using System.Windows.Navigation;
using System.Windows.Shapes;
using Trivia_client.Pages;

namespace Trivia_client.Pages
{

    public partial class CreateRoomPage : Window
    {
        static class Constants
        {
            public const int SUCCESS = 1;
        }

        /// Private members for socket handling and user information
        private SocketHandler socketHandler;
        private string username;
        private bool logout = false;

        /// <summary>
        /// Constructor for CreateRoomPage
        /// </summary>
        /// <param name="username">The username of the user</param>
        public CreateRoomPage(string username)
        {
            /// Initialize the socket handler and user name
            socketHandler = SocketHandler.getInstance();
            this.username = username;
            InitializeComponent();

            /// Attach the closing event handler
            Closing += new System.ComponentModel.CancelEventHandler(ClosingHandler);
        }

        /// <summary>
        /// Event handler for when a TextBox gets focus
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void TextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            TextBox textBox = (TextBox)sender;
            TextBlock placeholder = GetPlaceholder(textBox.Name);
            placeholder.Visibility = Visibility.Collapsed; /// Hide the placeholder text
        }

        /// <summary>
        /// Event handler for when a TextBox loses focus
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void TextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            TextBox textBox = (TextBox)sender;
            TextBlock placeholder = GetPlaceholder(textBox.Name);
            if (string.IsNullOrWhiteSpace(textBox.Text))
            {
                placeholder.Visibility = Visibility.Visible; /// Show the placeholder text if TextBox is empty
            }
        }

        /// <summary>
        /// Helper method to get the corresponding placeholder for a given TextBox name
        /// </summary>
        /// <param name="name">The name of the placeholder</param>
        /// <returns>The placeholder according to the name</returns>
        private TextBlock GetPlaceholder(string name)
        {
            switch (name)
            {
                case "RoomName":
                    return RoomNamePlaceholder;
                case "NumberOfPlayers":
                    return NumberOfPlayersPlaceholder;
                case "NumberOfQuestions":
                    return NumberOfQuestionsPlaceholder;
                case "TimeForQuestion":
                    return TimeForQuestionPlaceholder;
                default:
                    return null;
            }
        }

        /// <summary>
        /// Event handler for the Create Room button click
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void CreateRoomClick(object sender, RoutedEventArgs e)
        {
            bool success = true;
            string roomName = RoomName.Text;
            string numberOfPlayers = NumberOfPlayers.Text;
            string numberOfQuestions = NumberOfQuestions.Text;
            string timeForQuestion = TimeForQuestion.Text;

            /// Validate inputs and create room request if inputs are valid
            if (!string.IsNullOrEmpty(roomName) && !string.IsNullOrEmpty(numberOfPlayers) && !string.IsNullOrEmpty(numberOfQuestions) && !string.IsNullOrEmpty(timeForQuestion))
            {
                CreateRoomRequest createRoomRequest = new CreateRoomRequest();
                createRoomRequest.roomName = roomName;

                /// Validate and parse number of players
                success = int.TryParse(numberOfPlayers, out int playersC);
                if (!success)
                {
                    MessageBox.Show("Illegal room settings.");
                    return;
                }
                createRoomRequest.maxUsers = (uint)playersC;

                /// Validate and parse number of questions
                success = int.TryParse(numberOfQuestions, out int questionC);
                if (!success)
                {
                    MessageBox.Show("Illegal room settings.");
                    return;
                }
                createRoomRequest.questionCount = (uint)questionC;

                /// Validate and parse time for each question
                success = int.TryParse(timeForQuestion, out int answerT);
                if (!success)
                {
                    MessageBox.Show("Illegal room settings.");
                    return;
                }
                createRoomRequest.answerTimeout = (uint)answerT;

                /// Serialize the create room request and send it to the server
                string msg = JsonPacketSerializer.Serialize(createRoomRequest);
                socketHandler.Send(msg, (byte)CODES.CREATE_ROOM_REQUEST);

                /// Receive the response from the server
                ResponseInfo info = socketHandler.Receive();
                CreateRoomResponse data = new CreateRoomResponse();
                try
                {
                    data = JsonPacketDeserializer.DeserializeCreateRoomResponse(info.Data);
                }
                catch
                {
                    ErrorResponse errorResponse = JsonPacketDeserializer.DeserializeErrorResponse(info.Data);
                    MessageBox.Show(errorResponse.Message);
                }

                /// If the room creation is successful, navigate to the WaitForGamePage
                if (data.status == Constants.SUCCESS)
                {
                    logout = true;
                    WaitForGamePage waitForGamePage = new WaitForGamePage(username, true, new RoomData
                    {
                        id = (int)data.roomId,
                        maxPlayers = (int)createRoomRequest.maxUsers,
                        name = createRoomRequest.roomName,
                        numOfQuestionsInGame = (int)createRoomRequest.questionCount,
                        state = (int)RoomState.OPENED,
                        timePerQuestion = (int)createRoomRequest.answerTimeout
                    });
                    this.Close();
                    waitForGamePage.Show();
                }
            }
            else
            {
                MessageBox.Show("You think you're funny trying to send nothing?!");
            }
        }

        /// <summary>
        /// Event handler for the Back button click
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void BackClick(object sender, RoutedEventArgs e)
        {
            logout = true;
            new MenuPage(username).Show(); /// Navigate to the MenuPage
            this.Close(); /// Close the current page
        }

        /// <summary>
        /// Event handler for the window closing event
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void ClosingHandler(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (!logout)
            {
                /// Send a logout request if the user is not logging out explicitly
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
