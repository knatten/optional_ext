#include <optional>

template <class T>
class optional {
public:
    optional() noexcept = default;

    optional(const optional<T>& rhs) = default;

    optional(optional<T>&& rhs) noexcept : o_(std::move(rhs.o_)) {
    }

    optional(T val) : o_(std::move(val)){
    }

    constexpr const T& value() const {
        return o_.value();
    }

    constexpr bool has_value() const noexcept {
        return o_.has_value();
    }

    template <class UnaryOperation>
    constexpr optional<T> transform(UnaryOperation op) const& {
        return has_value() ?
            optional(op(*o_)) :
            optional();
    }

    template <class UnaryOperation>
    constexpr optional<T> transform(UnaryOperation op) &&{
        return has_value() ?
            optional(op(std::move(*o_))) :
            optional();
    }

private:
    std::optional<T> o_;
};
