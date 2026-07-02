#include "Interacter.h"

Interacter::Interacter(Vault using_vault) {
    mVault = using_vault;

    Keyroot kr;
    kr.seq_res_mode = kr.NewScene_NewLoader;
    maKeyRoots.insert(std::make_pair<size_t, Keyroot>{0, kr});

}

void Interacter::addBranch(size_t event_id) {
    Keyroot kr;
    maKeyRoots.insert(std::make_pair<size_t, Keyroot>{event_id, kr});
}

Interacter::Keyroot* Interacter::extractBranch(size_t event_id) {
    auto iter = maKeyRoots.find(event_id);
    if(iter == maKeyRoots.end()) return nullptr;
    else return *maKeyRoots[iter->first];
}

void Interacter::removeBranch(size_t event_id) {
    auto iter = maKeyRoots.find(event_id);
    if(iter == maKeyRoots.end())
        maKeyRoots.erase(iter->first);
}
