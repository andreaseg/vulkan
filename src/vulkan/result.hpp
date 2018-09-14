#ifndef RESULT_HPP
#define RESULT_HPP
#include <variant>
#include <functional>

template<typename R, typename E>
class Result {
    std::variant<R, E> inner;

    public:

    Result(R ok) : inner(ok) {}

    Result(E err) : inner(err) {}

    inline bool is_ok() {
        return std::holds_alternative<R>(inner);
    }

    inline E err() {
        return std::get<E>(inner);
    }

    inline R unwrap() {
        return std::get<R>(inner);
    }

    inline R unwrap_or(R fallback) {
        return is_ok ? std::get<R>(inner) : fallback;
    }
};

#endif // RESULT_HPP