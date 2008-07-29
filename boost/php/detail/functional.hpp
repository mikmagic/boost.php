//
// Copyright 2008 (C) Moriyoshi Koizumi. All rights reserved.
//
// This software is distributed under the Boost Software License, Version 1.0.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

// heavily inspired by get_signature() of Boost.Python, which is the
// really excellent work.

#ifndef MOZO_PHP_DETAIL_FUNCTIONAL_HPP
#define MOZO_PHP_DETAIL_FUNCTIONAL_HPP

#include <cstddef>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/config/limits.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

namespace boost { namespace php { namespace detail {

struct constructor_mark {};

#define __MOZO_PHP_MPL_VECTOR_N(__arity__) \
    BOOST_PP_CAT(::boost::mpl::vector, __arity__)

#define __MOZO_PHP_FUNCTIONAL_VECTOR_TPL(__z__, __arity__, __var__) \
    __MOZO_PHP_MPL_VECTOR_N(__arity__)< BOOST_PP_ENUM_PARAMS_Z(__z__, __arity__, __var__) >

#define __MOZO_PHP_FUNCTIONAL_FUN_TYPE_M(__z__, __arity__, __arg_and_name__) \
    Tretval_(Tobj_::*BOOST_PP_TUPLE_ELEM(2, 1, __arg_and_name__))( \
        BOOST_PP_ENUM_PARAMS_Z(__z__, __arity__, \
            BOOST_PP_TUPLE_ELEM(2, 0, __arg_and_name__)))

#define __MOZO_PHP_FUNCTIONAL_FUN_TYPE_F(__z__, __arity__, __arg_and_name__) \
    Tretval_(*BOOST_PP_TUPLE_ELEM(2, 1, __arg_and_name__))( \
        BOOST_PP_ENUM_PARAMS_Z(__z__, __arity__, \
            BOOST_PP_TUPLE_ELEM(2, 0, __arg_and_name__)))

#define __MOZO_PHP_FUNCTIONAL_TPL_M(__z__, __arity__, __var__) \
    template<typename Tretval_, typename Tobj_ \
             BOOST_PP_ENUM_TRAILING_PARAMS_Z(__z__, __arity__, typename Targ) > \
    struct unbound_function<Tretval_, Tobj_, \
            __MOZO_PHP_FUNCTIONAL_VECTOR_TPL(__z__, __arity__, Targ)> { \
        typedef Tretval_ result_type; \
        typedef Tobj_ object_type; \
        typedef __MOZO_PHP_FUNCTIONAL_VECTOR_TPL(__z__, __arity__, Targ) \
                arguments; \
        typedef __MOZO_PHP_FUNCTIONAL_FUN_TYPE_M(__z__, __arity__, (Targ, impl_ptr_type)); \
        unbound_function(impl_ptr_type _impl): impl(_impl) {} \
        Tretval_ operator()(Tobj_* obj BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(__z__, __arity__, Targ, arg)) { \
            return (obj->*impl)(BOOST_PP_ENUM_PARAMS_Z(__z__, __arity__, arg)); \
        } \
        impl_ptr_type impl; \
    };

#define __MOZO_PHP_FUNCTIONAL_TPL_C(__z__, __arity__, __var__) \
    template<typename Tobj_ \
             BOOST_PP_ENUM_TRAILING_PARAMS_Z(__z__, __arity__, typename Targ) > \
    struct unbound_function<constructor_mark, Tobj_, \
            __MOZO_PHP_FUNCTIONAL_VECTOR_TPL(__z__, __arity__, Targ)> { \
        typedef void result_type; \
        typedef Tobj_ object_type; \
        typedef __MOZO_PHP_FUNCTIONAL_VECTOR_TPL(__z__, __arity__, Targ) \
                arguments; \
        unbound_function() {} \
        void operator()(void* obj BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(__z__, __arity__, Targ, arg)) { \
            new(obj) Tobj_(BOOST_PP_ENUM_PARAMS_Z(__z__, __arity__, arg)); \
        } \
    };

#define __MOZO_PHP_FUNCTIONAL_TPL_F(__z__, __arity__, __var__) \
    template<typename Tretval_ \
             BOOST_PP_ENUM_TRAILING_PARAMS_Z(__z__, __arity__, typename Targ)> \
    struct unbound_function<Tretval_, void, \
            __MOZO_PHP_FUNCTIONAL_VECTOR_TPL(__z__, __arity__, Targ)> { \
        typedef Tretval_ result_type; \
        typedef void object_type; \
        typedef __MOZO_PHP_FUNCTIONAL_VECTOR_TPL(__z__, __arity__, Targ) \
                arguments; \
        typedef __MOZO_PHP_FUNCTIONAL_FUN_TYPE_F(__z__, __arity__, (Targ, impl_ptr_type)); \
        unbound_function(impl_ptr_type _impl): impl(_impl) {} \
        Tretval_ operator()(void* BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(__z__, __arity__, Targ, arg)) { \
            return impl(BOOST_PP_ENUM_PARAMS_Z(__z__, __arity__, arg)); \
        } \
        impl_ptr_type impl; \
    };

#define __MOZO_PHP_FUNCTIONAL_TPL(__z__, __arity__, __var__) \
    __MOZO_PHP_FUNCTIONAL_TPL_M(__z__, __arity__, __var__) \
    __MOZO_PHP_FUNCTIONAL_TPL_C(__z__, __arity__, __var__) \
    __MOZO_PHP_FUNCTIONAL_TPL_F(__z__, __arity__, __var__)

template<typename Tretval_, typename Tobj_, typename Targs_>
struct unbound_function {};

BOOST_PP_REPEAT_FROM_TO(0, BOOST_MPL_LIMIT_VECTOR_SIZE, __MOZO_PHP_FUNCTIONAL_TPL, _);

#undef __MOZO_PHP_MPL_VECTOR_N
#undef __MOZO_PHP_FUNCTIONAL_VECTOR_TPL
#undef __MOZO_PHP_FUNCTIONAL_FUN_TYPE_F
#undef __MOZO_PHP_FUNCTIONAL_FUN_TYPE_M
#undef __MOZO_PHP_FUNCTIONAL_TPL_M
#undef __MOZO_PHP_FUNCTIONAL_TPL_C
#undef __MOZO_PHP_FUNCTIONAL_TPL_F
#undef __MOZO_PHP_FUNCTIONAL_TPL

} } } // namespace boost::php::detail

#endif /* MOZO_PHP_DETAIL_FUNCTIONAL_HPP */