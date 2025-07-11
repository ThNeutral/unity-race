using System.Net.Sockets;

namespace Network.Internals.Helpers
{
    public readonly struct SocketResult
    {
        private readonly int count;
        public readonly int Count { get => count; }

        private readonly SocketError error;
        public readonly SocketError Error { get => error; }

        public SocketResult(int count, SocketError error)
        {
            this.count = count;
            this.error = error;
        }
    }
}