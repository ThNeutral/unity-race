using System.Net;
using System.Net.Sockets;
using Network.Internal.Message;

namespace Network.Internal.Sock
{
    public interface ISender
    {
        public SocketResult Send(IMessage message, EndPoint endPoint);
    }
}