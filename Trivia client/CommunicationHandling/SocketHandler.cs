using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Text.Json;
using System.Windows;

namespace Trivia_client.CommunicationHandling
{

    /// <summary>
    /// Internal class for handling socket communication with the server
    /// </summary>
    internal class SocketHandler
    {
        static class Constants
        {
            public const int PORT = 8175;
            public const int CODE_AND_SIZE_BYTE_LENGTH = 5;
            public const int CODE_BYTE_POSITION = 0;
            public const int MESSAGE_SIZE_BYTE_LENGTH = 4;
        }

        /// Singleton instance of the SocketHandler
        private static SocketHandler instance = new SocketHandler();
        private IPAddress ipAddr; // IP address of the server
        private IPEndPoint localEndPoint; // Endpoint for connecting to the server
        private Socket sender; // Socket for communication

        /// <summary>
        /// Private constructor to implement Singleton pattern
        /// </summary>
        private SocketHandler()
        {
            byte[] serverIP = new byte[] { 127, 0, 0, 1 };
            int port = Constants.PORT;
            if (File.Exists("config.txt"))
            {
                try
                {
                    string content = File.ReadAllText("config.txt");
                    if (content.StartsWith("server_ip = ") && content.Contains("server_port = "))
                    {
                        var a = content.Split('\n');
                        string ip = a[0].Substring("server_ip = ".Length);
                        var splitIP = ip.Split('.');
                        for(int i = 0; i < splitIP.Length; i++)
                        {
                            serverIP[i] = (byte)int.Parse(splitIP[i]);
                        }
                        string portStr = a[1].Substring("server_port = ".Length);
                        port = int.Parse(portStr);
                    }
                }
                catch { }
            }
            ipAddr = new IPAddress(serverIP);
            localEndPoint = new IPEndPoint(ipAddr, port);
            sender = new Socket(ipAddr.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

            // Attempt to connect to the server
            while (!sender.Connected)
            {
                try
                {
                    sender.Connect(localEndPoint);
                }
                catch
                {
                    // Show message box if connection fails
                    MessageBoxButton button = MessageBoxButton.OKCancel;
                    string ip = string.Format("{0}.{1}.{2}.{3}", serverIP[0], serverIP[1], serverIP[2], serverIP[3]);
                    MessageBoxResult result = MessageBox.Show("Can't find server on ip : " + ip + " and port : " + port.ToString() +"\n\nOk - try again\nCancel - close program", "Connection Error:", button);

                    // Exit application if user cancels
                    if (result != MessageBoxResult.OK)
                    {
                        Environment.Exit(1);
                    }
                }
            }
        }

        /// <summary>
        /// Method to get the Singleton instance of SocketHandler
        /// </summary>
        /// <returns></returns>
        public static SocketHandler getInstance()
        {
            if (instance == null)
            {
                instance = new SocketHandler();
            }
            return instance;
        }

        /// <summary>
        /// Method to disconnect the socket
        /// </summary>
        public void Disconnect()
        {
            sender.Shutdown(SocketShutdown.Both);
            sender.Close();
        }

        /// <summary>
        /// Method to send a message with a specific code to the server
        /// </summary>
        /// <param name="msg"></param>
        /// <param name="code"></param>
        /// <returns></returns>
        public int Send(string msg, byte code)
        {
            // Convert message length to bytes
            byte[] byteArray = BitConverter.GetBytes(msg.Length);

            // Create byte array to send, including code and length
            byte[] toSend = new byte[Constants.CODE_AND_SIZE_BYTE_LENGTH + msg.Length];
            toSend[Constants.CODE_BYTE_POSITION] = code;
            for (int i = 0; i < byteArray.Length; i++)
            {
                toSend[i + 1] = byteArray[i];
            }

            // Convert message to bytes and add to the array
            byte[] messageSent = Encoding.ASCII.GetBytes(msg);
            for (int i = 0; i < msg.Length; i++)
            {
                toSend[i + Constants.CODE_AND_SIZE_BYTE_LENGTH] = (byte)msg[i];
            }

            // Send the message and return the number of bytes sent
            int byteSent = sender.Send(toSend);
            return byteSent;
        }

        /// <summary>
        /// Method to receive a response from the server
        /// </summary>
        /// <returns></returns>
        public ResponseInfo Receive()
        {
            byte[] codes = new byte[Constants.CODE_AND_SIZE_BYTE_LENGTH]; /// Buffer for received data
            sender.Receive(codes);

            // Extract the length of the message
            byte[] len = new byte[Constants.MESSAGE_SIZE_BYTE_LENGTH];
            for (int i = 0; i < Constants.MESSAGE_SIZE_BYTE_LENGTH; i++)
            {
                len[i] = codes[i];
            }
            int length = BitConverter.ToInt32(len, 0);

            // Receive the message
            byte[] msg = new byte[length];
            int bytes = sender.Receive(msg);
            string message = Encoding.ASCII.GetString(msg, 0, bytes);

            // Return the received message and code as a ResponseInfo object
            return new ResponseInfo(message, (CODES)codes[Constants.CODE_BYTE_POSITION]);
        }
    }
}
