using UnityEngine;
using UnityEditor;

namespace Network
{
    [CustomEditor(typeof(NetworkController))]
    public class NetworkControllerEditor : Editor
    {
        public override void OnInspectorGUI()
        {
            serializedObject.Update();

            NetworkController controller = (NetworkController)target;

            SerializedProperty isServerProp = serializedObject.FindProperty("isServer");
            EditorGUILayout.PropertyField(isServerProp);

            if (isServerProp.boolValue)
            {
                // Server section
                SerializedProperty portProp = serializedObject.FindProperty("port");
                EditorGUILayout.PropertyField(portProp);

                if (GUILayout.Button("Start Server"))
                {
                    controller.StartServer();
                }
            }
            else
            {
                // Client section
                SerializedProperty serverIPProp = serializedObject.FindProperty("serverIP");
                SerializedProperty serverPortProp = serializedObject.FindProperty("serverPort");

                EditorGUILayout.PropertyField(serverIPProp);
                EditorGUILayout.PropertyField(serverPortProp);

                if (GUILayout.Button("Start Client"))
                {
                    controller.StartClient();
                }
            }

            serializedObject.ApplyModifiedProperties();
        }
    }   
}