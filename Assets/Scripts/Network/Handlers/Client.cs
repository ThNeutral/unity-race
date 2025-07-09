using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;
using Network.Handlers.Shared;
using Network.Internal.Message;
using Network.Internal.Sock;
using UnityEngine;
using Utilities;

namespace Network.Handlers
{
    public class Client : IDisposable
    {
        private readonly UDPSocket socket;
        private readonly EventLoop eventLoop;

        public Client(int port)
        {
            socket = new(port);
            eventLoop = new(socket);
        }

        public void Connect(EndPoint endPoint)
        {
            var helloMessage = MessageFactory.ClientHello();

            var maxRetries = 3;
            IMessage respMessage = null;
            EndPoint respEndpoint = null;
            bool action()
            {
                var res = socket.Send(helloMessage, endPoint);

                res = socket.Receive(out respMessage, out respEndpoint);
                if (res.IsError()) return false;

                var @type = respMessage.Type();
                if (@type != MessageType.ServerHello)
                {
                    Debug.Log("Unexpected message type");
                    return false;
                }

                return true;
            }
            if (!new Runner(action).WithRetry(maxRetries).WithTimeout(2000).Run())
            {
                Debug.Log($"Failed to connect to {endPoint} after {maxRetries} retries");
                return;
            }

            Debug.Log($"Successfully connected to {endPoint}");
            Listen();
        }

        private void Listen()
        {
            eventLoop.Run();
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