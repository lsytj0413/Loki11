// @file Singleton.hpp
// @brief 单例模式实现模板
// @author
// @version
// @date

#pragma once


namespace Loki11
{

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

}

}
