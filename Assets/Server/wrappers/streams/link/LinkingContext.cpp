#include <streams/link/LinkingContext.h>

uint32_t LinkingContext::GetNetworkID(const GameObject* inGameObject) {
    auto it = objectToID.find(const_cast<GameObject*>(inGameObject));
    if (it != objectToID.end()) {
        return it->second;
    }
    return 0;
}

GameObject* LinkingContext::GetGameObject(uint32_t inNetworkID) {
    auto it = idToObject.find(inNetworkID);
    if (it != idToObject.end()) {
        return it->second;
    }
    return nullptr;
}