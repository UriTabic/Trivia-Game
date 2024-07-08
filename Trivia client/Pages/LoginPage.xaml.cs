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
    public partial class LoginPage : Window
    {
        static class Constants
        {
            public const int SUCCESS = 1;
        }

        /// Socket handler instance to manage server communication
        SocketHandler socketHandler;

        /// <summary>
        /// Constructor initializes the socket handler and components
        /// </summary>
        public LoginPage()
        {
            socketHandler = SocketHandler.getInstance();
            InitializeComponent();
        }

        /// <summary>
        /// Redirects to the Signup page
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void redirectToSignup(object sender, RoutedEventArgs e)
        {
            SignupPage signupPage = new SignupPage();
            this.Close();
            signupPage.Show();
        }

        /// <summary>
        /// Handles focus event for TextBox, hides placeholder
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
        /// Handles loss of focus event for TextBox, shows placeholder if empty
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
        /// Handles focus event for PasswordBox, hides placeholder
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
        /// Handles loss of focus event for PasswordBox, shows placeholder if empty
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
        /// Returns the corresponding placeholder for the TextBox/PasswordBox
        /// </summary>
        /// <param name="name">The name of the placeholder</param>
        /// <returns>The placeholder according to the name</returns>
        private TextBlock GetPlaceholder(string name)
        {
            switch (name)
            {
                case "Username":
                    return UsernamePlaceholder;
                case "Password":
                    return PasswordPlaceholder;
                default:
                    return null;
            }
        }

        /// <summary>
        /// Handles the login button click event
        /// </summary>
        /// <param name="sender">The sender</param>
        /// <param name="e">The events</param>
        private void LoginClick(object sender, RoutedEventArgs e)
        {
            string username = Username.Text;
            string password = Password.Password;

            if (!string.IsNullOrEmpty(username) && !string.IsNullOrEmpty(password))
            {
                LoginRequest loginRequest = new LoginRequest();
                loginRequest.username = username;
                loginRequest.password = password;
                string msg = JsonPacketSerializer.Serialize(loginRequest);
                socketHandler.Send(msg, (byte)CODES.LOGIN_REQUEST);
                ResponseInfo info = socketHandler.Receive();
                LoginResponse data = new LoginResponse();
                try
                {
                    data = JsonPacketDeserializer.DeserializeLoginResponse(info.Data);
                }
                catch
                {
                    ErrorResponse errorResponse = JsonPacketDeserializer.DeserializeErrorResponse(info.Data);
                    MessageBox.Show(errorResponse.Message);
                }
                if (data.status == Constants.SUCCESS)
                {
                    MenuPage menuPage = new MenuPage(username);
                    this.Close();
                    menuPage.Show();
                }
            }
            else
            {
                MessageBox.Show("You think you're funny trying to send nothing?!");
            }
        }
    }
}
