using System.Net;
using System.Threading.Tasks;
using Network.Handlers;
using UnityEngine;

namespace Network
{
    public class NetworkController : MonoBehaviour
    {
        [SerializeField]
        private bool isServer;

        private object handler;
        private Task listener;

        [SerializeField]
        private int port;

        public void StartServer()
        {
            if (handler != null) throw new System.Exception("already running handler");

            var server = new Server(port);
            handler = server;
            listener = Task.Run(server.Listen);
        }
        
        [SerializeField]
        private string serverIP;
        [SerializeField]
        private int serverPort;

        public void StartClient()
        {
            if (handler != null) throw new System.Exception("already running handler");
            var endPoint = new IPEndPoint(IPAddress.Parse(serverIP), serverPort);

            var client = new Client(0);
            handler = client;
            listener = Task.Run(() => client.Connect(endPoint));
        }
    }
}