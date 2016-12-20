// @file SmartPtr.hpp
// @brief 智能指针
// @author
// @version
// @date
// @c++11 使用std::shared_ptr

#pragma once

#include <utility>
#include <cassert>


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

}
