A proposal for extending `std::optional` with convenience functions that allow you to skip a whole lot of `if has_value()` boilerplate and do things like:

    auto result = some_optional
        .transform(function1)
        .transform(function2);

See [the proposal](proposal.md) for more details.

Contents:

- [The proposal itself, `proposal.md`](proposal.md)
- [TODO](TODO)
- [Example implementation in a single header file, `optional_ext.h`](optional_ext.h)
- [A demonstration, demo.cpp](demo.cpp)
- [Unit tests, `optional_ext_test.cpp`](optional_ext_test.cpp) (Written in [Catch 2](https://github.com/catchorg/Catch).)

## License
MIT, see [LICENSE.txt](LICENSE.txt)
