using System.Collections.Generic;

namespace Network.ServerInternals
{
    public class ConnectionsList
    {
        private readonly HashSet<string> connections;
        private readonly int maximumNumberOfConnections;

        public ConnectionsList(int maximumNumberOfConnections)
        {
            this.maximumNumberOfConnections = maximumNumberOfConnections;
            connections = new(maximumNumberOfConnections);
        }

        public bool TryAdd(string connection)
        {
            if (connections.Count >= maximumNumberOfConnections || connections.Contains(connection))
                return false;

            connections.Add(connection);
            return true;
        }

        public void Remove(string connection)
        {
            connections.Remove(connection);
        }

        public bool Contains(string connection)
        {
            return connections.Contains(connection);
        }

        public IReadOnlyCollection<string> GetConnections()
        {
            return connections;
        }
    }
}