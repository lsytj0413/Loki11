#pragma once

#include <cstddef>


namespace Loki11
{

class FixedAllocator
{
private:
    struct Chunk
    {
        void Init(std::size_t blockSize, unsigned char blocks) {
            m_data = new unsigned char[blockSize * blocks];
            m_first_available_block = 0;
            m_blocks_available = blocks;
            unsigned char i = 0;
            unsigned char* p = m_data;
            for(; i != blocks; p += blockSize) {
                *p = ++i;
            }
        };

        void Release();

        void* Allcocate(std::size_t blockSize);
        void Deallocate(void* p, std::size_t blockSize);

        unsigned char* m_data;
        unsigned char m_first_available_block;
        unsigned char m_blocks_available;
    };
};

}
