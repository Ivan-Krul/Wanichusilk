#pragma once
#include <bitset>
#include <memory>

#include "define.h"

template<size_t ChunkByte, size_t ChunkBits = POWER_OF_TWO(ChunkByte)>
class ResizableBitset {
public:
    using IndexType = DEFAULT_RESBITSET_INDEX_TYPE;
    using BitsetChunk = std::bitset<ChunkBits>;
    using BitsetMap = std::unique_ptr<BitsetChunk[]>;

    inline size_t size() const { return mSize; }
    inline size_t maxcalledbit() const { return mMaxCalledBit; }

    bool get(IndexType index) const {
        if (index >= totalBits()) return false;

        return mapBitsets[index >> ChunkByte][index % ChunkBits];
    }

    void set(IndexType index, bool val) {
        if (index >= totalBits()) resizeIfWants(index);
        mMaxCalledBit = std::max(mMaxCalledBit, index + 1);

        mapBitsets[index >> ChunkByte][index % ChunkBits] = val;
    }
private:
    void resizeIfWants(IndexType target);
    inline IndexType totalBits() const { return mSize * ChunkBits; }

    BitsetMap mapBitsets = nullptr;
    IndexType mSize = 0;
    IndexType mMaxCalledBit = 0;
};

template<size_t ChunkByte, size_t ChunkBits>
void ResizableBitset<ChunkByte, ChunkBits>::resizeIfWants(IndexType target) {
    assert(target <= DEFAULT_RESBITSET_SIZE_LIM);
    IndexType new_size = ((ChunkBits - (target % ChunkBits)) + target) >> ChunkByte;
    if (mapBitsets) {
        auto new_bitset = std::make_unique<BitsetChunk[]>(new_size);
        for (size_t i = 0; i < mSize; i++) new_bitset[i] = mapBitsets[i];
        mapBitsets = std::move(new_bitset);
    }
    else mapBitsets = std::make_unique<BitsetChunk[]>(new_size);
    mSize = new_size;
}

using ResBitset = ResizableBitset<DEFAULT_RESBITSET_C_BYTES>;
