using System;
using System.Text.Json;

namespace Trivia_client.CommunicationHandling
{
    /// <summary>
    /// Internal class for serializing request structures into JSON strings
    /// </summary>
    internal class JsonPacketSerializer
    {
        /// <summary>
        /// Generic method to serialize any struct into a JSON string
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="request"></param>
        /// <returns></returns>
        public static string Serialize<T>(T request) where T : struct
        {
            return JsonSerializer.Serialize(request);
        }

        /// <summary>
        /// Method to serialize a LoginRequest object into a JSON string
        /// </summary>
        /// <param name="request"></param>
        /// <returns></returns>
        public static string SerializeLoginRequest(LoginRequest request)
        {
            return JsonSerializer.Serialize(request);
        }

        /// <summary>
        /// Method to serialize a SignupRequest object into a JSON string
        /// </summary>
        /// <param name="request"></param>
        /// <returns></returns>
        public static string SerializeSignupRequest(SignupRequest request)
        {
            return JsonSerializer.Serialize(request);
        }

        /// <summary>
        /// Method to serialize a GetPlayersInRoomRequest object into a JSON string
        /// </summary>
        /// <param name="request"></param>
        /// <returns></returns>
        public static string SerializeGetPlayersRequest(GetPlayersInRoomRequest request)
        {
            return JsonSerializer.Serialize(request);
        }

        /// <summary>
        /// Method to serialize a JoinRoomRequest object into a JSON string
        /// </summary>
        /// <param name="request"></param>
        /// <returns></returns>
        public static string SerializeJoinRoomRequest(JoinRoomRequest request)
        {
            return JsonSerializer.Serialize(request);
        }

        /// <summary>
        /// Method to serialize a CreateRoomRequest object into a JSON string
        /// </summary>
        /// <param name="request"></param>
        /// <returns></returns>
        public static string SerializeCreateRoomRequest(CreateRoomRequest request)
        {
            return JsonSerializer.Serialize(request);
        }

        /// <summary>
        /// Method to serialize a SubmitAnswerRequest object into a JSON string
        /// </summary>
        /// <param name="request"></param>
        /// <returns></returns>
        public static string SerializeSubmitAnswerRequest(SubmitAnswerRequest request)
        {
            return JsonSerializer.Serialize(request);
        }
    }
}
