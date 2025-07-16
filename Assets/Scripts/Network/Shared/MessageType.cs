using System;

namespace Network.Shared
{
    public enum MessageType : byte
    {
        ClientHello,
        ServerHello
    }

    public static class MessageTypeExtensions
    {
        public static byte ToByte(this MessageType type)
        {
            return (byte)type;
        }

        public static MessageType ToMessageType(this byte @byte)
        {
            return (MessageType)@byte;
        }

        public static bool IsValid(this MessageType type)
        {
            return Enum.IsDefined(typeof(MessageType), type);
        }
    }
}