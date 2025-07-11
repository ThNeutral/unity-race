using System;
using System.Net;
using System.Net.Sockets;
using Network.Internals.Helpers;

namespace Network.Internals.TCP
{
    public class Client : IDisposable
    {
        public readonly int MaximumPacketSize = 1200;
        public Socket Socket { get => socket; }


        private readonly Socket socket;
        public IPEndPoint LocalEndPoint { get => (IPEndPoint)socket.LocalEndPoint; }
        public IPEndPoint RemoteEndPoint { get => (IPEndPoint)socket.RemoteEndPoint; }

        public Client(Socket socket)
        {
            this.socket = socket;
        }

        public Client(int port)
        {
            socket = new(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            var endPoint = new IPEndPoint(IPAddress.Any, port);
            socket.Bind(endPoint);
        }

        public SocketResult Connect(EndPoint endPoint)
        {
            try
            {
                socket.Connect(endPoint);
                return new SocketResult(1, SocketError.Success);
            }
            catch (SocketException ex)
            {
                return new SocketResult(-1, ex.SocketErrorCode);
            }
        }

        public SocketResult Receive(out byte[] bytes)
        {
            try
            {
                byte[] buffer = new byte[MaximumPacketSize];
                int received = socket.Receive(buffer);

                bytes = buffer[..received];
                return new SocketResult(received, SocketError.Success);
            }
            catch (SocketException ex)
            {
                bytes = null;
                return new SocketResult(-1, ex.SocketErrorCode);
            }
        }

        public SocketResult Send(byte[] bytes)
        {
            try
            {
                int sent = socket.Send(bytes);
                return new SocketResult(sent, SocketError.Success);
            }
            catch (SocketException ex)
            {
                return new SocketResult(-1, ex.SocketErrorCode);
            }
        }

        public void Close()
        {
            socket.Close();
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