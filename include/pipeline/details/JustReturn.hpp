#pragma once

/**
   Макрос для создания функций с выводом возвращаемого
   значения.

   Приходится использовать decltype так как
   auto превращает ссылки в значения.
*/
#define JUST_RETURN(...) -> decltype( __VA_ARGS__ ) { return __VA_ARGS__; }
