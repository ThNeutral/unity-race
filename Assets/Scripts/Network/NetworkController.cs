using System;
using System.Net;
using Network;
using Network.Shared;
using Shared.Interfaces;
using UnityEngine;

public class NetworkController : MonoBehaviour
{
    [SerializeField]
    private bool isServer;

    [SerializeField]
    private string address;

    [SerializeField]
    private int port;

    private IUpdatable handler;
    public bool IsRunning => handler != null;

    void Update()
    {
        if (isServer)
            UpdateWithTickRate();
        else
            UpdateInternal();
    }

    [SerializeField]
    private int tickRate = 30;
    private float tickRateCounter = 0;
    private void UpdateWithTickRate()
    {
        tickRateCounter += Time.deltaTime;
        if (tickRateCounter < (1f / tickRate))
            return;

        tickRateCounter = 0;
        UpdateInternal();
    }

    private void UpdateInternal()
    {
        if (IsRunning) handler.Update();
    }

    public void StartServer()
    {
        CheckIfIsAlreadyRunning();

        var server = new Server(port, 4);

        handler = server;
    }

    public void StartClient()
    {
        CheckIfIsAlreadyRunning();

        var endpoint = new IPEndPoint(IPAddress.Parse(address), port);
        var client = new Client(endpoint);
        client.Start();

        handler = client;
    }

    private void CheckIfIsAlreadyRunning()
    {
        if (IsRunning) throw new Exception($"{(isServer ? "server" : "client")} is already running");    
    }
}
