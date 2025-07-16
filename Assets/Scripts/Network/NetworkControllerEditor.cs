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

            var controller = (NetworkController)target;

            var isServerProp = serializedObject.FindProperty("isServer");
            EditorGUILayout.PropertyField(isServerProp);

            if (isServerProp.boolValue)
            {
                // Server section
                var portProp = serializedObject.FindProperty("port");
                EditorGUILayout.PropertyField(portProp);

                if (controller.IsRunning)
                {
                    EditorGUILayout.HelpBox("Server is already running", MessageType.Info);
                }
                else if (GUILayout.Button("Start Server"))
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

                if (controller.IsRunning)
                {
                    EditorGUILayout.HelpBox("Client is already running", MessageType.Info);     
                }
                else if (GUILayout.Button("Start Client"))
                {
                    controller.StartClient();
                }
            }

            serializedObject.ApplyModifiedProperties();
        }
    }   
}