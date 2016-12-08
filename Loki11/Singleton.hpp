// @file Singleton.hpp
// @brief 单例模式实现模板
// @author
// @version
// @date

#pragma once

#include <cassert>
#include <cstdlib>
#include <new>
#include <algorithm>
#include <cstddef>
#include <stdexcept>

#include "Threads.hpp"


namespace Loki11
{

using atexit_pfn_t = void (*)();

namespace Private
{

class LifetimeTracker
{
private:
    unsigned int m_longevity;

public:
    LifetimeTracker(unsigned int x):
            m_longevity(x)
    {};

    virtual ~LifetimeTracker() = 0;

    static bool Compare(const LifetimeTracker* lhs, const LifetimeTracker* rhs)
    {
        return lhs->m_longevity < rhs->m_longevity;
    }
};

inline LifetimeTracker::~LifetimeTracker() {};


using TrackerArray = LifetimeTracker**;
static TrackerArray pTrackerArray = nullptr;
static unsigned int elements = 0;


template <typename T>
struct Deleter
{
    static void Delete(T* p) {
        delete p;
    }
};


template <typename T, typename Destroyer>
class ConcreteLifetimeTracker: public LifetimeTracker
{
private:
    T* m_tracked;
    Destroyer m_destroyer;

public:
    ConcreteLifetimeTracker(T* p, unsigned int longevity, Destroyer d)
            : LifetimeTracker(longevity)
            , m_tracked(p)
            , m_destroyer(d)
    {};

    ~ConcreteLifetimeTracker() {
        m_destroyer(m_tracked);
    }
};


inline void AtExitFn() {
    assert(elements > 0 && pTrackerArray != nullptr);

    LifetimeTracker* top = pTrackerArray[elements - 1];
    pTrackerArray = static_cast<TrackerArray>(std::realloc(pTrackerArray,
                                                           sizeof(*pTrackerArray) * --elements)
                                              );
    delete top;
};

}

template <typename T, typename Destroyer>
void SetLongevity(T* pDynObj, unsigned int longevity, Destroyer d = Private::Deleter<T>::Delete)
{
    using namespace Private;

    TrackerArray pNewArray = static_cast<TrackerArray>(std::realloc(pTrackerArray,
                                                                    sizeof(*pTrackerArray) * (elements + 1))
                                                       );
    if (!pNewArray) {
        throw std::bad_alloc();
    }

    pTrackerArray = pNewArray;
    LifetimeTracker* p = new ConcreteLifetimeTracker<T, Destroyer>(pDynObj, longevity, d);
    TrackerArray pos = std::upper_bound(pTrackerArray,
                                        pTrackerArray + elements,
                                        p,
                                        LifetimeTracker::Compare
                                        );
    std::copy_backward(pos, pTrackerArray + elements, pTrackerArray + elements + 1);
    *pos = p;
    ++elements;

    std::atexit(Private::AtExitFn);
};


template <class T>
struct CreateUsingNew
{
    static T* Create() {
        return new T;
    };

    static void Destory(T* p) {
        delete p;
    };
};


template <class T>
struct CreateUsingMalloc
{
    static T* Create() {
        void *p = std::malloc(sizeof(T));
        if (!p) {
            return nullptr;
        }

        return new(p) T;
    };

    static void Destory(T* p) {
        p->~T();
        std::free(p);
    };
};


template <class T>
struct CreateStatic
{
    static T* Create() {
        static std::max_align_t staticMem;
        return new(&staticMem) T;
    };

    static void Destory(T* p) {
        p->~T();
    };
};


template <class T>
struct DefaultLifetime
{
    static void ScheduleDestruction(T*, atexit_pfn_t fn) {
        std::atexit(fn);
    };

    static void OnDeadReference() {
        throw std::logic_error("Dead Reference Detected");
    };
};


template <class T>
class PhoenixSingleton
{
private:
    static bool m_destroy_once;

public:
    static void ScheduleDestruction(T*, atexit_pfn_t fn) {
        if(m_destroy_once) {
            std::atexit(fn);
        }
    };

    static void OnDeadReference() {
        m_destroy_once = true;
    };
};

template <class T>
bool PhoenixSingleton<T>::m_destroy_once = false;


namespace Private
{

template <class T>
struct Adapter
{
    atexit_pfn_t m_fn;
    void operator()(T*) {
        return m_fn();
    };
};

}


template <class T>
class SingletonWithLongevity
{
public:
    static void ScheduleDestruction(T* pObj, atexit_pfn_t fn) {
        Private::Adapter<T> adapter { fn };
        SetLongevity(pObj, GetLongevity(pObj), adapter);
    };

    static void OnDeadReference() {
        throw std::logic_error("Dead Reference Detected");
    };
};


template <class T>
struct NoDestory
{
    static void ScheduleDestruction(T*, atexit_pfn_t fn) {};
    static void OnDeadReference() {};
};


template <typename T,
          template <class> class CreationPolicy = CreateUsingNew,
          template <class> class LifetimePolicy = DefaultLifetime,
          template <class> class ThreadingModel = SingleThreaded
          >
class SingletonHolder
{
private:
    static void MakeInstance();
    static void DestroySingleton();

    SingletonHolder();

    using PtrInstanceType = typename ThreadingModel<T*>::VolatileType;
    static PtrInstanceType m_instance;
    static bool m_destroyed;

public:
    static T& Instance();
};


}
