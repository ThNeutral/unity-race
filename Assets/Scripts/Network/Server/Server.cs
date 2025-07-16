using System;
using System.Net;
using System.Net.Sockets;
using Network.ServerInternals;
using Network.Shared;
using Shared.Interfaces;
using UnityEngine;

namespace Network
{
    public class Server : IUpdatable
    {
        private readonly UDPClient udpClient;
        private readonly ConnectionsList connections;

        public Server(int port, int maximumNumberOfConnections)
        {
            udpClient = new(port);
            connections = new(maximumNumberOfConnections);
            Debug.Log($"listening on TCP {udpClient.LocalEndPoint}");
        }

        public void Update()
        {
            var code = udpClient.ReceiveFrom(out var bytes, out var endPoint);
            if (code == SocketError.WouldBlock)
            {
                // no data
                return;
            }

            if (code != SocketError.Success)
            {
                Debug.Log($"receive on {udpClient.LocalEndPoint} failed: {code}");
                return;
            }

            if (bytes.Length == 0)
            {
                Debug.Log($"zero byte payload from {endPoint}");
                return;
            }

            Debug.Log($"received {BitConverter.ToString(bytes)} from {endPoint}");

            var isNewConnection = !connections.Contains(endPoint.ToString());

            var type = bytes[0].ToMessageType();
            if (isNewConnection)
                HandleNewConnection(type, bytes, endPoint);
            else
                HandleExistingConnection(type, bytes, endPoint);
        }

        private void HandleNewConnection(MessageType type, byte[] payload, EndPoint endPoint)
        {
            if (type != MessageType.ClientHello)
            {
                Debug.Log($"expected {MessageType.ClientHello} message from client");
                return;
            }

            var resp = new byte[2] { MessageType.ServerHello.ToByte(), 0 };
            if (connections.TryAdd(endPoint.ToString())) resp[1] = 1;
            udpClient.SendTo(resp, endPoint);
        }

        private void HandleExistingConnection(MessageType type, byte[] payload, EndPoint endPoint)
        {
            
        }
    }   
}
