#define EnumTypePolicyObj(PolicyName, Ma, ValName, Val) \
struct PolicyName : virtual public Ma {                 \
  using MinorClass = Ma::ValName##TypeCate ;            \
  using ValName = MinorClass::Val;                      \
};

#define TypePolicyObj(PolicyName, Ma, ValName, Val)     \
struct PolicyName : virtual public Ma {                 \
  using MinorClass = Ma::ValName##TypeCate ;            \
  using ValName = Val;                                  \
};

#define ValuePolicyObj(PolicyName, Ma, ValName, Val)    \
struct PolicyName : virtual public Ma {                 \
  using MinorClass = Ma::ValName##ValueCate ;           \
private:                                                \
  using tmp = RmConstRef<decltype(Ma::ValName)>;        \
public:                                                 \
  static constexpr tmp ValName = static_cast<tmp>(Val); \
};


#define TypePolicyTemplate(PolicyName, Ma, ValName)     \
template <class T>                                      \
struct PolicyName : virtual public Ma {                 \
  using MinorClass = Ma::ValName##TypeCate ;            \
  using ValName = T;                                    \
};

#define ValuePolicyTemplate(PolicyName, Ma, ValName)    \
template <RmConstRef<decltype(Ma::ValName)> T>          \
struct PolicyName : virtual public Ma {                 \
  using MinorClass = Ma::ValName##ValueCate ;           \
private:                                                \
  using tmp = RmConstRef<decltype(Ma::ValName)>;        \
public:                                                 \
  static constexpr tmp ValName = T;                     \
};



