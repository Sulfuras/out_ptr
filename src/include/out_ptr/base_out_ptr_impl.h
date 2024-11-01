#pragma once

#include "inout_ptr_traits.h"
#include "integer_sequence.h"
#include "is_specialization_of.h"
#include "necessary_arity.h"
#include "pointer_of.h"
#include "voidpp_op.h"
#include <cstdlib>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

namespace std {
    namespace out_ptr {
        namespace op_detail {

// Has to be done in multiple places because certain compilers ignore this instantiation
#define ZTD_OUT_PTR_SAFETY_ASSERTION()                                                                                 \
    static_assert( sizeof...( Indices ) >=                                                                             \
                       necessary_arity< Smart, typename std::tuple_element< Indices, Base >::type... >::value,         \
                   "out_ptr requires certain arguments to be passed in for use with this type "                        \
                   "(e.g. shared_ptr<T> must pass a deleter in so when reset is called the "                           \
                   "deleter can be properly initialized, otherwise the deleter will be "                               \
                   "defaulted by the shared_ptr<T>::reset() call!)" )

            template< typename Smart, typename Pointer, typename Traits, typename Args, typename List >
            class base_out_ptr_impl;

            template< typename Smart, typename Pointer, typename Traits, typename Base, std::size_t... Indices >
            class base_out_ptr_impl< Smart,
                                     Pointer,
                                     Traits,
                                     Base,
                                     std::out_ptr::op_detail::index_sequence< Indices... > >
                    : public voidpp_op< base_out_ptr_impl< Smart,
                                                           Pointer,
                                                           Traits,
                                                           Base,
                                                           std::out_ptr::op_detail::index_sequence< Indices... > >,
                                        Pointer >
                    , protected Base
            {
            protected:
                using traits_t = Traits;
                using storage = pointer_of_or_t< traits_t, Pointer >;
                Smart* m_smart_ptr;
                storage m_target_ptr;

                ZTD_OUT_PTR_SAFETY_ASSERTION();

                base_out_ptr_impl( Smart& ptr, Base&& args, storage initial ) noexcept
                        : Base( std::move( args ) )
                        , m_smart_ptr( std::addressof( ptr ) )
                        , m_target_ptr( initial )
                {
                    ZTD_OUT_PTR_SAFETY_ASSERTION();
                }

            public:
                base_out_ptr_impl( Smart& ptr, Base&& args ) noexcept
                        : Base( std::move( args ) )
                        , m_smart_ptr( std::addressof( ptr ) )
                        , m_target_ptr(
                              traits_t::construct( ptr, std::get< Indices >( static_cast< Base& >( *this ) )... ) )
                {
                }

                base_out_ptr_impl( base_out_ptr_impl&& right ) noexcept
                        : Base( std::move( right ) )
                        , m_smart_ptr( right.m_smart_ptr )
                        , m_target_ptr( std::move( right.m_target_ptr ) )
                {
                    right.m_smart_ptr = nullptr;
                }
                base_out_ptr_impl& operator=( base_out_ptr_impl&& right ) noexcept
                {
                    static_cast< Base& >( *this ) = std::move( right );
                    this->m_smart_ptr = std::move( right.m_smart_ptr );
                    this->m_target_ptr = std::move( right.m_target_ptr );
                    right.m_smart_ptr = nullptr;
                    return *this;
                }

                operator Pointer*() const noexcept
                {
                    ZTD_OUT_PTR_SAFETY_ASSERTION();
                    using has_get_call = std::integral_constant< bool, has_traits_get_call< traits_t >::value >;
                    return call_traits_get< traits_t >( has_get_call(),
                                                        *const_cast< Smart* >( this->m_smart_ptr ),
                                                        const_cast< storage& >( this->m_target_ptr ) );
                }

                ~base_out_ptr_impl() noexcept(
                    noexcept( traits_t::reset( std::declval< Smart& >(),
                                               std::declval< storage& >(),
                                               std::get< Indices >( std::move( std::declval< Base& >() ) )... ) ) )
                {
                    ZTD_OUT_PTR_SAFETY_ASSERTION();
                    if( this->m_smart_ptr ) {
                        Base&& args = std::move( static_cast< Base& >( *this ) );
                        ( void ) args;   // unused if "Indices" is empty
                        traits_t::reset(
                            *this->m_smart_ptr, this->m_target_ptr, std::get< Indices >( std::move( args ) )... );
                    }
                }
            };

#undef ZTD_OUT_PTR_SAFETY_ASSERTION

        }
    }
}
