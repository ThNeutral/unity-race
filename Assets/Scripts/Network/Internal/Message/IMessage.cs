namespace Network.Internal.Message
{
    public interface IMessage
    {
        public byte[] Serialize();
        public bool TryRead<T>(string key, out T value);
        public T Read<T>(string key);
        public MessageType Type();
    }
}