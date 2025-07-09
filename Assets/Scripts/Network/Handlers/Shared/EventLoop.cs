using System;
using System.Collections.Generic;
using System.Net;
using Network.Internal.Message;
using Network.Internal.Sock;
using UnityEngine;

namespace Network.Handlers.Shared
{
    public class EventLoop
    {
        public enum EventType
        {
            Disconnect,
            Error,
        }

        private readonly UDPSocket socket;
        private readonly Dictionary<MessageType, Action<IMessage, EndPoint>> typeHandlers;
        private readonly Dictionary<EventType, Action<SocketResult, EndPoint>> eventHandlers;

        public EventLoop(UDPSocket socket)
        {
            this.socket = socket;
            typeHandlers = new();
            eventHandlers = new();
        }

        public void On(MessageType @type, Action<IMessage, EndPoint> action)
        {
            var exists = typeHandlers.TryGetValue(@type, out var act);
            if (!exists) typeHandlers.Add(@type, null);
            typeHandlers[@type] += action;
        }

        public void On(EventType @type, Action<SocketResult, EndPoint> action)
        {
            var exists = eventHandlers.TryGetValue(@type, out var act);
            if (!exists) eventHandlers.Add(@type, null);
            eventHandlers[@type] += action;
        }

        public void Run()
        {
            if (!socket.Blocking)
                throw new Exception("non blocking loop is not supported");

            while (true)
            {
                var result = socket.Receive(out var message, out var endPoint);
                Debug.Log($"Received message from {endPoint}");
                if (result.IsDisconnect())
                {
                    if (eventHandlers.TryGetValue(EventType.Disconnect, out var action))
                        action?.Invoke(result, endPoint);
                    continue;
                }

                if (result.IsError())
                {
                    if (eventHandlers.TryGetValue(EventType.Error, out var action))
                        action?.Invoke(result, endPoint);
                    continue;
                }

                var @type = message.Type();
                if (typeHandlers.TryGetValue(type, out var handler))
                    handler?.Invoke(message, endPoint);
                else if (Enum.IsDefined(typeof(MessageType), @type))
                    Debug.LogWarning($"Message type {type} is missing handler");
                else
                    Debug.LogWarning($"Invalid message type {type}");
            }
        }
    }
}