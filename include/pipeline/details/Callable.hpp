/**
   \file

   Тут содержится класс Callable. Он является альтернативой для
   std::function. В отличии от std::function он не стирает типы,
   а наоборот хранит информацию не только о сигнатуре, но и о
   том какой это объект: функциональный объект, метод или функция.

   Callable сам не аллоцирует память и старается использовать семантику
   перемещения. Т.е. он эффективней чем std::function.
   Но аллокация может произойти при копировании внутренних структур.

   Нет смысла создавать объект Callable напрямую, для создания стоит
   использовать функцию function.
*/

#pragma once

#include <pipeline/details/JustReturn.hpp>

#include <utility>
#include <type_traits>

namespace pipeline {

    namespace details {

        /**
           Тег указывающий, что это обёртка для
           функционального объекта(класса или лямбды)
        */
        struct CallableFunctor;
        /**
           Тег указывающий, что это обёртка для
           метода
        */
        struct CallableMethod;
        /**
           Тег указывающий, что это обёртка для
           функции
        */
        struct CallableFunction;

        /**
           Обертка для вызова функционального объекта, метода
           класса или функции.

           \tparam Type тег указывающий на тип вызова.
           Например: CallableFunctor
           \tparam Klass имя класса. Если не используется,
           то void
           \tparam Ret возвращаемый тип. Если неизвестен,
           то void
           \tparam Args типы передаваемых аргументов. Если
           неизвестны, то там один тип -- void
        */
        template <class Type,
                  class Klass,
                  class Ret,
                  class... Args>
        class Callable;

        /**
           Обёртка для функционального объекта(класса или лямбды).

           Возвращаемый тип и аргументы неизвестны так как
           функциональный объект может иметь несколько методов
           которые можно вызвать.
        */
        template <class Klass>
        class Callable<CallableFunctor, Klass, void, void> {
            Klass m_klass;
        public:
            explicit Callable(Klass&& klass)
                : m_klass(std::forward<Klass>(klass)) {}

            template <class... TArgs>
            auto operator()(TArgs&&... args) const
                JUST_RETURN(
                    m_klass(std::forward<TArgs>(args)...)
                    );

            template <class... TArgs>
            auto operator()(TArgs&&... args)
                JUST_RETURN(
                    m_klass(std::forward<TArgs>(args)...)
                    );
        };

        /**
           Обёртка для метода класса.

           Указатель на сам объект класса не хранится в Callable;
           ссылку на объект нужно передать первым аргументом при
           вызове.

           Если метод с атрибутом const, то Klass имеет атрибут
           const, если метод не const, то и Klass не const.
        */
        template <class Klass,
                  class Ret,
                  class... Args>
        class Callable<CallableMethod, Klass, Ret, Args...> {
            using MethodPtr = std::conditional_t<std::is_const<Klass>::value,
                                                 Ret(Klass::*)(Args...) const,
                                                 Ret(Klass::*)(Args...)>;
            MethodPtr m_method;
        public:
            explicit Callable(MethodPtr method)
                : m_method(method) {}

            /**
               Вызов константного метода с lvalue klass
            */
            template <class... TArgs>
            auto operator()(Klass& klass, TArgs&&... args) const
                JUST_RETURN(
                    (klass.*m_method)(std::forward<TArgs>(args)...)
                    );

            /**
               Вызов не константного метода с lvalue klass
            */
            template <class... TArgs>
            auto operator()(Klass& klass, TArgs&&... args)
                JUST_RETURN(
                    (klass.*m_method)(std::forward<TArgs>(args)...)
                    );

            /**
               Вызов константного метода с rvalue klass
            */
            template <class... TArgs>
            auto operator()(Klass&& klass, TArgs&&... args) const
                JUST_RETURN(
                    (klass.*m_method)(std::forward<TArgs>(args)...)
                    );

            /**
               Вызов не константного метода с rvalue klass
            */
            template <class... TArgs>
            auto operator()(Klass&& klass, TArgs&&... args)
                JUST_RETURN(
                    (klass.*m_method)(std::forward<TArgs>(args)...)
                    );
        };

        /**
           Обёртка для функции.
        */
        template <class Ret,
                  class... Args>
        class Callable<CallableFunction, void, Ret, Args...> {
            using FunctionPtr = Ret(*)(Args...);
            FunctionPtr m_function;
        public:
            explicit Callable(FunctionPtr function)
                : m_function(function) {}

            template <class... TArgs>
            auto operator()(TArgs&&... args) const
                JUST_RETURN(
                    m_function(std::forward<TArgs>(args)...)
                    );

            template <class... TArgs>
            auto operator()(TArgs&&... args)
                JUST_RETURN(
                    m_function(std::forward<TArgs>(args)...)
                    );
        };

        /**
           Функция для создания Callable для функционального
           объекта(класса или лямбды)
        */
        template <class Klass>
        auto function(Klass&& t) {
            return Callable<CallableFunctor,
                            std::remove_reference_t<Klass>,
                            void, void>(std::forward<Klass>(t));
        }

        /**
           Функция для создания Callable для метода класса.
           Используется для метода без атрибута const.
        */
        template <class Ret, class Klass, class... Args>
        auto function(Ret(Klass::*t)(Args...)) {
            return Callable<CallableMethod, Klass, Ret, Args...>(t);
        }

        /**
           Функция для создания Callable для метода класса.
           Используется для метода с атрибутом const.
        */
        template <class Ret, class Klass, class... Args>
        auto function(Ret(Klass::*t)(Args...) const) {
            return Callable<CallableMethod, const Klass, Ret, Args...>(t);
        }

        /**
           Функция для создания Callable для функции
        */
        template <class Ret, class... Args>
        auto function(Ret(*t)(Args...)) {
            return Callable<CallableFunction, void, Ret, Args...>(t);
        }

    } /* namespace details */

} /* namespace pipeline */
