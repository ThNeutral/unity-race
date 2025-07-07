using System;
using System.Net;
using System.Threading;
using System.Threading.Tasks;
using Network.Internal.Message;
using Network.Internal.Sock;
using UnityEngine;

namespace Network.Handlers
{
    public class Client : IDisposable
    {
        private readonly UDPSocket socket;

        public Client(int port)
        {
            socket = new(port);
        }

        public async Task Connect(EndPoint endPoint)
        {
            var helloMessage = MessageFactory.FromObject(new { hello = true });

            var retries = 0;
            while (retries < 3)
            {
                var result = socket.Send(helloMessage, endPoint);
                if (!result.IsSuccess()) throw new Exception($"connection failed with code {result.error}");

                var token = new CancellationTokenSource(TimeSpan.FromSeconds(2)).Token;

                IMessage respMessage = null;
                EndPoint respEndpoint = null;
                var listenTask = Task.Run(() => socket.Receive(out respMessage, out respEndpoint));
                var timeoutTask = Task.Delay(Timeout.Infinite, token);
                var completedTask = await Task.WhenAny(listenTask, timeoutTask);
                if (completedTask == listenTask)
                {
                    Debug.Log($"Successfully connected to {respEndpoint}");
                    break;
                }
                else
                {
                    retries += 1;
                    Debug.Log($"Connection attempt {retries} had failed");    
                }
            }
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