// @file Threads.hpp
// @brief 线程锁定模型
// @author
// @version
// @date

#pragma once

#include <mutex>
#include <atomic>


// @c++11 使用std::thread及相关组件
namespace Loki11
{

// @class SingleThreaded
// @brief 单线程模型
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

    // @function
    // @brief 原子加
    // @param lval: 待增加值
    // @param val: 增加值
    // @return IntType
    static IntType AtomicAdd(volatile IntType& lval, IntType val) {
        return lval += val;
    };

    // @function
    // @brief 原子减
    // @param lval: 待减值
    // @param val: 减值
    // @return IntType
    static IntType AtomicSubtract(volatile IntType& lval, IntType val) {
        return lval -= val;
    };

    // @function
    // @brief 原子乘
    // @param lval: 待乘值
    // @param val: 乘值
    // @return IntType
    static IntType AtomicMultiply(volatile IntType& lval, IntType val) {
        return lval *= val;
    };

    // @function
    // @brief 原子除
    // @param lval: 待除值
    // @param val: 除值
    // @return IntType
    static IntType AtomicDivide(volatile IntType& lval, IntType val) {
        return lval /= val;
    };

    // @function
    // @brief 原子自增
    // @param lval: 自增值
    // @return IntType
    static IntType AtomicIncrement(volatile IntType& lval) {
        return ++lval;
    };

    // @function
    // @brief 原子自减
    // @param lval: 自减值
    // @return IntType
    static IntType AtomicDecrement(volatile IntType& lval) {
        return --lval;
    };

    // @function
    // @brief 原子赋值
    // @param lval: 被赋值值
    // @param val: 新值
    static void AtomicAssign(volatile IntType& lval, IntType val) {
        lval = val;
    };

    // @function
    // @brief 原子赋值
    // @param lval: 被赋值值
    // @param val: 新值
    static void AtomicAssign(IntType& lval, volatile IntType val) {
        lval = val;
    };
};

// @class ObjectLevelLockable
// @brief 对象层次多线程模型
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

// @class ClassLevelLockable
// @brief 类层次多线程模型
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
