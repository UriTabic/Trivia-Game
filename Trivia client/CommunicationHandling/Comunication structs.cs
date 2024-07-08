using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace Trivia_client.CommunicationHandling
{
    /// <summary>
    /// Enumeration for different types of communication codes
    /// </summary>
    public enum CODES
    {
        LOGIN_REQUEST = 0, LOGIN_RESPONSE,
        SIGNUP_REQUEST, SIGNUP_RESPONSE,
        ERROR_RESPONSE,
        LOGOUT_REQUEST, LOGOUT_RESPONSE,
        GET_PLAYERS_IN_ROOM_REQUEST, GET_PLAYERS_IN_ROOM_RESPONSE,
        JOIN_ROOM_REQUEST, JOIN_ROOM_RESPONSE,
        CREATE_ROOM_REQUEST, CREATE_ROOM_RESPONSE,
        GET_ROOMS_REQUEST, GET_ROOMS_RESPONSE,
        GET_HIGH_SCORE_REQUEST, GET_HIGH_SCORE_RESPONSE,
        GET_PERSONAL_STATS_REQUEST, GET_PERSONAL_STATS_RESPONSE,
        CLOSE_ROOM_REQUEST, CLOSE_ROOM_RESPONSE,
        START_GAME_REQUEST, START_GAME_RESPONSE,
        GET_ROOM_STATE_REQUEST, GET_ROOM_STATE_RESPONSE,
        LEAVE_ROOM_REQUEST, LEAVE_ROOM_RESPONSE,
        SUBMIT_ANSWER_REQUEST, SUBMIT_ANSWER_RESPONSE,
        GET_QUESTION_REQUEST, GET_QUESTION_RESPONSE,
        GET_GAME_RESULTS_REQUEST, GET_GAME_RESULTS_RESPONSE,
        LEAVE_GAME_REQUEST, LEAVE_GAME_RESPONSE
    }

    /// <summary>
    /// Structure representing a login request
    /// </summary>
    public struct LoginRequest
    {
        public string username { get; set; }
        public string password { get; set; }
    }

    /// <summary>
    /// Structure representing a signup request
    /// </summary>
    public struct SignupRequest
    {
        public string username { get; set; }
        public string password { get; set; }
        public string email { get; set; }
    }

    /// <summary>
    /// Structure for storing response information
    /// </summary>
    public struct ResponseInfo
    {
        public CODES Code { get; private set; }
        public DateTime ReceivalTime { get; private set; }
        public string Data { get; private set; }

        /// <summary>
        /// Constructor initializing response info
        /// </summary>
        /// <param name="msg"></param>
        /// <param name="code"></param>
        public ResponseInfo(string msg, CODES code)
        {
            this.Code = code;
            this.Data = msg;
            this.ReceivalTime = DateTime.UtcNow;
        }

        /// <summary>
        /// Override ToString method to format the response info
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.AppendLine($"Code: {(int)Code}");
            sb.AppendLine($"Receival time: {ReceivalTime}");
            sb.AppendLine($"Data: {Data}");
            return sb.ToString();
        }
    }

    /// <summary>
    /// Structure representing a login response
    /// </summary>
    public struct LoginResponse
    {
        public uint status { get; set; }
    }

    /// <summary>
    /// Structure representing a signup response
    /// </summary>
    public struct SignupResponse
    {
        public uint status { get; set; }
    }

    /// <summary>
    /// Structure representing a logout response
    /// </summary>
    public struct LogoutResponse
    {
        public uint status { get; set; }
    }

    /// <summary>
    /// Structure representing a response for getting rooms
    /// </summary>
    public struct GetRoomsResponse
    {
        public uint status { get; set; }
        public List<RoomData> Rooms { get; set; }
    }

    /// <summary>
    /// Enumeration for different states of a room
    /// </summary>
    enum RoomState { OPENED = 0, CLOSED, STARTED };

    /// <summary>
    /// Structure representing data about a room
    /// </summary>
    public struct RoomData
    {
        public int id { get; set; }
        public int state { get; set; }
        public int maxPlayers { get; set; }
        public string name { get; set; }
        public int numOfQuestionsInGame { get; set; }
        public int timePerQuestion { get; set; }
    }

    /// <summary>
    /// Structure representing a request to get players in a room
    /// </summary>
    public struct GetPlayersInRoomRequest
    {
        public int roomId { get; set; }
    }

    /// <summary>
    /// Structure representing a response for getting players in a room
    /// </summary>
    public struct GetPlayersInRoomResponse
    {
        public uint status { get; set; }
        public List<string> players { get; set; }
    }

    /// <summary>
    /// Structure representing a response for getting high scores
    /// </summary>
    public struct GetHighScoreResponse
    {
        public uint status { get; set; }
        public List<string> statistics { get; set; }
    }

    /// <summary>
    /// Structure representing a response for getting personal statistics
    /// </summary>
    public struct GetPersonalStatsResponse
    {
        public uint status { get; set; }
        public List<string> statistics { get; set; }
    }

    /// <summary>
    /// Structure representing a request to join a room
    /// </summary>
    public struct JoinRoomRequest
    {
        public int roomId { get; set; }
    }

    /// <summary>
    /// Structure representing a response for joining a room
    /// </summary>
    public struct JoinRoomResponse
    {
        public uint status { get; set; }
    }

    /// <summary>
    /// Structure representing a request to create a room
    /// </summary>
    public struct CreateRoomRequest
    {
        public string roomName { get; set; }
        public uint maxUsers { get; set; }
        public uint questionCount { get; set; }
        public uint answerTimeout { get; set; }
    }

    /// <summary>
    /// Structure representing a response for creating a room
    /// </summary>
    public struct CreateRoomResponse
    {
        public uint status { get; set; }
        public uint roomId { get; set; }
    }

    /// <summary>
    /// Structure representing a response for closing a room
    /// </summary>
    struct CloseRoomResponse
    {
        public uint status { get; set; }
    }

    /// <summary>
    /// Structure representing a response for starting a game
    /// </summary>
    struct StartGameResponse
    {
        public uint status { get; set; }
    }

    /// <summary>
    /// Structure representing a response for leaving a room
    /// </summary>
    struct LeaveRoomResponse
    {
        public uint status { get; set; }
    }

    /// <summary>
    /// Structure representing a response for getting the state of a room
    /// </summary>
    struct GetRoomStateResponse
    {
        public uint status { get; set; }
        public uint questionCount { get; set; }
        public uint answerTimeout { get; set; }
        public uint state { get; set; }
        public List<string> players { get; set; }
    }

    /// <summary>
    /// Structure representing a response for leaving a game
    /// </summary>
    public struct LeaveGameResponse
    {
        public uint status { get; set; }
    }

    /// <summary>
    /// Structure representing a response for getting a question
    /// </summary>
    public struct GetQuestionResponse
    {
        public Dictionary<uint, string> answers { get; set; }
        public string question { get; set; }
        public uint status { get; set; }
    }

    /// <summary>
    /// Structure representing a request to submit an answer
    /// </summary>
    struct SubmitAnswerRequest
    {
        public int answerId { get; set; }
    }

    /// <summary>
    /// Structure representing a response for submitting an answer
    /// </summary>
    public struct SubmitAnswerResponse
    {
        public uint correctAnswerId { get; set; }
        public uint status { get; set; }
    }

    /// <summary>
    /// Structure representing player results in a game
    /// </summary>
    public struct PlayerResults
    {
        public string username { get; set; }
        public uint correctAnswerCount { get; set; }
        public uint wrongAnswerCount { get; set; }
        public uint averageAnswerTime { get; set; }
        public uint hasRetired { get; set; }
    }

    /// <summary>
    /// Structure representing a response for getting game results
    /// </summary>
    public struct GetGameResultsResponse
    {
        public uint status { get; set; }
        public List<PlayerResults> results { get; set; }
    }

    /// <summary>
    /// Structure representing an error response
    /// </summary>
    public struct ErrorResponse
    {
        public string Message { get; set; }
    }
}
