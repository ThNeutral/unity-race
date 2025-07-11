using System;
using System.Text;
using Newtonsoft.Json;

using JSONObject = System.Collections.Generic.Dictionary<string, object>;

namespace Network.Internals.Message
{
    public class JSON
    {
        private readonly JSONObject data;
        public JSON(string serialized)
        {
            data = JsonConvert.DeserializeObject<JSONObject>(serialized);
        }

        public JSON(JSONObject obj)
        {
            data = obj;
        }

        public MessageType Type()
        {
            if (!TryRead<byte>(MessageFactory.TypeKey, out var type))
                throw new Exception($"message did not have a type. Payload {JsonConvert.SerializeObject(data)}");

            return (MessageType)type;
        }

        public bool TryRead<T>(string key, out T value)
        {
            try
            {
                var exists = data.TryGetValue(key, out object obj);
                if (!exists)
                {
                    value = default;
                    return false;
                }

                if (obj is T t)
                {
                    value = t;
                    return true;
                }

                value = (T)Convert.ChangeType(obj, typeof(T));
                return true;
            }
            catch
            {
                value = default;
                return false;
            }
        }

        public byte[] Serialize()
        {
            var str = JsonConvert.SerializeObject(data);
            var bytes = Encoding.ASCII.GetBytes(str);
            return bytes;
        }
    }
}