#pragma once

#include "inout_ptr_traits.h"
#include "marker.h"
#include "simple_inout_ptr.h"
#include <cstddef>

namespace std {
    namespace out_ptr {

        namespace op_detail {
            template< typename Smart, typename Pointer, typename... Args >
            using core_inout_ptr_t = simple_inout_ptr_t< Smart, Pointer, Args... >;
        }   // namespace op_detail

        template< typename Smart, typename Pointer, typename... Args >
        class inout_ptr_t : public op_detail::core_inout_ptr_t< Smart, Pointer, Args... >
        {
        private:
            using base_t = op_detail::core_inout_ptr_t< Smart, Pointer, Args... >;

        public:
            using base_t::base_t;
        };

        namespace op_detail {
            template< typename Pointer, typename Smart, typename... Args >
            inout_ptr_t< Smart, Pointer, Args... >
                inout_ptr_tagged( std::false_type, Smart& s, Args&&... args ) noexcept(
                    ::std::is_nothrow_constructible< inout_ptr_t< Smart, Pointer, Args... >, Smart&, Args... >::value )
            {
                using P = inout_ptr_t< Smart, Pointer, Args... >;
                return P( s, std::forward< Args >( args )... );
            }

            template< typename, typename Smart, typename... Args >
            inout_ptr_t< Smart, pointer_of_t< Smart >, Args... >
                inout_ptr_tagged( std::true_type, Smart& s, Args&&... args ) noexcept(
                    ::std::is_nothrow_constructible< inout_ptr_t< Smart, pointer_of_t< Smart >, Args... >,
                                                     Smart&,
                                                     Args... >::value )
            {
                using Pointer = pointer_of_t< Smart >;
                using P = inout_ptr_t< Smart, Pointer, Args... >;
                return P( s, std::forward< Args >( args )... );
            }

        }   // namespace op_detail

        template< typename Pointer = op_detail::marker, typename Smart, typename... Args >
        auto inout_ptr( Smart& s, Args&&... args ) noexcept( noexcept( op_detail::inout_ptr_tagged< Pointer >(
            ::std::is_same< Pointer, op_detail::marker >(), s, std::forward< Args >( args )... ) ) )
            -> decltype( op_detail::inout_ptr_tagged< Pointer >( ::std::is_same< Pointer, op_detail::marker >(),
                                                                 s,
                                                                 std::forward< Args >( args )... ) )
        {
            return op_detail::inout_ptr_tagged< Pointer >(
                ::std::is_same< Pointer, op_detail::marker >(), s, std::forward< Args >( args )... );
        }

    }
}
