using System;
using System.Windows;
using System.Windows.Controls;
using Trivia_client.CommunicationHandling;

namespace Trivia_client.Pages
{
    public partial class SignupPage : Window
    {
        static class Constants
        {
            public const int SUCCESS = 1;
        }

        SocketHandler socketHandler;
        
        /// <summary>
        /// Constructor to initialize the SignupPage and obtain a SocketHandler instance.
        /// </summary>
        public SignupPage()
        {
            socketHandler = SocketHandler.getInstance();
            InitializeComponent();
        }

        /// <summary>
        /// Redirects to the Login page when the corresponding button is clicked.
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void redirectToLogin(object sender, RoutedEventArgs e)
        {
            LoginPage loginPage = new LoginPage();
            this.Close();
            loginPage.Show();
        }

        /// <summary>
        /// Handles the GotFocus event for text boxes to hide their placeholder.
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void TextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            TextBox textBox = (TextBox)sender;
            TextBlock placeholder = GetPlaceholder(textBox.Name);
            placeholder.Visibility = Visibility.Collapsed;
        }

        /// <summary>
        /// Handles the LostFocus event for text boxes to show their placeholder if empty.
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void TextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            TextBox textBox = (TextBox)sender;
            TextBlock placeholder = GetPlaceholder(textBox.Name);
            if (string.IsNullOrWhiteSpace(textBox.Text))
            {
                placeholder.Visibility = Visibility.Visible;
            }
        }

        /// <summary>
        /// Handles the GotFocus event for password boxes to hide their placeholder.
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void PasswordBox_GotFocus(object sender, RoutedEventArgs e)
        {
            PasswordBox passwordBox = (PasswordBox)sender;
            TextBlock placeholder = GetPlaceholder(passwordBox.Name);
            placeholder.Visibility = Visibility.Collapsed;
        }

        /// <summary>
        /// Handles the LostFocus event for password boxes to show their placeholder if empty.
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void PasswordBox_LostFocus(object sender, RoutedEventArgs e)
        {
            PasswordBox passwordBox = (PasswordBox)sender;
            TextBlock placeholder = GetPlaceholder(passwordBox.Name);
            if (string.IsNullOrWhiteSpace(passwordBox.Password))
            {
                placeholder.Visibility = Visibility.Visible;
            }
        }

        /// <summary>
        /// Returns the corresponding placeholder TextBlock based on the provided name.
        /// </summary>
        /// <param name="name">The name of the placeholder</param>
        /// <returns>The placeholder according to the name</returns>
        private TextBlock GetPlaceholder(string name)
        {
            switch (name)
            {
                case "Mail":
                    return MailPlaceholder;
                case "Username":
                    return UsernamePlaceholder;
                case "Password":
                    return PasswordPlaceholder;
                default:
                    return null;
            }
        }

        /// <summary>
        /// Handles the SignUp button click event.
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void SignUpClick(object sender, RoutedEventArgs e)
        {
            string mail = Mail.Text;
            string username = Username.Text;
            string password = Password.Password;

            /// Checks if all required fields are filled before proceeding with signup.
            if (!string.IsNullOrEmpty(mail) && !string.IsNullOrEmpty(username) && !string.IsNullOrEmpty(password))
            {
                SignupRequest signupRequest = new SignupRequest();
                signupRequest.email = mail;
                signupRequest.username = username;
                signupRequest.password = password;
                string msg = JsonPacketSerializer.Serialize(signupRequest);
                socketHandler.Send(msg, (byte)CODES.SIGNUP_REQUEST);
                ResponseInfo info = socketHandler.Receive();
                SignupResponse data = new SignupResponse();
                try
                {
                    data = JsonPacketDeserializer.DeserializeSignupResponse(info.Data);
                }
                catch
                {
                    ErrorResponse errorResponse = JsonPacketDeserializer.DeserializeErrorResponse(info.Data);
                    MessageBox.Show(errorResponse.Message);
                }
                if (data.status == Constants.SUCCESS)
                {
                    /// If signup is successful, navigate to the MenuPage for the user.
                    MenuPage menuPage = new MenuPage(username);
                    this.Close();
                    menuPage.Show();
                }
            }
            else
            {
                /// Displays an error message if any required field is empty.
                MessageBox.Show("You think you're funny trying to send nothing?!");
            }
        }
    }
}
