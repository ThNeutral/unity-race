using System;
using System.Collections.Generic;
using System.Net;
using Network.Internal.Message;
using Network.Internal.Sock;
using UnityEngine;

namespace Network.Handlers
{
    public class Server : IDisposable
    {
        private readonly UDPSocket socket;
        public bool Blocking => socket.Blocking;

        private readonly Dictionary<string, bool> connections;
        private readonly int maximumConnectionSize;

        public Server(int port, int maximumConnectionSize = 3)
        {
            socket = new(port);
            connections = new(maximumConnectionSize);
            this.maximumConnectionSize = maximumConnectionSize;
        }

        public void Listen()
        {
            if (!Blocking)
                throw new Exception("non blocking listener is not supported");

            while (true)
            {
                var result = socket.Receive(out var message, out var endPoint);
                if (result.IsDisconnect())
                {
                    connections.Remove(endPoint.ToString());
                    continue;
                }

                if (result.IsError())
                {
                    Debug.Log($"Receive from {endPoint} failed with code {result.error}");
                    continue;
                }

                var messageType = message.Type();
                switch (messageType)
                {
                    case MessageType.ClientHello:
                        HandleClientHello(message, endPoint);
                        break;

                    case MessageType.ServerHello:
                        Debug.Log($"Unexpected message type received on server: {messageType}");
                        break;

                    default:
                        Debug.Log($"Invalid message type: {messageType}");
                        break;
                }
            }
        }

        private void HandleClientHello(IMessage message, EndPoint endPoint)
        {
            if (connections.Count >= maximumConnectionSize)
            {
                // Reject connection 
                return;
            }

            var resp = new { acknowledged = true };
            var respMessage = MessageFactory.FromObject(resp);
            var ipStr = endPoint.ToString();
            if (connections.ContainsKey(ipStr))
            {
                // If key already is in connections, 
                // then it is a sign that we have received ClientHello 
                // again, probably due to UDP unreliability   
                socket.Send(respMessage, endPoint);
                return;
            }

            var res = socket.Send(respMessage, endPoint);
            if (res.IsSuccess()) connections.Add(ipStr, true);
        }

        private bool disposed = false;
        public void Dispose()
        {
            if (!disposed)
            {
                socket.Dispose();
                disposed = true;
                GC.SuppressFinalize(this);
            }
        }
    }
}