namespace Message
{
    public enum MessageType : byte
    {
        ClientHello,
        ServerAccept,
        ServerReject,
        ServerInitialSyncState
    }
}