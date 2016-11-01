// @file Threads.hpp
// @brief 线程锁定模型
// @author
// @version
// @date

#pragma once

#include <mutex>
#include <atomic>


namespace Loki11
{

template <class Host>
class SingleThreaded
{
public:
    struct Lock {
        Lock() {};
        explicit Lock (const SingleThreaded&) {};
    };

    using VolatileType = Host;
    using IntType = int;

    static IntType AtomicAdd(volatile IntType& lval, IntType val) {
        return lval += val;
    };

    static IntType AtomicSubtract(volatile IntType& lval, IntType val) {
        return lval -= val;
    };

    static IntType AtomicMultiply(volatile IntType& lval, IntType val) {
        return lval *= val;
    };

    static IntType AtomicDivide(volatile IntType& lval, IntType val) {
        return lval /= val;
    };

    static IntType AtomicIncrement(volatile IntType& lval) {
        return ++lval;
    };

    static IntType AtomicDecrement(volatile IntType& lval) {
        return --lval;
    };

    static void AtomicAssign(volatile IntType& lval, IntType val) {
        lval = val;
    };

    static void AtomicAssign(IntType& lval, volatile IntType val) {
        lval = val;
    };
};


template <class Host>
class ObjectLevelLockable
{
    mutable std::mutex m_mutex;

public:
    ObjectLevelLockable() {
        // m_mutex.lock();
    };

    ~ObjectLevelLockable() {
        // m_mutex.unlock();
    };

    class Lock;
    friend class Lock;

    class Lock {
        ObjectLevelLockable const& m_host;

        Lock(const Lock&) = delete;
        Lock(Lock&&) = delete;
        Lock& operator=(const Lock&) = delete;
        Lock& operator=(Lock&&) = delete;

    public:
        explicit Lock(const ObjectLevelLockable& host) :
                m_host(host)
        {
            m_host.m_mutex.lock();
        }

        ~Lock() {
            m_host.m_mutex.unlock();
        }
    };

    using VolatileType = volatile Host;
    using IntType = long;

    // TODO
    static IntType AtomicIncrement(IntType& lval) {
        return *(std::atomic<IntType*>(&lval)++);
    };

    static IntType AtomicDecrement(IntType& lval) {
        return *(std::atomic<IntType*>(&lval)--);
    };

    static void AtomicAssign(IntType& lval, IntType val) {
        std::atomic<IntType*>(&lval).exchange(&val);
    }
};


template <class Host>
class ClassLevelLockable
{
    struct Initializer
    {
        std::mutex m_mutex;
    };

    static Initializer m_initializer;

public:
    class Lock;
    friend class Lock;

    class Lock
    {
        Lock(const Lock&) = delete;
        Lock(Lock&&) = delete;
        Lock& operator=(const Lock&) = delete;
        Lock& operator=(Lock&&) = delete;

    public:
        Lock () {
            m_initializer.m_mutex.lock();
        };

        explicit Lock (const ClassLevelLockable&) {
            m_initializer.m_mutex.lock();
        }

        ~Lock() {
            m_initializer.m_mutex.unlock();
        }
    };

    using VolatileType = volatile Host;
    using IntType = long;

    // TODO
    static IntType AtomicIncrement(IntType& lval) {
        return *(std::atomic<IntType*>(&lval)++);
    };

    static IntType AtomicDecrement(IntType& lval) {
        return *(std::atomic<IntType*>(&lval)--);
    };

    static void AtomicAssign(IntType& lval, IntType val) {
        std::atomic<IntType*>(&lval).exchange(&val);
    }
};

template <class Host>
typename ClassLevelLockable<Host>::Initializer ClassLevelLockable<Host>::m_initializer;

}

#ifndef DEFAULT_THREADING
#define DEFAULT_THREADING /**/ ::Loki11::SingleThreaded
#endif
