
#pragma once

namespace JYNN{

struct Sequential{

private:
//声明
    template <size_t N, class Item, class Cont> struct CreateItem;
    template <class Item, class Cont, size_t N=0> struct GetIndex;
    template <size_t N, class Cont> struct AtImp;
    template <size_t N, class Cont, class Item> struct SetImp;
    template <class Cont, class Item> struct PushBack;
    template <class Cont, class Item> struct PushFront;
    template <class ContNew, class Cont, template <class, class> class Fun> struct Fold;
    template <class Cont> struct GetSize;
    template <class Cont, template <class, class> class Fun> struct IsUniqueImp;

public:
// 别名

// 这里的意思是， 放入到 类型为 Cont 的容器里， 所以 要用模板；如果用 class 那就代表 一个 类型 如 int， 他无法改变；
    template <size_t N, class Item, template <class...> class Cont>
    using Create = typename CreateItem<N, Item, Cont<>>::type;
// 这里 Cont 是一个容器类型；
    template <class Item, class Cont>
    constexpr static size_t Index = GetIndex<Item, Cont, 0>::Index_;

    template <size_t N, class Cont>
    using At = typename AtImp<N, Cont>::type;

    template <size_t N, class Cont, class NewItem>
    using Set = typename SetImp<N, Cont, NewItem>::type;

    template <class Cont, class Item>
    using PushBk = typename PushBack<Cont, Item>::type;

    template <class Cont, class Item>
    using PushFrt = typename PushFront<Cont, Item>::type;

// NewCont 是要 放入的容器的类型， Cont 是 已经存在的类型， Fun 是一个 模板函数， 将来要调用；
    template <template <class...> class NewCont, class Cont, template <class, class> class Fun>
    using Fliter = typename Fold<NewCont<>, Cont, Fun>::type;

    template <class Cont>
    constexpr static size_t Size = GetSize<Cont>::size_;

// 判断 Cont 两两 之间 是否满足 Fun
    template <class Cont, template <class, class> class Fun>
    constexpr static bool IsUnique = IsUniqueImp<Cont, Fun>::isUnique_;
private:
// 实现
    // Create Items
    template <class Item, template <class...> class Cont, class... Items> 
    struct CreateItem<0, Item, Cont<Items...>> : 
        std::enable_if<true, Cont<Items...>>
    {};
    template <size_t N, class Item, template <class...> class Cont, class... Items>
    struct CreateItem<N, Item, Cont<Items...>> : 
        CreateItem<N-1, Item, Cont<Item, Items...>>
    {};
    // Get Index
    template <class Item, class Cur, class... Items, template <class...> class Cont, size_t N> 
    struct GetIndex<Item, Cont<Cur, Items...>, N> :
        GetIndex<Item, Cont<Items...>, N+1>  
    {};
    template <class Item, class... Items, template <class...> class Cont, size_t N>
    struct GetIndex<Item, Cont<Item, Items...>, N> {
        constexpr static size_t Index_ = N;
    };
    template <class Item, template <class...> class Cont, size_t N>
    struct GetIndex<Item, Cont<>, N> {
        constexpr static size_t Index_ = N;
    };
    // At
    template <class Cont>
    struct AtImp_;
    template <class... placements, template <class...> class Cont>
    struct AtImp_ <Cont<placements...>>{
        template <class nth>
        static nth apply(
            placements... ,
            nth*,
            ...
        );
    };
    template <size_t N, class... Items, template <class...> class Cont>
    struct AtImp<N, Cont<Items...>> :
        std::enable_if<
            true,
            decltype (AtImp_< Create<N, void*, std::tuple> >::apply((Items*)nullptr...))
        >
    {};
    // Set
    template <size_t N, class LCont, class RCont, class Item>
    struct SetImp_;
    template <size_t N, class Cur, class... L, class... R, template <class...> class LCont, template <class...> class RCont, class NewItem>
    struct SetImp_<N, LCont<L...>, RCont<Cur, R...>, NewItem> :
        SetImp_<N-1, LCont<L..., Cur>, RCont<R...>, NewItem>
    {};
    template <class Cur, class... L, class... R, template <class...> class LCont, template <class...> class RCont, class NewItem>
    struct SetImp_<0, LCont<L...>, RCont<Cur, R...>, NewItem> :
        std::enable_if<
            true,
            LCont<L..., NewItem, R...>
        >
    {};
    template <size_t N, template <class...> class Cont, class... Items, class NewItem>
    struct SetImp<N, Cont<Items...>, NewItem> :
        SetImp_<N, Cont<>, Cont<Items...>, NewItem>
    {};
    // Push
    template <class... Items, template <class...> class Cont, class Item>
    struct PushBack<Cont<Items...>, Item> : 
        std::enable_if<true, Cont<Items..., Item>>
    {};
    template <class... Items, template <class...> class Cont, class Item>
    struct PushFront<Cont<Items...>, Item> : 
        std::enable_if<true, Cont<Item, Items...>>
    {};
    // Fold
    template <class ContNew, template <class...> class Cont, template <class, class> class Fun>
    struct Fold<ContNew, Cont<>, Fun> : 
        std::enable_if<true, ContNew>
    {};
    template <class ContNew, class Cur, class... Items, template<class...> class Cont, template <class, class> class Fun>
    struct Fold<ContNew, Cont<Cur, Items...>, Fun> :
        Fold<typename Fun< ContNew, Cur>::type, Cont<Items...>, Fun >
    {};
    // Size
    template <class... Items, template <class...> class Cont>
    struct GetSize<Cont<Items...>> {
        constexpr static size_t size_ = sizeof...(Items);
    };
    // IsUnique
    template <template <class...> class Cont, template <class, class> class Fun>
    struct IsUniqueImp<Cont<>, Fun>  {
        static constexpr bool isUnique_ = true;
    };
    template <class Cur, template <class...> class Cont, template <class, class> class Fun>
    struct IsUniqueImp<Cont<Cur>, Fun>{
        static constexpr bool isUnique_ = true;
    };
    template <class Cur, class... Items, template <class...> class Cont, template <class, class> class Fun>
    struct IsUniqueImp<Cont<Cur, Items...>, Fun> {
        static constexpr bool CurOK = 
            ( Fun<Cur, Items>::value && ... );
        static constexpr bool isUnique_ = 
            CurOK ? (IsUniqueImp<Cont<Items...>, Fun>::isUnique_) : false;
    };

};


};
