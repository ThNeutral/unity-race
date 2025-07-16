using System.Collections.Generic;
using System.Text;
using State.DTO;
using UnityEngine;

using JSONObject = System.Collections.Generic.Dictionary<string, object>;

namespace Message
{
    public static class MessageFactory
    {
        public static readonly string TypeKey = "typ";

        public static JSON ClientHello()
        {
            var obj = new JSONObject()
            {
                {TypeKey, MessageType.ClientHello }
            };
            return new JSON(obj);
        }

        public static readonly string PortKey = "por";

        public static JSON ServerAccept(ushort udpPort)
        {
            var obj = new JSONObject()
            {
                { TypeKey, MessageType.ServerAccept },
                { PortKey, udpPort },
            };
            return new JSON(obj);
        }

        public static JSON ServerReject()
        {
            var obj = new JSONObject()
            {
                { TypeKey, MessageType.ServerReject },
            };
            return new JSON(obj);
        }

        public static readonly string DatasKey = "dts";

        public static JSON ServerInitialSyncState(List<SyncData> syncDatas)
        {
            var obj = new JSONObject()
            {
                { TypeKey, MessageType.ServerInitialSyncState },
                { DatasKey, syncDatas }
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