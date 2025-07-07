using System.Net.Sockets;

namespace Network.Internal.Sock
{
    public class SocketResult
    {
        public static SocketResult Success(int count)
        {
            return new(count, SocketError.Success);
        }

        public static SocketResult Error(SocketError error)
        {
            return new(-1, error);
        }

        public SocketError error;
        public int count;

        public SocketResult(int count, SocketError error)
        {
            this.count = count;
            this.error = error;
        }

        public bool IsSuccess()
        {
            return (count > 0) && (error == SocketError.Success);
        }

        public bool ShouldRetry()
        {
            return error == SocketError.TryAgain;
        }

        public bool IsDisconnect()
        {
            return count == 0;
        }

        public bool IsError()
        {
            return !IsSuccess() && !IsDisconnect();
        }
    }
}