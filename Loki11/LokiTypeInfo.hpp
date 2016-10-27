// @file LokiTypeInfo.hpp
// @brief std::type_index的封装
// @author
// @version
// @date

#pragma once

#include <typeinfo>
#include <cassert>


namespace Loki11
{

// @class TypeInfo
// @brief 封装std::type_index
// @c++11 使用std::type_index
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

    // @function
    // @brief 判断类型次序
    // @param rhs: 另一个TypeInfo对象
    // @return bool
    bool before(const TypeInfo& rhs) const {
        assert(pInfo_);
        return pInfo_->before(*rhs.pInfo_) != 0;
    };

    // @function
    // @brief 获取类型名称
    // @return const char*
    const char* name() const {
        assert(pInfo_);
        return pInfo_->name();
    };

    // @function
    // @brief 获取所表示的std::type_index
    // @return const std::type_index&
    const std::type_info& Get() const {
        assert(pInfo_);
        return *pInfo_;
    };

private:
    const std::type_info* pInfo_;
};

// @function
// @brief 判断两个TypeInfo是否相等
// @param lhs: TypeInfo对象
// @param rhs: TypeInfo对象
// @return bool
inline bool operator==(const TypeInfo& lhs, const TypeInfo& rhs)
{
    return (lhs.Get() == rhs.Get()) != 0;
}

// @function
// @brief 判断两个TypeInfo是否小于关系
// @param lhs: TypeInfo对象
// @param rhs: TypeInfo对象
// @return bool
inline bool operator<(const TypeInfo& lhs, const TypeInfo& rhs)
{
    return lhs.before(rhs);
}

// @function
// @brief 判断两个TypeInfo是否不相等
// @param lhs: TypeInfo对象
// @param rhs: TypeInfo对象
// @return bool
inline bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs)
{
    return !(lhs == rhs);
}

// @function
// @brief 判断两个TypeInfo是否大于关系
// @param lhs: TypeInfo对象
// @param rhs: TypeInfo对象
// @return bool
inline bool operator>(const TypeInfo& lhs, const TypeInfo& rhs)
{
    return rhs < lhs;
}

// @function
// @brief 判断两个TypeInfo是否小于等于
// @param lhs: TypeInfo对象
// @param rhs: TypeInfo对象
// @return bool
inline bool operator<=(const TypeInfo& lhs, const TypeInfo& rhs)
{
    return !(lhs > rhs);
}

// @function
// @brief 判断两个TypeInfo是否大于等于
// @param lhs: TypeInfo对象
// @param rhs: TypeInfo对象
// @return bool
inline bool operator>=(const TypeInfo& lhs, const TypeInfo& rhs)
{
    return !(lhs < rhs);
}

}
