using UnityEngine;
using System.Net;
using System.Net.Sockets;
using Network.Internals.Message;
using System;
using System.Text;

namespace Network.Runners
{
    public class Client
    {
        private readonly Internals.TCP.Client tcpClient;
        private readonly Internals.UDP.Client udpClient;

        public Client()
        {
            tcpClient = new(0);
            udpClient = new(0);
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
                    udpClient.SendTo(Encoding.ASCII.GetBytes("test"), serverUDPEndPoint);
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
    }
}