#pragma once

#include <typeinfo>
#include <cassert>


namespace Loki11
{

// C++11之后可以使用std::type_index
class TypeInfo
{
public:
    TypeInfo() {
        class Nil {};

        pInfo_ = &typeid(Nil);
        assert(pInfo_);
    };

    TypeInfo(const std::type_info& ti)
            : pInfo_(&ti)
    {
        assert(pInfo_);
    };

    TypeInfo(const TypeInfo& rhs)
            : pInfo_(rhs.pInfo_)
    {
        assert(pInfo_);
    };

    TypeInfo& operator=(const TypeInfo& rhs) {
        assert(rhs.pInfo_);
        pInfo_ = rhs.pInfo_;
        return *this;
    };

    bool before(const TypeInfo& rhs) const {
        assert(pInfo_);
        return pInfo_->before(*rhs.pInfo_) != 0;
    };

    const char* name() const {
        assert(pInfo_);
        return pInfo_->name();
    };

    const std::type_info& Get() const {
        assert(pInfo_);
        return *pInfo_;
    };

private:
    const std::type_info* pInfo_;
};

inline bool operator==(const TypeInfo& lhs, const TypeInfo& rhs)
{
    return (lhs.Get() == rhs.Get()) != 0;
}

inline bool operator<(const TypeInfo& lhs, const TypeInfo& rhs)
{
    return lhs.before(rhs);
}

inline bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs)
{
    return !(lhs == rhs);
}

inline bool operator>(const TypeInfo& lhs, const TypeInfo& rhs)
{
    return rhs < lhs;
}

inline bool operator<=(const TypeInfo& lhs, const TypeInfo& rhs)
{
    return !(lhs > rhs);
}

inline bool operator>=(const TypeInfo& lhs, const TypeInfo& rhs)
{
    return !(lhs < rhs);
}

}
