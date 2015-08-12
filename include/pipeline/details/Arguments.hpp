/**
   \file

   Содержит вспомогательные классы и функции.
   С помощью них можно использовать
   в pipeline обычные функции и методы не
   сконвертированые в PipeOp.

   Пример:
   int sum(int a, int b) { return a + b; }

   4 | sum << args(2);
*/

#pragma once

#include <pipeline/details/PipeOpFactory.hpp>
#include <pipeline/details/UnpackTuple.hpp>

#include <tuple>

namespace pipeline {

    namespace details {

        /**
           Эта структура используется для хранения
           аргументов для привязки к функции
        */
        template <class... Args>
        struct Arguments {
            std::tuple<Args...> m_args;

            explicit Arguments(Args&&... args)
                : m_args(std::forward<Args>(args)...) {}
        };

        /**
           Функция для создания Arguments
        */
        template <class... Args>
        auto args(Args&&... args) {
            return Arguments<Args...>(std::forward<Args>(args)...);
        }

        /**
           Этот оператор привязывает ко всем параметрам, кроме первого,
           функционального объекта func значения args
        */
        template <class Func, class... Args>
        auto operator<<(Func&& func, Arguments<Args...>&& args) {
            auto factory = pipe_op_factory(std::forward<Func>(func));
            return UnpackTuple::call(std::move(factory),
                                     std::move(args.m_args));
        }

    } /* namespace details */

} /* namespace pipeline */
