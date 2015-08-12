#pragma once

namespace pipeline {

    namespace details {

        /**
           Этот костыль нужен для использования при вызовах некоторых функций.

           Пример: pd::bind

           Из-за того, что некоторые классы могут находиться в namespace'е
           с std::bind, например тип возвращаемый std::ref, то у bind приходится
           явно указывать namespace.
        */
        namespace pd = pipeline::details;

    } /* namespace details */

} /* namespace pipeline */
