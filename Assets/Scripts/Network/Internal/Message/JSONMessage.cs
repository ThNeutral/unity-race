using System.Collections.Generic;
using System.Text;
using Newtonsoft.Json;

namespace Network.Internal.Message
{
    public class JSONMessage : IMessage
    {
        public static JSONMessage New(object payload)
        {
            var str = JsonConvert.SerializeObject(payload);
            return new JSONMessage(str);
        }

        private readonly byte[] serialized;
        private readonly Dictionary<string, object> map;
        public JSONMessage(string payload)
        {
            serialized = Encoding.ASCII.GetBytes(payload);
            map = JsonConvert.DeserializeObject<Dictionary<string, object>>(payload);
        }

        public bool TryRead<T>(string key, out T value)
        {
            var present = map.TryGetValue(key, out var val);
            if (!present || val is not T)
            {
                value = default;
                return false;
            }

            value = (T)val;
            return true;
        }

        public T Read<T>(string key)
        {
            var ok = TryRead(key, out T val);
            if (!ok) throw new System.Exception($"key {key} was not present in map");
            return val;
        }

        public MessageType Type()
        {
            return (MessageType)Read<byte>("t");
        }

        public byte[] Serialize()
        {
            return (byte[])serialized.Clone();
        }
    }
}