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

}
