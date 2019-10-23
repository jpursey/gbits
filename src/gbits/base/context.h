#ifndef GBITS_BASE_CONTEXT_H_
#define GBITS_BASE_CONTEXT_H_

#include "absl/container/flat_hash_map.h"

#include "gbits/base/context_type.h"

namespace gb {

// This class contains a set of values keyed by type.
//
// Only one value of each type may be stored. The values stored in the context
// are never const. The only requirement for value types stored in the context
// is that they have a public destructor.
//
// Context is a move-only type. This class is thread-compatible.
class Context final {
 public:
  Context() = default;
  Context(const Context&) = delete;
  Context(Context&&) = default;
  Context& operator=(const Context&) = delete;
  Context& operator=(Context&&) = default;
  ~Context() { Reset(); }

  // Returns true if there are no values stored in the context.
  bool Empty() const { return values_.empty(); }

  // Resets the context to be empty. All owned values are destructed.
  void Reset();

  // Constructs a new value of the specified type with the provided constructor
  // parameters.
  //
  // Any existing owned value will be destructed and replaced with the new
  // value.
  template <typename Type, class... Args>
  void SetNew(Args&&... args) {
    SetImpl(ContextType::Get<Type>(), new Type(std::forward<Args>(args)...),
            true);
  }

  // Sets the value of the specified Type with the context taking ownership.
  //
  // Any existing owned value will be destructed and replaced with the new
  // value. If the new value is the same as the old value, then no destruction
  // will not occur -- only the ownership may change (from unowned to owned). If
  // a null is passed in, this is equivalent to calling Clear<Type> with no
  // value being stored.
  //
  // Unlike SetNew and SetValue, SetOwned does not require explicit Type
  // specification as it is unambiguous.
  template <typename Type>
  void SetOwned(std::unique_ptr<Type> value) {
    SetImpl(ContextType::Get<Type>(), value.release(), true);
  }

  // Sets the value of the specified Type without the context taking ownership.
  //
  // Any existing owned value will be destructed and replaced with the new
  // value. If the new value is the same as the old value, then no destruction
  // will not occur -- only the ownership may change (from owned to unowned). If
  // a null is passed in, this is equivalent to calling Clear<Type> with no
  // value being stored.
  //
  // Unlike SetNew and SetValue, SetOwned does not require explicit Type
  // specification as it is unambiguous.
  template <typename Type>
  void SetPtr(Type* value) {
    SetImpl(ContextType::Get<Type>(), value, false);
  }

  // Updates the value of the specified Type in the context with the new value.
  //
  // The Type must support copy or move construction. If the Type supports copy
  // or move assignment and there is an existing value in the context, then the
  // corresponding supported assignment will take place. If assignment is not
  // supported but there is an existing owned value, the old value will be
  // destructed and a new value will be copy/move constructed from the provided
  // value.
  //
  // Unlike SetPtr and SetOwned, SetValue always requires explicit Type
  // specification, as implicit specification can easily cause ambiguity and
  // hard to track bugs. This also allows assignment support for other types
  // (assigning a const char* to a std::string, for instance).
  template <
      typename Type, typename OtherType,
      std::enable_if_t<std::is_assignable<std::decay_t<Type>, OtherType>::value,
                       int> = 0>
  void SetValue(OtherType&& value) {
    Type* old_value = GetPtr<Type>();
    if (old_value != nullptr) {
      *old_value = std::forward<OtherType>(value);
    } else {
      SetImpl(ContextType::Get<Type>(),
              new Type(std::forward<OtherType>(value)), true);
    }
  }
  template <typename Type, typename OtherType,
            std::enable_if_t<std::negation<std::is_assignable<
                                 std::decay_t<Type>, OtherType>>::value,
                             int> = 0>
  void SetValue(OtherType&& value) {
    SetImpl(ContextType::Get<Type>(), new Type(std::forward<OtherType>(value)),
            true);
  }

  // Returns a pointer to the stored value of the specified Type if there is
  // one, or null otherwise.
  template <typename Type>
  Type* GetPtr() const {
    auto it = values_.find(ContextType::Get<Type>());
    return it != values_.end() ? static_cast<Type*>(it->second.value) : nullptr;
  }

  // Returns the stored value of the specified Type if there is one, or a
  // default constructed value if there isn't. The Type must support copy
  // construction.
  template <typename Type>
  Type GetValue() const {
    Type* value = GetPtr<Type>();
    if (value != nullptr) {
      return *value;
    }
    return Type{};
  }

  // Returns the stored value of the specified Type if there is one, or the
  // specified default value if there isn't. The Type must support copy
  // construction.
  template <typename Type, typename DefaultType>
  Type GetValue(DefaultType&& default_value) const {
    Type* value = GetPtr<Type>();
    if (value != nullptr) {
      return *value;
    }
    return std::forward<DefaultType>(default_value);
  }

  // Returns true if a value of the specified Type exists in the context.
  //
  // This is equivalent to (GetPtr<Type>() != nullptr)
  template <typename Type>
  bool Exists() const {
    return values_.find(ContextType::Get<Type>()) != values_.end();
  }

  // Returns true if a value of the specified Type exists in the context AND it
  // is owned by the context.
  template <typename Type>
  bool Owned() const {
    auto it = values_.find(ContextType::Get<Type>());
    return it != values_.end() && it->second.owned;
  }

  // Releases ownership of the value of the specified Type to the caller.
  //
  // If the value does not exist or is not owned (Owned<Type>() would return
  // false), then this will return null.
  template <typename Type>
  std::unique_ptr<Type> Release() {
    return std::unique_ptr<Type>(
        static_cast<Type*>(ReleaseImpl(ContextType::Get<Type>())));
  }

  // Clears any value of the specified Type from the context (if one existed).
  //
  // If the value exists and is owned, it will be destructed.
  template <typename Type>
  void Clear() {
    SetImpl(ContextType::Get<Type>(), nullptr, false);
  }

 private:
  struct Value {
    Value() = default;
    void* value = nullptr;
    bool owned = false;
  };

  void SetImpl(ContextType* type, void* new_value, bool owned);
  void* ReleaseImpl(ContextType* type);

  absl::flat_hash_map<ContextType*, Value> values_;
};

}  // namespace gb

#endif  // GBITS_BASE_CONTEXT_H_