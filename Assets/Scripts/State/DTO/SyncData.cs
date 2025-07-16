using UnityEngine;
using State.Enums;
using Store.Enums;

namespace State.DTO
{
    public class SyncData
    {
        public static SyncData CreateCarDefault()
        {
            return WithZeroID(SyncAction.Create, ObjectType.Car, Vector3.zero, Quaternion.identity);
        }

        public static SyncData WithZeroID(SyncAction action, ObjectType type, Vector3 pos, Quaternion quaternion)
        {
            return new(0, action, type, pos, quaternion);
        }

        public readonly SyncAction Action;
        public readonly ObjectType ObjectType;
        public readonly uint ObjectID;
        public readonly Vector3 Position;
        public readonly Quaternion Quaternion;

        public SyncData(uint objectID, SyncAction action, ObjectType type, Vector3 pos, Quaternion quaternion)
        {
            ObjectID = objectID;
            Position = pos;
            Action = action;
            ObjectType = type;
            Quaternion = quaternion;
        } 
    }
}