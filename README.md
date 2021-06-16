# Monoid Traits Options

This repository contains a brief overview of two implementation options for
monoids in the [GraphBLAS C++ API](https://graphblas.github.io/).

The main CPP file (which includes exposition in comments) is in [

In GraphBLAS, as in mathematics generally, monoids are defined as binary operators
with a set type `T` and an identity value `i`.  I would like to allow binary
operators more generally to be able to be ``promoted'' to monoids with an
instantiation of a `monoid_traits` template struct.  This is analogous to how
iterator types are ``promoted'' to iterators with `iterator_traits`.  We can
use partial specializations of `monoid_traits` to promote binary operators such
as `std::plus<T>` to monoids in a similar way to how `T*` and `const T*` are
promoted to iterators with a specialization of `iterator_traits`.

This presents a problem with 
