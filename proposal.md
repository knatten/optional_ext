A proposal to add convenience functions to `std::optional`

Anders Schau Knatten (anders@knatten.org)

## Introduction
This proposal adds three convenience functions to `std::optional`. They all aim to eliminate a lot of `if (x.has_value())` conditionals in user code. The interface is based on Rust's `Optional` type, which is a core and very popular type in Rust.

The proposal requires no changes to core language, and breaks no existing code. The target is programmers at all levels, from novices to experts.

*WARNING:* This is a work in progress. In particular, I'm not happy with the name `transform_optional`.

A reference implementation with unit tests and demonstration code can be found at https://github.com/knatten/optional_proposal. That is also the current home of this proposal.

## Motivation and Scope
Calling a function on an optional object currently requires the user to first check whether the object has a value. This proposal extends `std::optional` with three functions that eliminates this need.

Imagine user code integrating with Twitter. It has a types `tweet` and `author`, and the following functions:

    optional<tweet> find_first(const string& search_string);
    author lookup_author(const tweet& t);

If we want to search for a tweet, and get the author if a tweet was found, we currently have to do:

    auto foo = find_first("foo");
    auto foo_author(foo.has_value() ? lookup_author(*foo) : optional<author>());

The first function added by this proposal is `std::optional::transform()` (corresponding to Rust's `map()`), which takes a unary operation `T->U` and returns an new `optional<U>` that:

- If the original `optional<T>` was empty, is empty
- If the original `optional<T>` object had a value, calls the unary operation on that value, and returns an `optional<U>` containing the resulting value

It's easier to demonstrate in code:

    auto foo_author = find_first("foo").transform(lookup_author);

These chain nicely, so you can do this:

    auto result = some_optional
        .transform(function1)
        .transform(function2)
        .transform([](const T& t){/**/});

The proposal also contains two more functions, which are discussed in more detail below

- `transform_optional()`, which is like `transform` but takes an operation `T->optional<U>` instead.
- `call()`, which takes an operation `T->U` which, if there is a current value, calls the operation with that value. The return value `U` is ignored, and `U` can be `void`.

## Impact On the Standard
This does not depend on anything else than C++17 `std::optional`, and can be implemented in the current standard.

## Design decisions

### Things I didn't do
Following is a list of things I either considered doing, or that were suggested to me, which were discarded.

Merge `transform` and `transform_optional` into one function `transform`, that wraps the result of `op` into an `optional` only if `op` doesn't already return an optional. While this could be convenient, it results in `transform` not having a consistent type. It would make the type of `transform` be `(T->U)->optional<U>` in most cases, but `(T->U)->U` when `U` is itself `optional`.

Make `transform` conditionally `noexcept`. This is not possible since `optional(U&& v)` is not `noexcept`.

Make `transform_optional` conditionally `noexcept`. This is actually possible for the rvalue overloads, as `optional`'s move constructor is `noexcept` when `T`'s move constructor is `noexcept`. However, for `transform_optional`, `op` is itself expected to construct an `optional`, meaning that `op` would rarely be `noexcept`.

## Technical Specification

### `std::optional::transform`

    template <class UnaryOperation>
    constexpr optional<U> transform(UnaryOperation op) &

    template <class UnaryOperation>
    constexpr optional<U> transform(UnaryOperation op) const&

*`op` is a function `T->U`*

*Returns: `optional<U>(op(*val))` if `*this` has a value, otherwise `optional<U>()`*

    template <class UnaryOperation>
    constexpr optional<U> transform(UnaryOperation op) &&

    template <class UnaryOperation>
    constexpr optional<U> transform(UnaryOperation op) const&&

*`op` is a function `T->U`*

*Returns: `optional<U>(op(std::move(*val)))` if `*this` has a value, otherwise `optional<U>()`*

### `std::optional::transform_optional`

    template <class UnaryOperation>
    constexpr optional<U> transform_optional(UnaryOperation op) &

    template <class UnaryOperation>
    constexpr optional<U> transform_optional(UnaryOperation op) const&

*`op` is a function `T->optional<U>`*

*Returns: `op(*val)` if `*this` has a value, otherwise `optional<U>()`*

    template <class UnaryOperation>
    constexpr optional<U> transform_optional(UnaryOperation op) &&

    template <class UnaryOperation>
    constexpr optional<U> transform_optional(UnaryOperation op) const&&

*`op` is a function `T->optional<U>`*

*Returns: `op(std::move(*val))` if `*this` has a value, otherwise `optional<U>()`*

### `std::optional::call`

    template <class UnaryOperation>
    void call(UnaryOperation op) &

    template <class UnaryOperation>
    void call(UnaryOperation op) const&

*`op` is a function `T->U`*

*Calls `op(*val)` if `*this` has a value.

    template <class UnaryOperation>
    void call(UnaryOperation op) &&

    template <class UnaryOperation>
    void call(UnaryOperation op) const&&

*`op` is a function `T->U`*

*Calls `op(std::move(*val))` if `*this` has a value.
