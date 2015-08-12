#pragma once

#include <pipeline/details/Bind.hpp>
#include <pipeline/details/Callable.hpp>
#include <pipeline/details/Namespaces.hpp>
#include <pipeline/details/PipeOp.hpp>

namespace pipeline {

    namespace details {

        /**
           Фабрика по производству PipeOp.

           Если функциональный объект(класс или лямбда),
           функция или метод принимают больше одного аргумента,
           то нам нужна возможность передать эти аргументы.
           Но pip'ы могут передавать только один аргумент и
           для решения данной проблемы используется PipeOpFactory.

           \warning
           Некоторые решения(например Fit) позволяют использовать
           в качестве фабрики объекты участвующие в pipeline. В
           данных решения используется механизм схожий с
           карированием(carrying) и при передаче не всех аргументов
           мы получаем новый функциональных объект. Из-за этого
           код становится менее устойчивым к ошибкам, и если разработчик
           забудет написать аргумент, то вместо выполнения кода
           pipeline вернёт функциональный объект. Такие ошибки сложно
           искать, они раздражают. Кто хочет, тот пусть попробует
           сделать что-то использую Fit. В данной библиотеке
           в pipeline участвуют только PipeOp, а фабрики не участвуют.
        */
        template <class Func>
        struct PipeOpFactory final {
            Func m_func;

            explicit PipeOpFactory(Func func)
                : m_func(std::move(func)) {}

            template <class... TArgs>
            auto operator()(TArgs&&... args) const {
                // вместо std::bind используется bind как легковесная
                // альтернатива
                return pipe_op(pd::bind(m_func, std::forward<TArgs>(args)...));
            }

            template <class... TArgs>
            auto operator()(TArgs&&... args) {
                return pipe_op(pd::bind(m_func, std::forward<TArgs>(args)...));
            }
        };

        /**
           Эта фукнция создаёт PipeOpFactory из переданной функции,
           функционального объекта(класса или лямбды) или метода.
        */
        template <class Func>
        auto pipe_op_factory(Func&& func) {
            auto callable = pd::function(std::forward<Func>(func));
            return PipeOpFactory<decltype(callable)>(std::move(callable));
        }

    } /* namespace details */

} /* namespace pipeline */
