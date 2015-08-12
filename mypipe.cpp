/**
   Пример использования pipe'ов на fit.

   О самой библиотеке: она очень капризная. Проблема
   в том, что если будет ошибка типов, то она не
   кидает ошибку компиляции, а возвращает свой внутренний
   тип. Из-за этого очень сложно искать ошибки.

   Сама библиотека не плохая по скорости, но из-за
   склонности к ошибкам её лучше не использовать.
*/


#include <iostream>
// #include "pipable.h"
#include <functional>
#include <tuple>


/////////////////////////////////

struct CallableFunctor;
struct CallableMethod;
struct CallableFunction;

template <class Type,
          class Klass,
          class Ret,
          class... Args>
struct Callable;

template <class Klass>
struct Callable<CallableFunctor, Klass, void, void> {
    Klass m_klass;

    Callable(Klass&& klass)
        : m_klass(std::forward<Klass>(klass)) {}

    template <class... TArgs>
    auto operator()(TArgs... args) const {
        return m_klass(std::forward<TArgs>(args)...);
    }
};

template <class Klass,
          class Ret,
          class... Args>
struct Callable<CallableMethod, Klass, Ret, Args...> {
    using MethodPtr = Ret(Klass::*)(Args...);
    MethodPtr m_method;

    Callable(MethodPtr method)
        : m_method(method) {}

    template <class... TArgs>
    auto operator()(Klass& klass, TArgs... args) const {
        return (klass.*m_method)(std::forward<TArgs>(args)...);
    }
};

template <class Ret,
          class... Args>
struct Callable<CallableFunction, void, Ret, Args...> {
    using FunctionPtr = Ret(*)(Args...);
    FunctionPtr m_function;

    Callable(FunctionPtr function)
        : m_function(function) {}

    template <class... TArgs>
    auto operator()(TArgs... args) const {
        return m_function(std::forward<TArgs>(args)...);
    }
};

/**
   Функциональный объект(класс или лямбда)
*/
template <class Klass>
auto function(Klass&& t) {
    return Callable<CallableFunctor, Klass, void, void>(std::forward<Klass>(t));
}

/**
   Метод класса
*/
template <class Ret, class Klass, class... Args>
auto function(Ret(Klass::*t)(Args...)) {
    return Callable<CallableMethod, Klass, Ret, Args...>(t);
}

/**
   Функция
*/
template <class Ret, class... Args>
auto function(Ret(*t)(Args...)) {
    return Callable<CallableFunction, void, Ret, Args...>(t);
}

//////////////////////////////////////

template <class Ret,
          class Arg,
          class... Args>
struct Bind final {
    std::function<Ret(Arg, Args...)> m_op;
    std::tuple<Args...> m_args;

    explicit Bind(std::function<Ret(Arg, Args...)> op,
                  Args&&... args)
        : m_op(std::move(op)),
          m_args(std::forward<Args>(args)...) {}

    Ret operator()(Arg&& arg) {
        return m_op(std::forward<Arg>(arg),
                    std::get<Args>(std::move(m_args))...);
    }
};

//////////////////////////////////////

template <class Ret, class Arg>
struct PipeOp final {
    std::function<Ret(Arg)> m_op;

    explicit PipeOp(std::function<Ret(Arg)> op)
        : m_op(std::move(op)) {}

    template <class TArg>
    auto operator()(TArg&& arg) {
        return m_op(std::forward<TArg>(arg));
    }
};

template <class Ret, class Arg>
auto make_pipe_op(std::function<Ret(Arg)> op) {
    return PipeOp<Ret, Arg>(std::move(op));
}

template <class Ret, class Arg, class... Args>
struct PipeOpFactory final {
    std::function<Ret(Arg, Args...)> m_op;

    explicit PipeOpFactory(std::function<Ret(Arg, Args...)> op)
        : m_op(std::move(op)) {}

    template <class... TArgs>
    auto operator()(TArgs&&... args) {
        using namespace std::placeholders;
        // return make_pipe_op<Ret, Arg>(std::bind(m_op, _1, std::forward<TArgs>(args)...));
        return make_pipe_op<Ret, Arg>(Bind<Ret, Arg, Args...>(m_op, std::forward<TArgs>(args)...));
    }
};

template <class Ret, class... Args>
auto make_pipe_op_factory(std::function<Ret(Args...)> op) {
    return PipeOpFactory<Ret, Args...>(std::move(op));
}

template <class T, class Ret, class Arg>
auto operator|(T&& t, PipeOp<Ret, Arg>& op) {
    return op(std::forward<T>(t));
}

template <class T, class Ret, class Arg>
auto operator|(T&& t, PipeOp<Ret, Arg>&& op) {
    return op(std::forward<T>(t));
}

/////////////////////////////////////////////////////

struct Data {
    int m_data = 1;

    Data& inc(int num = 1) {
        m_data += num;
        return *this;
    }
};

void print(const Data& d) {
    printf("%d\n", d.m_data);
}


int main(int argc, char *argv[]) {
    Data d;
    d.m_data = 3;

    using namespace std::placeholders;

    auto f1 = make_pipe_op(std::function<void(const Data&)>(print));
    auto meth1 = make_pipe_op_factory(std::function<Data&(Data&, int)>(std::bind(&Data::inc, _1, _2)));

    // d | meth1 | f1; // ошибка компиляции
    d | meth1(1) | f1;
    f1(d);

    function(d);
    function(&Data::inc)(d, 5);
    function(print)(d);
    function([](int) {})(1);

    /*
    using namespace std::placeholders;

    auto f1 = fit::pipable([](const Data& d) { print(d); });
    auto meth1 = fit::pipable([](Data& d, int num) { return d.inc(num); });

    d | meth1(1) | f1;

    /// Лажа в fit. Этот код аналогичен f1(d). И он компилируется,
    /// а должна быть ошибка компиляции.
    // (d | meth1 | f1)(d);
    (d | meth1 | f1)(d);

    /// Лажа в fit. Этот код аналогичен f1(d). И он компилируется,
    /// а должна быть ошибка компиляции.
    // (meth1 | f1)(d);
    */
    return 0;
}
