#pragma once

#include <cstddef>
#include <cassert>
#include <vector>
#include <algorithm>
#include <limits.h>

#ifndef DEFAULT_CHUNK_SIZE
#define DEFAULT_CHUNK_SIZE 4096
#endif

#ifndef MAX_SMALL_OBJECT_SIZE
#define MAX_SAMLL_OBJECT_SIZE 64
#endif


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

        void Release() {
            delete []m_data;
        };

        void* Allcocate(std::size_t blockSize) {
            if (!m_blocks_available) {
                return nullptr;
            }

            unsigned char* pResult = m_data + (m_first_available_block*blockSize);
            m_first_available_block = *pResult;
            --m_blocks_available;
            return pResult;
        };

        void Deallocate(void* p, std::size_t blockSize) {
            assert(p >= m_data);

            unsigned char* toRelease = static_cast<unsigned char*>(p);
            assert((toRelease - m_data) % blockSize == 0);

            *toRelease = m_first_available_block;
            m_first_available_block = static_cast<unsigned char>((toRelease-m_data)/blockSize);
            assert(m_first_available_block == (toRelease-m_data)/blockSize);

            ++m_blocks_available;
        };

        unsigned char* m_data;
        unsigned char m_first_available_block;
        unsigned char m_blocks_available;
    };

private:
    std::size_t m_block_size;
    unsigned char m_num_blocks;
    using Chunks = std::vector<Chunk>;
    Chunks m_chunks;
    Chunk* m_alloc_chunk;
    Chunk* m_dealloc_chunk;

    mutable const FixedAllocator* m_prev;
    mutable const FixedAllocator* m_next;

public:
    FixedAllocator(std::size_t blockSize)
            : m_block_size(blockSize)
            , m_alloc_chunk(nullptr)
            , m_dealloc_chunk(nullptr)
    {
        assert(m_block_size > 0);

        m_prev = m_next = this;

        std::size_t numBlocks = DEFAULT_CHUNK_SIZE / blockSize;
        if (numBlocks > UCHAR_MAX) {
            numBlocks = UCHAR_MAX;
        }
        else if (numBlocks == 0) {
            numBlocks = 8*blockSize;
        }

        m_num_blocks = static_cast<unsigned char>(numBlocks);
        assert(m_num_blocks == numBlocks);
    }

    FixedAllocator(const FixedAllocator& rhs)
            : m_block_size(rhs.m_block_size)
            , m_num_blocks(rhs.m_num_blocks)
            , m_chunks(rhs.m_chunks)
    {
        m_prev = &rhs;
        m_next = rhs.m_next;
        rhs.m_next->m_prev = this;
        rhs.m_next = this;

        m_alloc_chunk = rhs.m_alloc_chunk
                        ? &m_chunks.front() + (rhs.m_alloc_chunk - &rhs.m_chunks.front())
                        : nullptr;
        m_dealloc_chunk = rhs.m_dealloc_chunk
                          ? &m_chunks.front() + (rhs.m_dealloc_chunk - &rhs.m_chunks.front())
                          : 0;
    }

    FixedAllocator& operator=(const FixedAllocator& rhs)
    {
        FixedAllocator copy(rhs);
        copy.Swap(*this);
        return *this;
    }

    ~FixedAllocator()
    {
        if (m_prev != this) {
            m_prev->m_next = m_next;
            m_next->m_prev = m_prev;
            return ;
        }

        assert(m_prev == m_next);

        Chunks::iterator i = m_chunks.begin();
        for(; i != m_chunks.end(); ++i){
            assert(i->m_blocks_available == m_num_blocks);
            i->Release();
        }
    }

    std::size_t BlockSize() const {
        return m_block_size;
    }

    void Swap(FixedAllocator& rhs)
    {
        std::swap(m_block_size, rhs.m_block_size);
        std::swap(m_num_blocks, rhs.m_num_blocks);
        m_chunks.swap(rhs.m_chunks);
        std::swap(m_alloc_chunk, rhs.m_alloc_chunk);
        std::swap(m_dealloc_chunk, rhs.m_dealloc_chunk);
    }

    void* Allocate() {
        if (m_alloc_chunk == nullptr || m_alloc_chunk->m_blocks_available == 0) {
            Chunks::iterator i = m_chunks.begin();
            for(;; ++i) {
                if (i == m_chunks.end()) {
                    m_chunks.push_back(Chunk());
                    Chunk& newChunk = m_chunks.back();
                    newChunk.Init(m_block_size, m_num_blocks);
                    m_alloc_chunk = &newChunk;
                    m_dealloc_chunk = &m_chunks.front();
                    break;
                }
                else if (i->m_blocks_available > 0) {
                    m_alloc_chunk = &(*i);
                    break;
                }
            }
        }

        assert(m_alloc_chunk != nullptr);
        assert(m_alloc_chunk->m_blocks_available > 0);
        return m_alloc_chunk->Allcocate(m_block_size);
    }

    void Deallocate(void *p)
    {
        assert(!m_chunks.empty());
        assert(&m_chunks.front() <= m_dealloc_chunk);
        assert(&m_chunks.back() >= m_dealloc_chunk);

        m_dealloc_chunk = VicinityFind(p);
        assert(m_dealloc_chunk);

        DoDeallocate(p);
    }

    Chunk* VicinityFind(void* p)
    {
        assert(!m_chunks.empty());
        assert(m_dealloc_chunk);

        const std::size_t chunkLength = m_num_blocks * m_block_size;

        Chunk* lo = m_dealloc_chunk;
        Chunk* hi = m_dealloc_chunk + 1;
        Chunk* loBound = &m_chunks.front();
        Chunk* hiBound = &m_chunks.back() + 1;

        if (hi == hiBound) {
            hi = nullptr;
        }

        for(;;) {
            if (lo) {
                if (p >= lo->m_data && p < lo->m_data + chunkLength) {
                    return lo;
                }
                if (lo == loBound) {
                    lo = nullptr;
                }
                else {
                    --lo;
                }
            }

            if (hi) {
                if (p >= hi->m_data && p < hi->m_data + chunkLength) {
                    return hi;
                }
                if (++hi == hiBound) {
                    hi = nullptr;
                }
            }
        }

        assert(false);
        return nullptr;
    }

    void DoDeallocate(void* p)
    {
        assert(m_dealloc_chunk->m_data <= p);
        assert(m_dealloc_chunk->m_data + m_num_blocks*m_block_size > p);

        m_dealloc_chunk->Deallocate(p, m_block_size);

        if (m_dealloc_chunk->m_blocks_available == m_num_blocks) {
            Chunk& lastChunk = m_chunks.back();

            if (&lastChunk == m_dealloc_chunk) {
                if (m_chunks.size() > 1 && m_dealloc_chunk[-1].m_blocks_available == m_num_blocks) {
                    lastChunk.Release();
                    m_chunks.pop_back();
                    m_alloc_chunk = m_dealloc_chunk = &m_chunks.front();
                }
                return ;
            }

            if (lastChunk.m_blocks_available == m_num_blocks) {
                lastChunk.Release();
                m_chunks.pop_back();
                m_alloc_chunk = m_dealloc_chunk;
            }
            else {
                std::swap(*m_dealloc_chunk, lastChunk);
                m_alloc_chunk = &m_chunks.back();
            }
        }
    }
};

class SmallObjAllocator
{
private:
    using Pool = std::vector<FixedAllocator>;
    Pool m_pool;
    FixedAllocator* m_last_alloc;
    FixedAllocator* m_last_dealloc;
    std::size_t m_chunk_size;
    std::size_t m_max_object_size;

public:
    SmallObjAllocator(std::size_t chunkSize, std::size_t maxObjectSize)
            : m_last_alloc(nullptr)
            , m_last_dealloc(nullptr)
            , m_chunk_size(chunkSize)
            , m_max_object_size(maxObjectSize)
    {};

    void* Allocate(std::size_t numBytes)
    {
        if (numBytes > m_max_object_size) {
            return operator new(numBytes);
        }

        if (m_last_alloc && m_last_alloc->BlockSize() == numBytes) {
            return m_last_alloc->Allocate();
        }

        auto i = std::lower_bound(m_pool.begin(),
                                  m_pool.end(),
                                  numBytes,
                                  [](const FixedAllocator& v, int numBytes) -> bool {
                                      return v.BlockSize() <= numBytes;
                                  });
        if (i == m_pool.end() || i->BlockSize() != numBytes) {
            i = m_pool.insert(i, FixedAllocator(numBytes));
            m_last_dealloc = &(*m_pool.begin());
        }

        m_last_alloc = &(*i);
        return m_last_alloc->Allocate();
    };

    void Deallocate(void* p, std::size_t size)
    {
        if (size > m_max_object_size) {
            return operator delete(p);
        }

        if (m_last_dealloc && m_last_dealloc->BlockSize() == size) {
            m_last_dealloc->Deallocate(p);
            return ;
        }

        auto i = std::lower_bound(m_pool.begin(),
                                  m_pool.end(),
                                  size,
                                  [](const FixedAllocator& v, int numBytes) -> bool {
                                      return v.BlockSize() <= numBytes;
                                  });
        assert(i != m_pool.end());
        assert(i->BlockSize() == size);
        m_last_dealloc = &(*i);
        m_last_dealloc->Deallocate(p);
    };
};

}
