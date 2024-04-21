#pragma once

#include <memory>

#include "data/facilities/device.hpp"
#include "data/facilities/category.hpp"

namespace JYNN{

template <class Category, class Item, class Device=Device::CPU>
class DynamicBase {
protected:
    using EvalType = PrincipalDataType<Category, Item, Device>;
public:
    virtual ~DynamicBase() = default;
    virtual const Shape<Category::dim_>& GetShape() const = 0;
    virtual bool operator== (const DynamicBase& other) const = 0;
    virtual const EvalType& Value() const = 0;
};

template <class InnerData>
class DynamicWrapper : public DynamicBase<typename InnerData::CategoryTag, 
                                          typename InnerData::ItemType,
                                          typename InnerData::DeviceType>{
    using Base = DynamicBase<typename InnerData::CategoryTag,
                              typename InnerData::ItemType,
                              typename InnerData::DeviceType>;
private:
    InnerData innerData_;

public:
    DynamicWrapper(const InnerData& data) :
        innerData_(std::move(data))
    {}


    const Shape<InnerData::CategoryTag::dim_>& GetShape() const override final {
        return innerData_.GetShape();
    }
    bool operator== (const Base& other) const override final {
        try{
            const auto& dWrapper = dynamic_cast<const DynamicWrapper&>(other);
            return innerData_ == dWrapper.innerData_;
        }catch (std::bad_cast&){
            return false;
        }
    }
    const InnerData& Value() const override final {
        return innerData_;
    }
};

template <class Category, class Item, class Device = Device::CPU>
class DynamicData {

    using Base = DynamicBase<Category, Item, Device>;
private:
    std::shared_ptr<Base> innerDataWrapper_ { nullptr };

public:
    using CategoryTag = Category;
    using ItemType = Item;
    using DeviceType = Device;

public:
    template <class InnerData>
    DynamicData(const std::shared_ptr<DynamicWrapper<InnerData>>& data) : 
        innerDataWrapper_(std::move(data))
    {}

    const auto GetShape() const {
        assert(innerDataWrapper_ != nullptr);
        return innerDataWrapper_->GetShape();
    }

    bool operator== (const DynamicData& other) const {
        if(!innerDataWrapper_ ) {
            return nullptr == other.innerDataWrapper_;
        }
        if(!other.innerDataWrapper_) {
            return nullptr == innerDataWrapper_;
        }
        return *innerDataWrapper_ == *other.innerDataWrapper_;
    }
    bool Empty() const {
        return innerDataWrapper_ == nullptr;
    }
    const auto& Value() const {
        return *innerDataWrapper_;
    }
};

template <class Data>
struct IsDynamic_ : std::false_type {};
template <class Category, class Item, class Device>
struct IsDynamic_<DynamicData<Category, Item, Device>> : std::true_type {};
template <class Data>
constexpr bool isDynamic = IsDynamic_< RmConstRef<Data> >::value;


template <class Data>
auto MakeDynamic(Data&& data) {
    if constexpr ( isDynamic<Data> ) {
        return std::forward<Data> (data);
    } else{
        using RawData = RmConstRef<Data>;
        using DWrapper = DynamicWrapper< RawData >;
        return DynamicData< DataCategory<RawData>,
                            typename RawData::ItemType,
                            typename RawData::DeviceType>
        ( std::make_shared<DWrapper>(std::forward<Data>(data)) );
    }
}



};