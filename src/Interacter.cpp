#include "Interacter.h"

bool Interacter::create(Vault using_vault) {
    mVault = using_vault;

    Keyroot kr;
    maKeyRoots.insert({0, kr});

    return false;
}

void Interacter::addBranch(size_t event_id) {
    Keyroot kr;
    maKeyRoots.insert({event_id, kr});
}

Keyroot* Interacter::extractBranch(size_t event_id) {
    auto iter = maKeyRoots.find(event_id);
    if(iter == maKeyRoots.end()) return nullptr;
    else return &(maKeyRoots[iter->first]);
}

void Interacter::removeBranch(size_t event_id) {
    auto iter = maKeyRoots.find(event_id);
    if(iter == maKeyRoots.end())
        maKeyRoots.erase(iter->first);
}

void Interacter::forceEvent(size_t event_id) {
    
}

void Interacter::start() {
    
}
