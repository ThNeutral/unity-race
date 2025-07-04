using UnityEngine;
using UnityEngine.InputSystem;

public class CarInputs : MonoBehaviour
{
    [SerializeField]
    private CarController controller;

    private DriveInputs inputs;
    private InputAction horizontal;
    private InputAction vertical;
    private InputAction @break;

    public bool AllowManualInput { get; set; }

    private void Awake()
    {
        inputs = new DriveInputs();
        horizontal = inputs.Driving.Horizontal;
        vertical = inputs.Driving.Vertical;
        @break = inputs.Driving.Break;
    }

    private void OnEnable()
    {
        horizontal.Enable();
        vertical.Enable();
        @break.Enable();
    }

    private void OnDisable()
    {
        horizontal.Disable();
        vertical.Disable();
        @break.Disable();
    }

    private void Update()
    {
        if (AllowManualInput)
        {
            Horizontal = horizontal.ReadValue<float>();
            Vertical = vertical.ReadValue<float>();
            IsBreaking = @break.ReadValue<float>() != 0;
        }
    }

    public float Horizontal
    {
        get => horizontal.ReadValue<float>();
        set => controller.HorizontalInput = value;
    }

    public float Vertical
    {
        get => vertical.ReadValue<float>();
        set => controller.VerticallInput = value;
    }

    public bool IsBreaking
    {
        get => @break.ReadValue<float>() != 0;
        set => controller.IsBreaking = value;
    }
}