using System.Collections.Concurrent;
using System.Collections.Generic;
using State.DTO;
using State.Enums;
using Store;
using Store.Enums;
using UnityEngine;

namespace State
{
    public class StateController : MonoBehaviour
    {
        private static StateController instance;
        public static StateController Instance => instance;

        private void Awake()
        {
            var obj = FindFirstObjectByType<StateController>();
            if (instance != null)
            {
                Debug.Log($"Found duplicate {nameof(StateController)}");
                Destroy(this);
                return;
            }

            instance = this;
        }

        private PrefabStore store;

        private void Start()
        {
            store = PrefabStore.Instance;
        }

        private void Update()
        {
            while (actions.TryDequeue(out var data))
            {
                HandleSyncData(data);
            }

            datas.Clear();
            datas.Capacity = idToObject.Count;
            foreach (var kvp in idToObject)
            {
                var transform = kvp.Value.transform;
                var data = new SyncData(kvp.Key, SyncAction.Create, ObjectType.Car, transform.position, transform.rotation);
                datas.Add(data);
            }
        }

        private static readonly object mutex = new();
        private static uint prevID = 0;
        private static uint NextID
        {
            get
            {
                lock (mutex)
                {
                    prevID += 1;
                    return prevID;
                }
            }
        }

        private readonly Dictionary<GameObject, uint> objectToID = new();
        private readonly Dictionary<uint, GameObject> idToObject = new();

        private readonly List<SyncData> datas = new();
        public List<SyncData> GetInitialSyncData()
        {
            return datas;
        }

        private readonly ConcurrentQueue<SyncData> actions = new();

        public void Schedule(List<SyncData> syncDatas)
        {
            foreach (var data in syncDatas)
            {
                Schedule(data);
            }
        }

        public void Schedule(SyncData syncData)
        {
            actions.Enqueue(syncData);
        }

        private void HandleSyncData(SyncData data)
        {
            switch (data.Action)
            {
                case SyncAction.Create:
                    HandleSyncDataCreate(data);
                    break;
                default:
                    throw new System.Exception($"Unexpected {nameof(SyncAction)} value: {data.Action}");
            }
        }

        private void HandleSyncDataCreate(SyncData data)
        {
            if (!store.TryGetGameObjectByObjectType(data.ObjectType, out var gameObject))
                throw new System.Exception($"no prefab for {data.ObjectType}");
                
            switch (data.ObjectType)
            {
                case ObjectType.Car:
                    var instance = Instantiate(gameObject);
                    AddObject(instance);
                    break;

                default:
                    throw new System.Exception($"Unexpected {nameof(ObjectType)} value: {data.ObjectType}");
            }
        }

        private void AddObject(GameObject @object)
        {
            var existingID = GetIDByObject(@object);
            if (existingID != 0)
                throw new System.Exception($"object {@object} is already present in state controller with id {existingID}");

            AddObjectWithID(NextID, @object);
        }

        private void AddObjectWithID(uint id, GameObject @object)
        {
            var existingObject = GetObjectByID(id);
            if (existingObject != null)
                throw new System.Exception($"id {id} is already present in state controller");

            objectToID.Add(@object, id);
            idToObject.Add(id, @object);
        }

        public GameObject GetObjectByID(uint id)
        {
            if (!idToObject.TryGetValue(id, out var @object)) return null;
            return @object;
        }

        public uint GetIDByObject(GameObject @object)
        {
            if (!objectToID.TryGetValue(@object, out var id)) return 0;
            return id;
        }
    }
}