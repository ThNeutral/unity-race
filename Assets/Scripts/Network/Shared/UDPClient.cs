using System.Net;
using System.Net.Sockets;

namespace Network.Shared
{
    public class UDPClient
    {
        public static readonly int MaximumDatagramSize = 1200;

        private readonly Socket socket;
        public IPEndPoint LocalEndPoint => (IPEndPoint)socket.LocalEndPoint;

        public UDPClient(int port)
        {
            socket = new(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            var endpoint = new IPEndPoint(IPAddress.Any, port);
            socket.Bind(endpoint);
            socket.Blocking = false;
        }

        public SocketError ReceiveFrom(out byte[] bytes, out EndPoint endPoint)
        {
            bytes = new byte[MaximumDatagramSize];
            endPoint = new IPEndPoint(IPAddress.Any, 0);
            try
            {
                var count = socket.ReceiveFrom(bytes, ref endPoint);
                bytes = bytes[..count];
                return SocketError.Success;
            }
            catch (SocketException ex)
            {
                return ex.SocketErrorCode;
            }
        }

        public SocketError SendTo(byte[] bytes, EndPoint endPoint)
        {
            try
            {
                socket.SendTo(bytes, endPoint);
                return SocketError.Success;
            }
            catch (SocketException ex)
            {
                return ex.SocketErrorCode;
            }
        }
    }
}