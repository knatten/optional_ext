#include <optional>

template <typename T>
class optional {
public:
    optional() {
    }

    optional(T val) : o_(val) {
    }

    constexpr const T& value() const {
        return o_.value();
    }

    constexpr bool has_value() const noexcept {
        return o_.has_value();
    }

    template <typename Func>
    constexpr decltype(auto) transform(Func f) const& {
        return has_value() ?
            optional(f(*o_)) :
            optional();
    }

    template <typename Func>
    constexpr decltype(auto) transform(Func f) &&{
        return has_value() ?
            optional(f(std::move(*o_))) :
            optional();
    }

private:
    std::optional<T> o_;
};
