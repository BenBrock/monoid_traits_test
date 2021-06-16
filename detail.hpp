#pragma once

#include <utility>

// Check whether binary operator `T` has
// an identity and a `type` typedef.
template <typename T>
struct has_identity {
  template <typename U>
	static constexpr
	decltype(U::identity(), bool())
	test_identity(int) {
	  return true;
	}

  template <typename U>
  static constexpr
  decltype(std::declval<U::type>(), bool())
  test_type(int) {
    return true;
  }

	template <typename U>
	static constexpr bool test_identity(...) {
	  return false;
	}

	template <typename U>
	static constexpr bool test_type(...) {
	  return false;
	}

	static constexpr bool value = test_identity<T>(int()) && test_type<T>(int());
};

// Check whether binary operator `T` has
// an identity.
template <typename T>
inline constexpr bool has_identity_v = has_identity<T>::value;

// Check whether binary operator `Fn` has
// an identity for the type `T`
template <typename Fn, typename T>
struct has_identity_ {
  template <typename U, typename F>
	static constexpr
	decltype(U:: template identity<F>(), bool())
	test_identity(int) {
	  return true;
	}

	template <typename U, typename F>
	static constexpr bool test_identity(...) {
	  return false;
	}

	static constexpr bool value = test_identity<Fn, T>(int());
};

// Check whether binary operator `T` has
// an identity.
template <typename Fn, typename T>
inline constexpr bool has_identity_v_ = has_identity_<Fn, T>::value;
