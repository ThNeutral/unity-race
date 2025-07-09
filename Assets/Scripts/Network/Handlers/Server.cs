using System;
using System.Collections.Generic;
using System.Net;
using Network.Handlers.Shared;
using Network.Internal.Message;
using Network.Internal.Sock;
using UnityEngine;

namespace Network.Handlers
{
    public class Server : IDisposable
    {
        private readonly UDPSocket socket;
        public bool Blocking => socket.Blocking;

        private readonly EventLoop eventLoop;

        private readonly Dictionary<string, bool> connections;
        private readonly int maximumConnectionSize;

        public Server(int port, int maximumConnectionSize = 3)
        {
            socket = new(port);
            eventLoop = new(socket);
            connections = new(maximumConnectionSize);
            this.maximumConnectionSize = maximumConnectionSize;

            SetupListeners();
        }

        private void SetupListeners()
        {
            eventLoop.On(EventLoop.EventType.Disconnect, HandleDisconnect);
            eventLoop.On(EventLoop.EventType.Error, HandleError);

            eventLoop.On(MessageType.ClientHello, HandleClientHello);
        }

        private void HandleDisconnect(SocketResult result, EndPoint endPoint)
        {
            Debug.Log($"{endPoint} disconnected");    
        }
        
        private void HandleError(SocketResult result, EndPoint endPoint)
        {
            Debug.Log($"{endPoint} error: {result.error}");
        }

        public void Listen()
        {
            eventLoop.Run();
        }

        private void HandleClientHello(IMessage message, EndPoint endPoint)
        {
            if (connections.Count >= maximumConnectionSize)
            {
                Debug.Log($"Rejected {endPoint}: too many connectins");
                // Reject connection 
                return;
            }

            var respMessage = MessageFactory.ServerHello();
            var ipStr = endPoint.ToString();
            Debug.Log($"Accepted {ipStr}: too many connectins");
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