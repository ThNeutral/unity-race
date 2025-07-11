using System;
using System.Net;
using System.Net.Sockets;
using Network.Internals.Helpers;

namespace Network.Internals.UDP
{
    public class Client : IDisposable
    {
        public readonly int MaximumPacketSize = 1200;
        public Socket Socket { get => socket; }


        private readonly Socket socket;
        public IPEndPoint LocalEndPoint { get => (IPEndPoint)socket.LocalEndPoint; }
        
        public Client(int port)
        {
            socket = new(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            var endPoint = new IPEndPoint(IPAddress.Any, port);
            socket.Bind(endPoint);
        }

        public SocketResult ReceiveFrom(out byte[] bytes, out EndPoint endPoint)
        {
            try
            {
                EndPoint ep = new IPEndPoint(IPAddress.Any, 0);
                var buffer = new byte[MaximumPacketSize];
                var received = socket.ReceiveFrom(buffer, ref ep);

                bytes = buffer[..received];
                endPoint = ep;

                return new SocketResult(received, SocketError.Success);
            }
            catch (SocketException ex)
            {
                bytes = null;
                endPoint = null;
                return new SocketResult(-1, ex.SocketErrorCode);
            }
        }

        public SocketResult SendTo(byte[] bytes, EndPoint endPoint)
        {
            try
            {
                var received = socket.SendTo(bytes, endPoint);
                return new SocketResult(received, SocketError.Success);
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