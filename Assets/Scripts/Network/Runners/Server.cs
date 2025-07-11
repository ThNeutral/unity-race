using System;
using System.Collections.Generic;
using UnityEngine;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using Network.Internals;
using Network.Internals.Message;

namespace Network.Runners
{
    public class Server
    {
        private readonly Internals.TCP.Listener tcpListener;
        private readonly List<Internals.TCP.Client> tcpClients;
        private readonly List<string> connections;

        private readonly Internals.UDP.Client udpClient;

        private readonly int maximumNumberOfClients = 3;

        public Server(int port, int maximumNumberOfClients = 3)
        {
            this.maximumNumberOfClients = maximumNumberOfClients;
            tcpListener = new(port);
            tcpClients = new(maximumNumberOfClients);
            connections = new(maximumNumberOfClients);
            udpClient = new(0);
            sockets = new(2 + maximumNumberOfClients);
        }

        private readonly List<Socket> sockets;
        private List<Socket> GetAvailableSockets()
        {
            sockets.Clear();
            sockets.Add(tcpListener.Socket);
            foreach (var client in tcpClients) sockets.Add(client.Socket);
            sockets.Add(udpClient.Socket);
            return sockets;
        }

        public void Run()
        {
            Debug.Log($"Listening on: TCP {tcpListener.LocalEndPoint}, UDP {udpClient.LocalEndPoint}");
            try
            {
                while (true)
                {
                    var socks = GetAvailableSockets();
                    Socket.Select(socks, null, null, -1);
                    foreach (var sock in socks)
                    {
                        if (sock == tcpListener.Socket)
                        {
                            var client = tcpListener.Accept();

                            if (tcpClients.Count >= maximumNumberOfClients)
                            {
                                var serverReject = MessageFactory.ServerReject();
                                client.Send(serverReject.Serialize());
                                Debug.Log($"Rejected {client.RemoteEndPoint}");
                                client.Close();
                            }
                            else
                            {
                                var port = (ushort)udpClient.LocalEndPoint.Port;
                                var serverAccept = MessageFactory.ServerAccept(port);
                                var result = client.Send(serverAccept.Serialize());
                                if (result.Error == SocketError.Success)
                                {
                                    tcpClients.Add(client);
                                    Debug.Log($"Accepted {client.RemoteEndPoint}");
                                }
                            }

                            continue;
                        }

                        if (sock == udpClient.Socket)
                        {
                            var result = udpClient.ReceiveFrom(out var bytes, out var endPoint);
                            if (result.Error != SocketError.Success)
                                Debug.Log($"Failed to receive bytes from {endPoint}: {result.Error}");
                            else
                                Debug.Log($"Received bytes from {endPoint}: {BitConverter.ToString(bytes)}");
                            continue;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.Log($"Server.Run failed: {ex.Message}");
            }
        }
    }
}