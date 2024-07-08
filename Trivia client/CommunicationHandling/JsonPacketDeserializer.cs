using System;
using System.Text.Json;

namespace Trivia_client.CommunicationHandling
{
    /// <summary>
    /// Internal class for deserializing JSON packets into their respective response structures
    /// </summary>
    internal class JsonPacketDeserializer
    {
        /// <summary>
        /// Method to deserialize JSON data into a LoginResponse object
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static LoginResponse DeserializeLoginResponse(string data)
        {
            return JsonSerializer.Deserialize<LoginResponse>(data);
        }

        /// <summary>
        /// Method to deserialize JSON data into a SignupResponse object
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static SignupResponse DeserializeSignupResponse(string data)
        {
            return JsonSerializer.Deserialize<SignupResponse>(data);
        }

        /// <summary>
        /// Method to create an ErrorResponse object from the given data string
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static ErrorResponse DeserializeErrorResponse(string data)
        {
            return new ErrorResponse { Message = data };
        }

        /// <summary>
        /// Method to deserialize JSON data into a LogoutResponse object
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static LogoutResponse DeserializeLogoutResponse(string data)
        {
            return JsonSerializer.Deserialize<LogoutResponse>(data);
        }

        /// <summary>
        /// Method to deserialize JSON data into a GetRoomsResponse object
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static GetRoomsResponse DeserializeGetRoomsResponse(string data)
        {
            return JsonSerializer.Deserialize<GetRoomsResponse>(data);
        }

        /// <summary>
        /// Method to deserialize JSON data into a GetPlayersInRoomResponse object
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static GetPlayersInRoomResponse DeserializeGetPlayersInRoomResponse(string data)
        {
            return JsonSerializer.Deserialize<GetPlayersInRoomResponse>(data);
        }

        /// <summary>
        /// Method to deserialize JSON data into a JoinRoomResponse object
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static JoinRoomResponse DeserializeJoinRoomResponse(string data)
        {
            return JsonSerializer.Deserialize<JoinRoomResponse>(data);
        }

        /// <summary>
        /// Method to deserialize JSON data into a CreateRoomResponse object
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static CreateRoomResponse DeserializeCreateRoomResponse(string data)
        {
            return JsonSerializer.Deserialize<CreateRoomResponse>(data);
        }

        /// <summary>
        /// Method to deserialize JSON data into a GetHighScoreResponse object
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static GetHighScoreResponse DeserializeHighScoreResponse(string data)
        {
            return JsonSerializer.Deserialize<GetHighScoreResponse>(data);
        }

        /// <summary>
        /// Method to deserialize JSON data into a GetPersonalStatsResponse object
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static GetPersonalStatsResponse DeserializeGetPersonalStatsResponse(string data)
        {
            return JsonSerializer.Deserialize<GetPersonalStatsResponse>(data);
        }

        /// <summary>
        /// Method to deserialize JSON data into a CloseRoomResponse object
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static CloseRoomResponse DeserializeCloseRoomResponse(string data)
        {
            return JsonSerializer.Deserialize<CloseRoomResponse>(data);
        }

        /// <summary>
        /// Method to deserialize JSON data into a StartGameResponse object
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static StartGameResponse DeserializeStartGameResponse(string data)
        {
            return JsonSerializer.Deserialize<StartGameResponse>(data);
        }

        /// <summary>
        /// Method to deserialize JSON data into a LeaveRoomResponse object
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static LeaveRoomResponse DeserializeLeaveRoomResponse(string data)
        {
            return JsonSerializer.Deserialize<LeaveRoomResponse>(data);
        }

        /// <summary>
        /// Method to deserialize JSON data into a GetRoomStateResponse object
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static GetRoomStateResponse DeserializeGetRoomStateResponse(string data)
        {
            return JsonSerializer.Deserialize<GetRoomStateResponse>(data);
        }

        /// <summary>
        /// Method to deserialize JSON data into a GetGameResultsResponse object
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static GetGameResultsResponse DeserializeGetGameResultsResponse(string data)
        {
            return JsonSerializer.Deserialize<GetGameResultsResponse>(data);
        }

        /// <summary>
        /// Method to deserialize JSON data into a SubmitAnswerResponse object
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static SubmitAnswerResponse DeserializeSubmitAnswerResponse(string data)
        {
            return JsonSerializer.Deserialize<SubmitAnswerResponse>(data);
        }

        /// <summary>
        /// Method to deserialize JSON data into a GetQuestionResponse object
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static GetQuestionResponse DeserializeGetQuestionResponse(string data)
        {
            return JsonSerializer.Deserialize<GetQuestionResponse>(data);
        }

        /// <summary>
        /// Method to deserialize JSON data into a LeaveGameResponse object
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static LeaveGameResponse DeserializeLeaveGameResponse(string data)
        {
            return JsonSerializer.Deserialize<LeaveGameResponse>(data);
        }
    }
}
