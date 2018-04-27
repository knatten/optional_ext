#include <optional>

namespace knatten {
    template <class T>
    class optional {
    public:
        //The most necessary constructors for the example
        optional() noexcept = default;
        optional(const optional<T>& rhs) = default;
        optional(optional<T>&& rhs) noexcept = default;
        optional(T val) : o_(std::move(val)) { }

        //Demonstration of the proposed methods

        template <class UnaryOperation>
        constexpr decltype(auto) transform(UnaryOperation op) & {
            using OptionalReturnType = optional<decltype(op(*o_))>;
            return has_value() ?
                OptionalReturnType(op(*o_)) :
                OptionalReturnType();
        }

        //const rvalue will also choose this one
        template <class UnaryOperation>
        constexpr decltype(auto) transform(UnaryOperation op) const& {
            using OptionalReturnType = optional<decltype(op(*o_))>;
            return has_value() ?
                OptionalReturnType(op(*o_)) :
                OptionalReturnType();
        }

        template <class UnaryOperation>
        constexpr decltype(auto) transform(UnaryOperation op) &&{
            using OptionalReturnType = optional<decltype(op(std::move(*o_)))>;
            return has_value() ?
                OptionalReturnType(op(std::move(*o_))) :
                OptionalReturnType();
        }

        template <class UnaryOperation>
        constexpr decltype(auto) transform_opt(UnaryOperation op) & {
            using OptionalReturnType = decltype(op(*o_));
            return has_value() ?
                op(*o_) :
                OptionalReturnType();
        }

        //const rvalue will also choose this one
        template <class UnaryOperation>
        constexpr decltype(auto) transform_opt(UnaryOperation op) const& {
            using OptionalReturnType = decltype(op(*o_));
            return has_value() ?
                op(*o_) :
                OptionalReturnType();
        }

        template <class UnaryOperation>
        constexpr decltype(auto) transform_opt(UnaryOperation op) && {
            using OptionalReturnType = decltype(op(std::move(*o_)));
            return has_value() ?
                op(std::move(*o_)) :
                OptionalReturnType();
        }

        template <class UnaryOperation>
        void execute(UnaryOperation op) & {
            if (has_value()) {
                op(*o_);
            }
        }

        template <class UnaryOperation>
        void execute(UnaryOperation op) const& {
            if (has_value()) {
                op(*o_);
            }
        }

        template <class UnaryOperation>
        void execute(UnaryOperation op) && {
            if (has_value()) {
                op(std::move(*o_));
            }
        }

        template <class UnaryOperation>
        void execute(UnaryOperation op) const&& {
            if (has_value()) {
                op(std::move(*o_));
            }
        }

        // Forward observers
        constexpr bool has_value() const noexcept { return o_.has_value(); }

        constexpr const T& value() const& { return o_.value(); }
        constexpr T& value() & { return o_.value(); }
        constexpr T&& value() && { return std::move(o_).value(); }
        constexpr const T&& value() const&& { return std::move(o_).value(); }

        constexpr const T& operator*() const& { return *o_; }
        constexpr T& operator*() & { return *o_; }
        constexpr const T&& operator*() const&& { return *std::move(o_); }
        constexpr T&& operator*() && { return *std::move(o_); }

        constexpr const T* operator->() const { return o_.operator->(); }
        constexpr T* operator->() { return o_.operator->(); }

    private:
        std::optional<T> o_;
    };
}
