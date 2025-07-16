using System;
using System.Collections.Generic;
using UnityEngine;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using Network.Internals;
using Message;
using State;
using State.Enums;
using Store.Enums;
using State.DTO;

namespace Network.Runners
{
    public class Server
    {
        private readonly Internals.TCP.Listener tcpListener;
        private readonly List<Internals.TCP.Client> tcpClients;
        private readonly List<string> connections;

        private readonly Internals.UDP.Client udpClient;

        private readonly int maximumNumberOfClients = 3;

        private StateController stateController;

        public Server(int port, int maximumNumberOfClients = 3)
        {
            this.maximumNumberOfClients = maximumNumberOfClients;
            tcpListener = new(port);
            tcpClients = new(maximumNumberOfClients);
            connections = new(maximumNumberOfClients);
            udpClient = new(0);
            sockets = new(2 + maximumNumberOfClients);

            stateController = StateController.Instance;
        }

        private readonly List<Socket> sockets;
        private void UpdateSockets()
        {
            sockets.Clear();
            sockets.Add(tcpListener.Socket);
            foreach (var client in tcpClients) sockets.Add(client.Socket);
            sockets.Add(udpClient.Socket);
        }

        public void Run()
        {
            try
            {
                var hostPlayerCar = SyncData.CreateCarDefault();
                stateController.Schedule(hostPlayerCar);
                Debug.Log($"Listening on: TCP {tcpListener.LocalEndPoint}, UDP {udpClient.LocalEndPoint}");
                while (true)
                {
                    UpdateSockets();
                    Socket.Select(sockets, null, null, -1);
                    foreach (var sock in sockets)
                    {
                        if (sock == tcpListener.Socket)
                        {
                            HandleInitialConnect();
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

        private void HandleInitialConnect()
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
                    var newClientCar = SyncData.CreateCarDefault();
                    stateController.Schedule(newClientCar);
                    Debug.Log($"Accepted {client.RemoteEndPoint}");
                }

                var syncDatas = stateController.GetInitialSyncData();
                var serverInitialSyncState = MessageFactory.ServerInitialSyncState(syncDatas);
                result = client.Send(serverInitialSyncState.Serialize());
                if (result.Error != SocketError.Success)
                    throw new Exception($"failed to send bytes: {result.Error}");
            }
        }
    }
}