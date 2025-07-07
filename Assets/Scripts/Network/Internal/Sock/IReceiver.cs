using System.Net;
using System.Net.Sockets;
using Network.Internal.Message;

namespace Network.Internal.Sock
{
    public interface IReceiver
    {
        public SocketResult Receive(out IMessage message, out EndPoint endpoint);
    }
}