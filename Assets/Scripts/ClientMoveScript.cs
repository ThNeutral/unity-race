using UnityEngine;
using Unity.Netcode;
using UnityEngine.InputSystem;

public class ClientMoveScript : NetworkBehaviour
{
    [SerializeField] private CarController m_CarController;

    private void Awake()
    {
        m_CarController.enabled = false;
    }

    public override void OnNetworkSpawn()
    {
        base.OnNetworkSpawn();

        if (IsServer)
        {
            m_CarController.enabled = true;
        }
    }
}
