#pragma once

#include "marker.h"
#include "out_ptr_traits.h"
#include "pointer_of.h"
#include "simple_out_ptr.h"
#include <tuple>
#include <type_traits>

namespace std {
    namespace out_ptr {

        namespace op_detail {

            template< typename Smart, typename Pointer, typename... Args >
            using core_out_ptr_t = simple_out_ptr_t< Smart, Pointer, Args... >;
        }

        template< typename Smart, typename Pointer, typename... Args >
        class out_ptr_t : public op_detail::core_out_ptr_t< Smart, Pointer, Args... >
        {
        private:
            using base_t = op_detail::core_out_ptr_t< Smart, Pointer, Args... >;

        public:
            using base_t::base_t;
        };

        namespace op_detail {
            template< typename Pointer, typename Smart, typename... Args >
            out_ptr_t< Smart, Pointer, Args... > out_ptr_tagged( std::false_type, Smart& s, Args&&... args ) noexcept(
                ::std::is_nothrow_constructible< out_ptr_t< Smart, Pointer, Args... >, Smart&, Args... >::value )
            {
                using P = out_ptr_t< Smart, Pointer, Args... >;
                return P( s, std::forward< Args >( args )... );
            }

            template< typename, typename Smart, typename... Args >
            out_ptr_t< Smart, pointer_of_t< Smart >, Args... >
                out_ptr_tagged( std::true_type, Smart& s, Args&&... args ) noexcept(
                    ::std::is_nothrow_constructible< out_ptr_t< Smart, pointer_of_t< Smart >, Args... >,
                                                     Smart&,
                                                     Args... >::value )
            {
                using Pointer = pointer_of_t< Smart >;
                using P = out_ptr_t< Smart, Pointer, Args... >;
                return P( s, std::forward< Args >( args )... );
            }

        }

        template< typename Pointer = op_detail::marker, typename Smart, typename... Args >
        auto out_ptr( Smart& s, Args&&... args ) noexcept( noexcept( op_detail::out_ptr_tagged< Pointer >(
            ::std::is_same< Pointer, op_detail::marker >(), s, std::forward< Args >( args )... ) ) )
            -> decltype( op_detail::out_ptr_tagged< Pointer >( ::std::is_same< Pointer, op_detail::marker >(),
                                                               s,
                                                               std::forward< Args >( args )... ) )
        {
            return op_detail::out_ptr_tagged< Pointer >(
                ::std::is_same< Pointer, op_detail::marker >(), s, std::forward< Args >( args )... );
        }

    }
}
