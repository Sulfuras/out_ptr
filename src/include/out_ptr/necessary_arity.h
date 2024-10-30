#pragma once

#include "is_specialization_of.h"
#include <memory>
#include <type_traits>

namespace std {
    namespace out_ptr {
        namespace op_detail {
            template< typename Smart, typename... Args >
            class necessary_arity_impl
                    : public std::integral_constant< std::size_t,
                                                     static_cast< std::size_t >(
                                                         is_specialization_of< Smart, std::shared_ptr >::value ) >
            {
            };
        }

        template< typename Smart, typename... Args >
        class necessary_arity : public op_detail::necessary_arity_impl< Smart, Args... >
        {
        };
    }
}
