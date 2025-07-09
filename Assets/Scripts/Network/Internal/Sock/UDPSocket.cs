using System;
using System.Net;
using System.Net.Sockets;
using Network.Internal.Message;

namespace Network.Internal.Sock
{
    public class UDPSocket : IDisposable
    {
        public static int MaximumDatagramSize = 1200;

        private readonly Socket socket;
        public bool Blocking => socket.Blocking;

        public UDPSocket(int port)
        {
            socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

            var endpoint = new IPEndPoint(IPAddress.Any, port);
            socket.Bind(endpoint);
        }

        public SocketResult Receive(out IMessage message, out EndPoint endpoint)
        {
            var bytes = new byte[MaximumDatagramSize];
            endpoint = null;
            message = null;

            try
            {
                var receivedCount = socket.ReceiveFrom(bytes, ref endpoint);
                message = MessageFactory.FromBytes(bytes[0..receivedCount]);
                return SocketResult.Success(receivedCount);
            }
            catch (SocketException ex)
            {
                return SocketResult.Error(ex.SocketErrorCode);
            }
        }

        public SocketResult Send(IMessage message, EndPoint endPoint)
        {
            try
            {
                var sendCount = socket.SendTo(message.Serialize(), endPoint);
                return SocketResult.Success(sendCount);
            }
            catch (SocketException ex)
            {
                return SocketResult.Error(ex.SocketErrorCode);
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