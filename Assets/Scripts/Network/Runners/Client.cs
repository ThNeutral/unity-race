using UnityEngine;
using System.Net;
using System.Net.Sockets;
using Message;
using System;
using System.Text;
using System.Collections.Generic;
using State;
using State.DTO;

namespace Network.Runners
{
    public class Client
    {
        private readonly Internals.TCP.Client tcpClient;
        private readonly Internals.UDP.Client udpClient;

        private readonly List<Socket> sockets;
        private void UpdateSockets()
        {
            sockets.Clear();
            sockets.Add(tcpClient.Socket);
            sockets.Add(udpClient.Socket);
        }

        private StateController stateController;

        public Client()
        {
            tcpClient = new(0);
            udpClient = new(0);
            sockets = new(2);
            stateController = StateController.Instance;
        }

        private EndPoint serverTCPEndPoint;
        private EndPoint serverUDPEndPoint;

        public void Connect(IPEndPoint endPoint)
        {
            try
            {
                serverTCPEndPoint = endPoint;
                var result = tcpClient.Connect(endPoint);
                if (result.Error != SocketError.Success)
                {
                    Debug.Log($"Failed to connect to {endPoint}: {result.Error}");
                    return;
                }
                Debug.Log($"Successfully connected to {endPoint}");

                result = tcpClient.Receive(out var bytes);
                if (result.Error != SocketError.Success)
                {
                    Debug.Log($"Failed to receive initial message from {endPoint}: {result.Error}");
                    return;
                }

                var message = MessageFactory.FromBytes(bytes);
                var @type = message.Type();

                if (@type == MessageType.ServerReject)
                {
                    Debug.Log("Server rejected connection");
                    return;
                }

                if (@type == MessageType.ServerAccept)
                {
                    if (!message.TryRead<ushort>(MessageFactory.PortKey, out var port))
                    {
                        Debug.Log($"{MessageType.ServerAccept} message has no '{MessageFactory.PortKey}' key");
                        return;
                    }

                    serverUDPEndPoint = new IPEndPoint(endPoint.Address, port);
                    Run();
                    return;
                }

                Debug.Log($"Unexpected message type {@type}");
                return;
            }
            catch (Exception ex)
            {
                Debug.Log($"Client.Connect failed: {ex}");
            }
        }

        private void Run()
        {
            try
            {
                while (true)
                {
                    UpdateSockets();
                    Socket.Select(sockets, null, null, -1);
                    foreach (var sock in sockets)
                    {
                        if (sock == tcpClient.Socket)
                        {
                            var result = tcpClient.Receive(out var bytes);
                            if (result.Error != SocketError.Success)
                            {
                                Debug.Log($"Failed to receive TCP bytes from {tcpClient.LocalEndPoint}: {result.Error}");
                                continue;
                            }

                            var message = MessageFactory.FromBytes(bytes);
                            var type = message.Type();
                            switch (type)
                            {
                                case MessageType.ServerInitialSyncState:
                                    HandleServerInitialSyncState(message);
                                    break;
                                default:
                                    Debug.Log($"Invalid message type from {tcpClient.LocalEndPoint}: {type}");
                                    break;
                            }
                        }
                        else if (sock == udpClient.Socket) { }
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.Log($"Client.Run had failed: {ex.Message}");
            }
        }

        private void HandleServerInitialSyncState(JSON message)
        {
            if (!message.TryRead<List<SyncData>>(MessageFactory.DatasKey, out var datas))
                throw new Exception($"invalid {MessageType.ServerInitialSyncState} payload: {Encoding.ASCII.GetString(message.Serialize())}");

            stateController.Schedule(datas);
        }
    }
}