using UnityEngine;
using Unity.Netcode;
using UnityEngine.InputSystem;

public class ClientMoveScript : NetworkBehaviour
{
    [SerializeField] private CarController m_CarController;
    [SerializeField] private CarInputs m_CarInputs;

    private void Awake()
    {
        m_CarController.enabled = false;
        m_CarInputs.enabled = false;
    }

    public override void OnNetworkSpawn()
    {
        base.OnNetworkSpawn();

        if (IsOwner)
        {
            m_CarInputs.enabled = true;
        }

        if (IsServer)
        {
            m_CarController.enabled = true;
        }
    }

    [Rpc(SendTo.Server)]
    private void UpdateInputServerRpc(float horizontalInput, float verticalInput, bool isBreaking)
    {
        m_CarInputs.Horizontal = horizontalInput;
        m_CarInputs.Vertical = verticalInput;
        m_CarInputs.IsBreaking = isBreaking;
    }

    private void LateUpdate()
    {
        if (!IsOwner) return;

        UpdateInputServerRpc(
            m_CarInputs.Horizontal,
            m_CarInputs.Vertical,
            m_CarInputs.IsBreaking
        );
    }
}
