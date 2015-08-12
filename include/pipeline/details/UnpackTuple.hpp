#pragma once

#include <pipeline/details/GenSeq.hpp>
#include <pipeline/details/JustReturn.hpp>

#include <tuple>

namespace pipeline {

    namespace details {

        /**
           Извлекает данные из std::tuple и передаёт
           в функцию
        */
        class UnpackTuple {
            template <class... Args,
                      class Func,
                      class Tuple,
                      int... S>
            static
            auto helper(Func&& func,
                        Tuple&& tuple,
                        Seq<S...>,
                        Args&&... args)
                JUST_RETURN(
                    func(std::forward<Args>(args)...,
                         std::get<S>(std::forward<Tuple>(tuple))...)
                    );
        public:
            /**
               Делает вызов: func(args..., tuple...);

               Для rvalue std::tuple.
            */
            template <class... Args,
                      class Func,
                      class... TArgs>
            static
            auto call(Func&& func,
                      std::tuple<TArgs...>&& tuple,
                      Args&&... args)
                JUST_RETURN(
                    helper(std::forward<Func>(func),
                           std::move(tuple),
                           GenSeq_t<sizeof...(TArgs)>(),
                           std::forward<Args>(args)...)
                    );

            /**
               Делает вызов: func(args..., tuple...);

               Для lvalue std::tuple.
            */
            template <class... Args,
                      class Func,
                      class... TArgs>
            static
            auto call(Func&& func,
                      std::tuple<TArgs...>& tuple,
                      Args&&... args)
                JUST_RETURN(
                    helper(std::forward<Func>(func),
                           tuple,
                           GenSeq_t<sizeof...(TArgs)>(),
                           std::forward<Args>(args)...)
                    );
        };

    } /* namespace details */

} /* namespace pipeline */
