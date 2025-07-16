using System;
using System.Net;
using System.Net.Sockets;
using Network.Shared;
using Shared.Interfaces;
using UnityEngine;

namespace Network
{
    public class Client : IUpdatable
    {
        private readonly UDPClient udpClient;

        private readonly EndPoint remoteEndPoint;

        private bool isConnected;

        public Client(IPEndPoint endPoint)
        {
            udpClient = new(0);
            remoteEndPoint = endPoint;
        }

        public void Start()
        {
            isConnected = false;
            Debug.Log($"connecting to {remoteEndPoint}...");
            udpClient.SendTo(new byte[] { MessageType.ClientHello.ToByte() }, remoteEndPoint);
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

            var type = bytes[0].ToMessageType();
            if (!isConnected)
                HandleNonConnectedMessage(type, bytes, endPoint);
            else
                HandleConnectedMessage(type, bytes, endPoint);
        }

        private void HandleNonConnectedMessage(MessageType type, byte[] payload, EndPoint endPoint)
        {
            if (type != MessageType.ServerHello)
            {
                Debug.Log($"expected {MessageType.ServerHello} message from server");
                return;
            }

            if (payload[1] == 0)
            {
                Debug.Log($"connection rejected by {endPoint}");
                return;
            }

            Debug.Log($"connected to {endPoint}");
            isConnected = true;
        }

        private void HandleConnectedMessage(MessageType type, byte[] payload, EndPoint endPoint)
        {
            switch (type)
            {
                case MessageType.ServerHello:
                case MessageType.ClientHello:
                default:
                    Debug.Log($"unexpected {nameof(MessageType)} value from {endPoint}: {type}");
                    break;
            }
        }
    }    
}

