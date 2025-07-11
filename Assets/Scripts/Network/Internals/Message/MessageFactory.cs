using System.Collections.Generic;
using System.Text;
using UnityEngine;

namespace Network.Internals.Message
{
    public static class MessageFactory
    {
        public static readonly string TypeKey = "typ";

        public static JSON ClientHello()
        {
            var obj = new Dictionary<string, object>()
            {
                {TypeKey, MessageType.ClientHello }
            };
            return new JSON(obj);
        }

        public static readonly string PortKey = "por";

        public static JSON ServerAccept(ushort udpPort)
        {
            var obj = new Dictionary<string, object>()
            {
                { TypeKey, MessageType.ServerAccept },
                { PortKey, udpPort },
            };
            return new JSON(obj);
        }

        public static JSON ServerReject()
        {
            var obj = new Dictionary<string, object>()
            {
                { TypeKey, MessageType.ServerReject },
            };
            return new JSON(obj);
        }

        public static JSON FromBytes(byte[] bytes)
        {
            var str = Encoding.ASCII.GetString(bytes);
            return new JSON(str);
        }
    } 
}