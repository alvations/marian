//=================================================================================================
/*!
//  \file blaze/math/traits/MultTrait.h
//  \brief Header file for the multiplication trait
//
//  Copyright (C) 2013 Klaus Iglberger - All Rights Reserved
//
//  This file is part of the Blaze library. You can redistribute it and/or modify it under
//  the terms of the New (Revised) BSD License. Redistribution and use in source and binary
//  forms, with or without modification, are permitted provided that the following conditions
//  are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice, this list
//     of conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//  3. Neither the names of the Blaze development group nor the names of its contributors
//     may be used to endorse or promote products derived from this software without specific
//     prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
*/
//=================================================================================================

#ifndef _BLAZE_MATH_TRAITS_MULTTRAIT_H_
#define _BLAZE_MATH_TRAITS_MULTTRAIT_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <utility>
#include <blaze/util/Complex.h>
#include <blaze/util/EnableIf.h>
#include <blaze/util/mpl/If.h>
#include <blaze/util/mpl/Or.h>
#include <blaze/util/typetraits/CommonType.h>
#include <blaze/util/typetraits/Decay.h>
#include <blaze/util/typetraits/IsBuiltin.h>
#include <blaze/util/typetraits/IsConst.h>
#include <blaze/util/typetraits/IsReference.h>
#include <blaze/util/typetraits/IsVolatile.h>


namespace blaze {

//=================================================================================================
//
//  CLASS DEFINITION
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Base template for the MultTrait class.
// \ingroup math_traits
//
// \section multtrait_general General
//
// The MultTrait class template offers the possibility to select the resulting data type of
// a generic multiplication operation between the two given types \a T1 and \a T2. MultTrait
// defines the nested type \a Type, which represents the resulting data type of the multiplication.
// In case the two types \a T1 and \a T2 cannot be multiplied, a compilation error is created.
// Note that \c const and \c volatile qualifiers and reference modifiers are generally ignored.
//
// Per default, MultTrait supports all built-in data types. Additionally, the Blaze library
// provides appropriate specializations for the following user-defined arithmetic types:
//
// <ul>
//    <li>std::complex</li>
//    <li>blaze::StaticVector</li>
//    <li>blaze::HybridVector</li>
//    <li>blaze::DynamicVector</li>
//    <li>blaze::CustomVector</li>
//    <li>blaze::CompressedVector</li>
//    <li>blaze::StaticMatrix</li>
//    <li>blaze::HybridMatrix</li>
//    <li>blaze::DynamicMatrix</li>
//    <li>blaze::CustomMatrix</li>
//    <li>blaze::CompressedMatrix</li>
//    <li>blaze::SymmetricMatrix</li>
//    <li>blaze::HermitianMatrix</li>
//    <li>blaze::LowerMatrix</li>
//    <li>blaze::UniLowerMatrix</li>
//    <li>blaze::StrictlyLowerMatrix</li>
//    <li>blaze::UpperMatrix</li>
//    <li>blaze::UniUpperMatrix</li>
//    <li>blaze::StrictlyUpperMatrix</li>
//    <li>blaze::DiagonalMatrix</li>
// </ul>
//
//
// \n \section multtrait_specializations Creating custom specializations
//
// MultTrait is guaranteed to work for all data types that provide a multiplication operator
// (i.e. \c operator*). In order to add support for user-defined data types that either don't
// provide a multiplication operator or whose addition operator returns a proxy object instead
// of a concrete type (as it is common in expression template libraries) it is possible to
// specialize the MultTrait template. The following example shows the according specialization
// for the multiplication between two dynamic column vectors:

   \code
   template< typename T1, typename T2 >
   struct MultTrait< DynamicVector<T1,columnVector>, DynamicVector<T2,columnVector> >
   {
      typedef DynamicVector< typename MultTrait<T1,T2>::Type, columnVector >  Type;
   };
   \endcode

// \n \section multtrait_examples Examples
//
// The following example demonstrates the use of the MultTrait template, where depending on
// the two given data types the resulting data type is selected:

   \code
   template< typename T1, typename T2 >  // The two generic types
   typename MultTrait<T1,T2>::Type       // The resulting generic return type
   mult( const T1& t1, const T2& t2 )    //
   {                                     // The function 'mult' returns the
      return t1 * t2;                    // product of the two given values
   }                                     //
   \endcode
*/
template< typename T1        // Type of the left-hand side operand
        , typename T2        // Type of the right-hand side operand
        , typename = void >  // Restricting condition
struct MultTrait
{
 private:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   using Type1 = Decay_<T1>;
   using Type2 = Decay_<T2>;
   /*! \endcond */
   //**********************************************************************************************

   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   struct MultType { using Type = decltype( std::declval<Type1>() * std::declval<Type2>() ); };
   /*! \endcond */
   //**********************************************************************************************

 public:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   using Type = typename If_< Or< IsConst<T1>, IsVolatile<T1>, IsReference<T1>
                                , IsConst<T2>, IsVolatile<T2>, IsReference<T2> >
                            , MultTrait<Type1,Type2>
                            , MultType >::Type;
   /*! \endcond */
   //**********************************************************************************************
};
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Specialization of the MultTrait class template for a complex and a built-in type.
// \ingroup math_traits
*/
template< typename T1, typename T2 >
struct MultTrait< complex<T1>, T2, EnableIf_< IsBuiltin<T2> > >
{
 public:
   //**********************************************************************************************
   using Type = CommonType_< complex<T1> , T2 >;
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Specialization of the MultTrait class template for a built-in and a complex type.
// \ingroup math_traits
*/
template< typename T1, typename T2 >
struct MultTrait< T1, complex<T2>, EnableIf_< IsBuiltin<T1> > >
{
 public:
   //**********************************************************************************************
   using Type = CommonType_< T1, complex<T2> >;
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Specialization of the MultTrait class template for two complex types.
// \ingroup math_traits
*/
template< typename T1, typename T2 >
struct MultTrait< complex<T1>, complex<T2> >
{
 public:
   //**********************************************************************************************
   using Type = CommonType_< complex<T1>, complex<T2> >;
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Auxiliary alias declaration for the MultTrait class template.
// \ingroup math_traits
//
// The MultTrait_ alias declaration provides a convenient shortcut to access the nested \a Type
// of the MultTrait class template. For instance, given the types \a T1 and \a T2 the following
// two type definitions are identical:

   \code
   using Type1 = typename MultTrait<T1,T2>::Type;
   using Type2 = MultTrait_<T1,T2>;
   \endcode
*/
template< typename T1, typename T2 >
using MultTrait_ = typename MultTrait<T1,T2>::Type;
//*************************************************************************************************

} // namespace blaze

#endif
