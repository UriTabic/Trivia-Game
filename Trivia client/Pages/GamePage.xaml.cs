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

namespace Trivia_client.Pages
{
    public partial class GamePage : Window
    {
        static class Constants
        {
            public const int GAME_ENDED_RESPONSE = 0;
            public const int BUTTON_NOT_CLICKED = -1;
            public const int FIRST_ANSWER_INDEX = 0;
            public const int SECOND_ANSWER_INDEX = 1;
            public const int THIRD_ANSWER_INDEX = 2;
            public const int FOURTH_ANSWER_INDEX = 3;
            public const int ALMOST_ONE_SECOND = 999;
            public const int THREE_SECONDS = 3000;
            public const int NOT_EVERYONE_ANSWERED_RESPONSE = 0;
            public const double SCORE_MULTIPLIER = 10000.0;
            public const int TEXT_WIDTH = 110;
            public const int HEADLINE_INDEX = 0;
            public const int DETAILS_INDEX = 1;
            public const int FIRST_PLACE = 1;
            public const int SECOND_PLACE = 2;
            public const int THIRD_PLACE = 3;
        }

        /// Private members for game state, user information, and socket handling
        private bool finished;
        private SocketHandler socketHandler;
        private bool logout = false;
        private string mUsername;
        private RoomData mRoomData;
        private Thread quesThread;
        private int buttonClicked;

        /// <summary>
        /// Constructor for the GamePage window
        /// </summary>
        /// <param name="username">The username of the user</param>
        /// <param name="roomData">The data of the room</param>
        public GamePage(string username, RoomData roomData)
        {
            mUsername = username;
            mRoomData = roomData;
            InitializeComponent();
            RoomName.Text += mRoomData.name;
            Username.Text += mUsername;
            Closing += new System.ComponentModel.CancelEventHandler(ClosingHandler);
            socketHandler = SocketHandler.getInstance();
            finished = false;
            quesThread = new Thread(() => Play());
            quesThread.Start();
            buttonClicked = Constants.BUTTON_NOT_CLICKED;
        }

        /// <summary>
        /// Event handler for the "Back" button click
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void BackClick(object sender, RoutedEventArgs e)
        {
            logout = true;
            socketHandler.Send("", (byte)CommunicationHandling.CODES.LEAVE_GAME_REQUEST);
            ResponseInfo info = socketHandler.Receive();
            try
            {
                LeaveGameResponse lg = JsonPacketDeserializer.DeserializeLeaveGameResponse(info.Data);
            }
            catch
            {
                ErrorResponse response = JsonPacketDeserializer.DeserializeErrorResponse(info.Data);
                MessageBox.Show(response.Message);
            }
            finished = true;
            new MenuPage(mUsername).Show();
            this.Close();
        }

        /// <summary>
        /// Main game loop for handling questions and answers
        /// </summary>
        private void Play()
        {
            int timeCount = mRoomData.timePerQuestion;
            bool hasClicked = false;
            int currQuestion = 0;

            while (!finished)
            {
                /// Fetch and display the next question
                this.Dispatcher.Invoke(() =>
                {
                    socketHandler.Send("", (byte)CommunicationHandling.CODES.GET_QUESTION_REQUEST);
                    ResponseInfo info = socketHandler.Receive();
                    if(info.Code == CODES.GET_QUESTION_RESPONSE)
                    {
                        QuestionCounter.Text = "Question " + (++currQuestion).ToString() + "/" + mRoomData.numOfQuestionsInGame.ToString();
                        GetQuestionResponse response = JsonPacketDeserializer.DeserializeGetQuestionResponse(info.Data);
                        if (response.status == Constants.GAME_ENDED_RESPONSE)
                        {
                            finished = true;
                            return;
                        }
                        Answer1.Content = response.answers[Constants.FIRST_ANSWER_INDEX];
                        Answer1.Background = Brushes.Cyan;
                        Answer2.Content = response.answers[Constants.SECOND_ANSWER_INDEX];
                        Answer2.Background = Brushes.Cyan;
                        Answer3.Content = response.answers[Constants.THIRD_ANSWER_INDEX];
                        Answer3.Background = Brushes.Cyan;
                        Answer4.Content = response.answers[Constants.FOURTH_ANSWER_INDEX];
                        Answer4.Background = Brushes.Cyan;
                        Question.Text = response.question;
                    }
                    else
                    {
                        ErrorResponse response = JsonPacketDeserializer.DeserializeErrorResponse(info.Data);
                        MessageBox.Show(response.Message);
                    }
                });

                if (finished) break;

                /// Countdown timer for the question
                timeCount = mRoomData.timePerQuestion;
                while (timeCount > 0)
                {
                    this.Dispatcher.Invoke(() =>
                    {
                        MyTimer.Text = timeCount.ToString();
                    });
                    timeCount--;

                    if (buttonClicked != Constants.BUTTON_NOT_CLICKED)
                    {
                        hasClicked = true;
                        bool res = SubmitAnswer(buttonClicked);
                        if (res)
                        {
                            buttonClicked = Constants.BUTTON_NOT_CLICKED;
                            break;
                        }
                    }

                    Thread.Sleep(Constants.ALMOST_ONE_SECOND);
                }

                if (finished) break;

                if (!hasClicked)
                {
                    SubmitAnswer(Constants.BUTTON_NOT_CLICKED);
                }
                if (finished) break;
                UpdateLeaderboard();
                Thread.Sleep(Constants.THREE_SECONDS);
            }
            if (logout) return;
            logout = true;
            this.Dispatcher.Invoke(() => {
                new FinalResults(mUsername).Show();
                this.Close();
            });
        }
        
        /// <summary>
        /// Submits the selected answer to the server
        /// </summary>
        /// <param name="answerId">The answer id</param>
        /// <returns>Returns true if all the users answered the question, and false if not</returns>
        private bool SubmitAnswer(int answerId)
        {
            SubmitAnswerRequest answerRequest = new SubmitAnswerRequest();
            answerRequest.answerId = answerId;
            string msg = JsonPacketSerializer.Serialize(answerRequest);
            socketHandler.Send(msg, (byte)CommunicationHandling.CODES.SUBMIT_ANSWER_REQUEST);
            ResponseInfo info = socketHandler.Receive();
            try
            {
                SubmitAnswerResponse response = JsonPacketDeserializer.DeserializeSubmitAnswerResponse(info.Data);
                if (response.status == Constants.NOT_EVERYONE_ANSWERED_RESPONSE)
                {
                    this.Dispatcher.Invoke(() => {
                        GetAnswerButton(answerId).Background = Brushes.Blue;
                    });
                    return false;
                }

                if (answerId == response.correctAnswerId)
                {
                    this.Dispatcher.Invoke(() => {
                        GetAnswerButton(answerId).Background = Brushes.Green;
                    });
                }
                else
                {
                    this.Dispatcher.Invoke(() =>
                    {
                        GetAnswerButton(answerId).Background = Brushes.Red;
                        GetAnswerButton((int)response.correctAnswerId).Background = Brushes.Green;
                    });
                }
                return true;
            }
            catch
            {
                ErrorResponse response = JsonPacketDeserializer.DeserializeErrorResponse(info.Data);
                MessageBox.Show(response.Message);
            }
            return false;
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
                socketHandler.Send("", (byte)CommunicationHandling.CODES.LEAVE_GAME_REQUEST);
                ResponseInfo info = socketHandler.Receive();
                try
                {
                    LeaveGameResponse lg = JsonPacketDeserializer.DeserializeLeaveGameResponse(info.Data);
                }
                catch
                {
                    ErrorResponse response = JsonPacketDeserializer.DeserializeErrorResponse(info.Data);
                    MessageBox.Show(response.Message);
                }

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

        /// <summary>
        /// Returns the button corresponding to the answer ID
        /// </summary>
        /// <param name="answerId">The answer id</param>
        /// <returns>The answer id's button</returns>
        private Button GetAnswerButton(int answerId)
        {
            switch (answerId)
            {
                case Constants.FIRST_ANSWER_INDEX:
                    return Answer1;
                case Constants.SECOND_ANSWER_INDEX:
                    return Answer2;
                case Constants.THIRD_ANSWER_INDEX:
                    return Answer3;
                case Constants.FOURTH_ANSWER_INDEX:
                    return Answer4;
                default:
                    return Answer1;
            }
        }

        /// <summary>
        /// Updates the leaderboard with current scores
        /// </summary>
        private void UpdateLeaderboard()
        {
            if(finished) return;
            this.Dispatcher.Invoke(() =>
            {
                UIElement healine = Leaderboard.Children[Constants.HEADLINE_INDEX];
                UIElement details = Leaderboard.Children[Constants.DETAILS_INDEX];
                Leaderboard.Children.Clear();
                Leaderboard.Children.Add(healine);
                Leaderboard.Children.Add(details);
                Dictionary<string, uint> scores = new Dictionary<string, uint>();
                int pos = 1;

                socketHandler.Send("", (byte)CommunicationHandling.CODES.GET_GAME_RESULTS_REQUEST);
                ResponseInfo info = socketHandler.Receive();
                try
                {
                    GetGameResultsResponse response = JsonPacketDeserializer.DeserializeGetGameResultsResponse(info.Data);
                    foreach (PlayerResults results in response.results)
                    {
                        uint score;
                        try
                        {
                            score = (uint)((((double)results.correctAnswerCount / (double)(results.correctAnswerCount + results.wrongAnswerCount)) / (double)results.averageAnswerTime) * Constants.SCORE_MULTIPLIER);
                        }
                        catch
                        {
                            score = 0;
                        }
                        scores.Add(results.username, score);
                    }
                }
                catch
                {
                    ErrorResponse response = JsonPacketDeserializer.DeserializeErrorResponse(info.Data);
                    MessageBox.Show(response.Message);
                }

                foreach (KeyValuePair<string, uint> score in scores.OrderBy(key => -key.Value))
                {
                    StackPanel panel = new StackPanel();
                    panel.Orientation = Orientation.Horizontal;
                    TextBlock text = new TextBlock();
                    text.Text = score.Key;
                    text.Width = Constants.TEXT_WIDTH;
                    TextBlock points = new TextBlock();
                    points.Text = score.Value.ToString();

                    panel.Children.Add(text);
                    panel.Children.Add(points);
                    switch (pos)
                    {
                        case Constants.FIRST_PLACE:
                            panel.Background = Brushes.Gold;
                            break;
                        case Constants.SECOND_PLACE:
                            panel.Background = Brushes.Silver;
                            break;
                        case Constants.THIRD_PLACE:
                            panel.Background = new SolidColorBrush(Color.FromRgb(205, 127, 50));
                            break;
                    }
                    Leaderboard.Children.Add(panel);
                    pos++;
                }
            });
        }

        /// <summary>
        /// Event handlers for answer button clicks
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void Answer1_Click(object sender, RoutedEventArgs e)
        {
            buttonClicked = Constants.FIRST_ANSWER_INDEX;
        }

        private void Answer2_Click(object sender, RoutedEventArgs e)
        {
            buttonClicked = Constants.SECOND_ANSWER_INDEX;
        }

        private void Answer3_Click(object sender, RoutedEventArgs e)
        {
            buttonClicked = Constants.THIRD_ANSWER_INDEX;
        }

        private void Answer4_Click(object sender, RoutedEventArgs e)
        {
            buttonClicked = Constants.FOURTH_ANSWER_INDEX;
        }
    }
}
