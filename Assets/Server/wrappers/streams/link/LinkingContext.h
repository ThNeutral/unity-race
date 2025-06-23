#ifndef LINKING_CONTEXT_H
#define LINKING_CONTEXT_H

#include <stdint.h>
#include <gameobject/GameObject.h>
#include <unordered_map>

class LinkingContext {
    public:
        uint32_t GetNetworkID(const GameObject* inGameObject);
        GameObject* GetGameObject(const uint32_t inNetworkID);

    private:
        std::unordered_map<uint32_t, GameObject*> idToObject;
        std::unordered_map<GameObject*, uint32_t> objectToID;
};

#endif