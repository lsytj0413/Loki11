// @file SmartPtr.hpp
// @brief 智能指针
// @author
// @version
// @date
// @c++11 使用std::shared_ptr

#pragma once

#include <utility>
#include <cassert>
#include <mutex>


namespace Loki11
{

template <typename T>
class DefaultSPStorage
{
public:
    using StoredType = T*;
    using PointerType = T*;
    using ReferenceType = T&;

private:
    StoredType m_pointee;

public:
    DefaultSPStorage()
            : m_pointee(Default())
    {};

    DefaultSPStorage(const DefaultSPStorage&)
    {};

    template <class U>
    DefaultSPStorage(const DefaultSPStorage<U>&)
    {};

    DefaultSPStorage(const StoredType& p)
            : m_pointee(p)
    {};

    PointerType operator-> () const {
        return m_pointee;
    };

    ReferenceType operator* () const {
        return *m_pointee;
    };

    void Swap(DefaultSPStorage& rhs) {
        std::swap(m_pointee, rhs.m_pointee);
    };

    friend inline PointerType GetImpl(const DefaultSPStorage& sp) {
        return sp.m_pointee;
    };

    friend inline const StoredType& GetImpl(const DefaultSPStorage& sp) {
        return sp.m_pointee;
    };

    friend inline StoredType& GetImplRef(const DefaultSPStorage& sp) {
        return sp.m_pointee;
    };

protected:
    static StoredType Default () {
        return nullptr;
    };

    void Destroy() {
        delete m_pointee;
    };
};


template <class P>
class RefCounted
{
private:
    unsigned int* m_count;

public:
    RefCounted() {
        m_count = new unsigned int(1);
        assert(m_count);
    };

    RefCounted(const RefCounted& rhs)
            : m_count(rhs.m_count)
    {};

    template <typename P1>
    RefCounted(const RefCounted<P1>& rhs)
            : m_count(reinterpret_cast<const RefCounted&>(rhs).m_count)
    {};

    P Clone(const P& val) {
        ++(*m_count);
        return val;
    };

    bool Release(const P&) {
        if (!(--(*m_count))) {
            delete m_count;
            m_count = nullptr;
            return true;
        }

        return false;
    };

    void Swap(RefCounted& rhs) {
        std::swap(m_count, rhs.m_count);
    };
};


struct RefCountedMTAdj
{
    template <class P>
    class RefCountedMT
    {
    private:
        unsigned int* m_count;
        std::mutex m_mutex;

    public:
        RefCountedMT() {
            m_count = new unsigned int(1);
            assert(m_count);
        };

        RefCountedMT(const RefCountedMT& rhs)
                : m_count(rhs.m_count)
        {};

        template <typename P1>
        RefCountedMT(const RefCountedMT<P1>& rhs)
                : m_count(reinterpret_cast<const RefCountedMT<P>&>(rhs).m_count)
        {};

        P Clone(const P& val) {
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                ++(*m_count);
            }
            return val;
        };

        bool Release(const P&) {
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                --(*m_count);
                if (!(*m_count)) {
                    delete m_count;
                    m_count = nullptr;
                    return true;
                }
            }

            return false;
        };

        void Swap(RefCountedMT& rhs) {
            std::swap(m_count, rhs.m_count);
        };
    };
};


template <typename T>
class SmartPtr
{
};


}
