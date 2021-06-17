#include <iostream>
#include <type_traits>
#include <functional>

#include "detail.hpp"

// PART ONE: All "binary operators" are strictly typed.
//           For example, we will have `std::plus<T>`,
//           which only accepts elements of type `T`,
//           and we know what `T` is.

// This is a binary operator: a functor that accepts two arguments
// and has a return value.
template <typename T>
struct plus_monoid {
  T operator()(const T& a, const T& b) const {
    return a + b;
  }

  // This one so happens to have some other methods and type defs, but these
  // are unnecessary to be a "binary operator."
  static constexpr T identity() {
    return T(0);
  }

  using type = T;
};

// This is also a binary operator
template <typename T>
using plus_stl = std::plus<T>;

// Now, we want to allow both `plus_monoid`, which has an identity method, and
// `plus_stl`, which does not, to be monoids.

// We can create a template struct `monoid_traits` that will be used to
// inspect attributes of a type `T` that is a monoid.
//
// For `std::plus<int>`, for example, we could say:
// int identity = monoid_traits<std::plus<int>>::identity();

template <typename T, typename Enabler = void>
struct monoid_traits;

// For functors that "fit the spec," meaning they have an `identity` method
// and a `type` typedef, this partial specialization will provide the
// `monoid_traits` struct.
template <typename T>
struct monoid_traits<T, std::enable_if_t<has_identity_v_<T>>>
{
  using type = typename T::type;

  static type identity() {
    return T::identity();
  }
};

// For functors that do not fit the spec, we can provide custom partial
// specializations.

template <typename T>
struct monoid_traits<std::plus<T>>
{
  using type = T;

  static constexpr type identity() {
    return type(0);
  }
};

// So `std::plus<T>` has essentially been "promoted" from a binary operator
// to a monoid through this `monoid_traits` partial specialization.
// This is analogous to how `iterator_traits` works with C++ classes that have
// all the appropriate typedefs as well as `T*` and `const T*`.
// cppreference: https://en.cppreference.com/w/cpp/iterator/iterator_traits



// PART TWO: In which the binary operators loose their
//           strict typing, and, with it, my sanity.

// But!! It is very nice to have binary operators that are
// flexible, accepting whatever types you like as input and
// doing the appropriate thing.

// An example of this is `std::plus<void>`, which accepts arguments of
// any type, as long as they have `operator+`, returning the appropriate
// return type.

using plus_nice = std::plus<void>;

// Now we have a problem.  The monoid has no pre-defined type, so we need an
// identity that could return *any* type, instead of a known type `T`.
// My current solution is to have a templated method `identity<T>()`, which
// is kind of ugly, and quite probably not good C++.
//
// This templated method is really my main concern.
//


struct plus_ {
  template <typename T, typename U>
  auto operator()(const T& a, const T& b) const {
    return a + b;
  }

  template <typename T>
  static constexpr T identity() {
    return T(0);
  }

  // using type = get rekt;
};

// We can now create another `monoid_traits_` template struct.
// However, now it gives us properties for the monoid constructed on the
// binary operator `Fn` *and* the type `T`.
//
// e.g., we can say:
// int identity = monoid_traits_<plus_, int>::identity()
//
// i.e. "give me the identity for bin. op. `plus_` for monoid with type `int`.

// (Monoids, by definition work on a single type.)

template <typename Fn, typename T, typename Enabler = void>
struct monoid_traits_;

// For binary operators with the `identity` method, we have one template
// specialization:
template <typename Fn, typename T>
struct monoid_traits_<Fn, T, std::enable_if_t<has_identity_v_<Fn, T>>>
{
  // `T` gives us the monoid's type
  using type = T;

  static T identity() {
    // Calling that raggedy template fn.
    return Fn:: template identity<T>();
  }
};

// Similarly, we can declare a partial specialization for types like `std::plus<T>`

template <typename T>
struct monoid_traits_<std::plus<T>, T>
{
  using type = T;

  static constexpr T identity() {
    // We're expliclty attaching this new identity to the pre-existing op.
    return T(0);
  }
};

int main(int argc, char** argv) {
  // "Tightly typed" identity
  int tight_identity = monoid_traits<std::plus<int>>::identity();

  // "Loosely typed" identity
  int loose_identity = monoid_traits_<plus_, int>::identity();

  std::cout << tight_identity << " " << loose_identity << std::endl;

  return 0;
}
