#pragma once

#include <pipeline/details/Callable.hpp>
#include <pipeline/details/JustReturn.hpp>
#include <pipeline/details/Namespaces.hpp>

namespace pipeline {

    namespace details {

        /**
           PipeOp это функциональный объект который
           участвует в соединении через pipe'ы.

           Его особенностью является то, что он
           обязательно принимает ровно один аргумент.
        */
        template <class Func>
        struct PipeOp final {
            Func m_func;

            explicit PipeOp(Func func)
                : m_func(std::move(func)) {}

            template <class TArg>
            auto operator()(TArg&& arg) const
                JUST_RETURN(
                    m_func(std::forward<TArg>(arg))
                    );

            template <class TArg>
            auto operator()(TArg&& arg)
                JUST_RETURN(
                    m_func(std::forward<TArg>(arg))
                    );
        };

        /**
           Эта фукнция создаёт PipeOp из переданной функции,
           функционального объекта(класса или лямбды) или метода.
        */
        template <class Func>
        auto pipe_op(Func&& func) {
            // так как std::function использовать дорого
            // мы использует тут класс Callable
            auto callable = pd::function(std::forward<Func>(func));
            return PipeOp<decltype(callable)>(std::move(callable));
        }

        /**
           Этот пайп используется для передачи объекта
           в не константный PipeOp.

           Передаваемый объект может быть константным,
           это корректно обработается так как T -- универсальная
           ссылка.
        */
        template <class T, class Callable>
        auto operator|(T&& t, PipeOp<Callable>& op)
            JUST_RETURN(
                op(std::forward<T>(t))
                );

        /**
           Этот пайп используется для передачи объекта
           в константный PipeOp.

           Передаваемый объект может быть константным,
           это корректно обработается так как T -- универсальная
           ссылка.

           Для константного PipeOp нужен отдельный метод так
           как он сам не передаётся по универсальной ссылке
           так как сам PipeOp не шаблонный параметр.
        */
        template <class T, class Callable>
        auto operator|(T&& t, const PipeOp<Callable>& op)
            JUST_RETURN(
                op(std::forward<T>(t))
                );

        /**
           Этот пайп используется для передачи объекта
           в rvalue PipeOp.

           Передаваемый объект может быть константным,
           это корректно обработается так как T -- универсальная
           ссылка.
        */
        template <class T, class Callable>
        auto operator|(T&& t, PipeOp<Callable>&& op)
            JUST_RETURN(
                op(std::forward<T>(t))
                );

    } /* namespace details */

} /* namespace pipeline */
