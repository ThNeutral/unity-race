using System;
using System.Net;
using System.Net.Sockets;
using UnityEngine;

namespace Network.Internals.TCP
{
    public class Listener : IDisposable
    {
        private readonly Socket socket;
        public Socket Socket { get => socket; }

        public IPEndPoint LocalEndPoint { get => (IPEndPoint)socket.LocalEndPoint; }

        public Listener(int port, int backlog = 10)
        {
            socket = new(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            var endpoint = new IPEndPoint(IPAddress.Any, port);
            socket.Bind(endpoint);
            socket.Listen(backlog);
        }

        public Client Accept()
        {
            var client = socket.Accept();
            return new(client);
        }

        public void Close()
        {
            socket.Close();
        }

        private bool disposed = false;
        public void Dispose()
        {
            if (!disposed)
            {
                socket.Dispose();
                disposed = true;
                GC.SuppressFinalize(this);
            }
        }
    }
}