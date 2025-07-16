using System.Collections.Generic;
using Store.Enums;
using UnityEngine;

namespace Store
{
    public class PrefabStore : MonoBehaviour
    {
        private static PrefabStore instance;
        public static PrefabStore Instance => instance;

        [SerializeField]
        private List<ObjectType> objectsTypes;

        [SerializeField]
        private List<GameObject> gameObjects;

        private Dictionary<ObjectType, GameObject> typeToObject;

        private void Awake()
        {
            var obj = FindFirstObjectByType<PrefabStore>();
            if (instance != null)
            {
                Debug.Log($"Found duplicate {nameof(PrefabStore)}");
                Destroy(this);
                return;
            }

            instance = this;

            if (objectsTypes.Count != gameObjects.Count)
                throw new System.Exception($"size mismatch between {nameof(objectsTypes)} and {nameof(gameObjects)}");

            typeToObject = new(objectsTypes.Count);

            for (int i = 0; i < objectsTypes.Count; i++)
            {
                typeToObject.Add(objectsTypes[i], gameObjects[i]);
            }
        }

        public bool TryGetGameObjectByObjectType(ObjectType key, out GameObject @object)
        {
            return typeToObject.TryGetValue(key, out @object);
        }
    }
}