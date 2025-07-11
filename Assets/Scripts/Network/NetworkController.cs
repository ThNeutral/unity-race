using System.Net;
using System.Threading.Tasks;
using Network.Runners;
using UnityEngine;

namespace Network
{
    public class NetworkController : MonoBehaviour
    {
        [SerializeField]
        private bool isServer;

        private object handler;
        public bool IsRunning => handler != null;

        [SerializeField]
        private int port = 9432;

        public void StartServer()
        {
            if (IsRunning) throw new System.Exception("already running handler");
            var server = new Server(port);
            Task.Run(() => server.Run());
            handler = server;
        }
        
        [SerializeField]
        private string serverIP = "127.0.0.1";
        [SerializeField]
        private int serverPort = 9432;

        public void StartClient()
        {
            if (IsRunning) throw new System.Exception("already running handler");
            var endPoint = new IPEndPoint(IPAddress.Parse(serverIP), serverPort);
            var client = new Client();
            Task.Run(() => client.Connect(endPoint));
            handler = client;
        }
    }
}