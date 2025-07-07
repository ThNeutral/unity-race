using System;
using System.Text;

namespace Network.Internal.Message
{
    public static class MessageFactory
    {
        public static readonly MessageFormat selectedMessageFormat = MessageFormat.JSON;

        public static IMessage FromObject(object payload)
        {
            switch (selectedMessageFormat)
            {
                case MessageFormat.JSON:
                    {
                        return JSONMessage.New(payload);
                    }
                default:
                    {
                        throw new Exception($"invalid format {selectedMessageFormat}");
                    }
            }
        }

        public static IMessage FromBytes(byte[] bytes)
        {
            // TODO: proper error handling

            if (bytes.Length < 1)
                throw new Exception("empty bytes");
            var @type = bytes[0];
            var format = (MessageFormat)@type;

            switch (format)
            {
                case MessageFormat.JSON:
                    {
                        return new JSONMessage(Encoding.ASCII.GetString(bytes[1..]));
                    }
                default:
                    {
                        throw new Exception($"invalid format {format}");
                    }
            }
        }
    } 
}