////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DLokiTypeTrait.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-4-30 9:45:30
//  Comment     : ��C++�����˼ά������
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DLOKITYPETRAIT_H_
#define _INCLUDE_KG3DLOKITYPETRAIT_H_

#include "KG3DLokiTypeList.h"

////////////////////////////////////////////////////////////////////////////////
namespace LOKI_APP
{
	////////////////////////////////////////////////////////////////////////////////
	// class template IsCustomUnsignedInt
	// Offers a means to integrate nonstandard built-in unsigned integral types
	// (such as unsigned __int64 or unsigned long long int) with the TypeTraits 
	//     class template defined below.
	// Invocation: IsCustomUnsignedInt<T> where T is any type
	// Defines 'value', an enum that is 1 iff T is a custom built-in unsigned
	//     integral type
	// Specialize this class template for nonstandard unsigned integral types
	//     and define value = 1 in those specializations
	////////////////////////////////////////////////////////////////////////////////

	template <typename T>
	struct TIsCustomUnsignedInt
	{
		enum { value = 0 };
	};        

	////////////////////////////////////////////////////////////////////////////////
	// class template IsCustomSignedInt
	// Offers a means to integrate nonstandard built-in unsigned integral types
	// (such as unsigned __int64 or unsigned long long int) with the TypeTraits 
	//     class template defined below.
	// Invocation: IsCustomSignedInt<T> where T is any type
	// Defines 'value', an enum that is 1 iff T is a custom built-in signed
	//     integral type
	// Specialize this class template for nonstandard unsigned integral types
	//     and define value = 1 in those specializations
	////////////////////////////////////////////////////////////////////////////////

	template <typename T>
	struct TIsCustomSignedInt
	{
		enum { value = 0 };
	};        

	////////////////////////////////////////////////////////////////////////////////
	// class template IsCustomFloat
	// Offers a means to integrate nonstandard floating point types with the
	//     TypeTraits class template defined below.
	// Invocation: IsCustomFloat<T> where T is any type
	// Defines 'value', an enum that is 1 iff T is a custom built-in
	//     floating point type
	// Specialize this class template for nonstandard unsigned integral types
	//     and define value = 1 in those specializations
	////////////////////////////////////////////////////////////////////////////////

	template <typename T>
	struct TIsCustomFloat
	{
		enum { value = 0 };
	};        

	////////////////////////////////////////////////////////////////////////////////
	// Helper types for class template TypeTraits defined below
	////////////////////////////////////////////////////////////////////////////////

	namespace Private
	{
		typedef TYPELIST_4(unsigned char, unsigned short int,
			unsigned int, unsigned long int) StdUnsignedInts;
		typedef TYPELIST_4(signed char, short int,
			int, long int) StdSignedInts;
		typedef TYPELIST_3(bool, char, wchar_t) StdOtherInts;
		typedef TYPELIST_3(float, double, long double) StdFloats;
	}

	////////////////////////////////////////////////////////////////////////////////
	// class template TypeTraits
	// Figures out various properties of any given type
	// Invocations (T is a type):
	// a) TypeTraits<T>::isPointer
	// returns (at compile time) true if T is a pointer type
	// b) TypeTraits<T>::PointeeType
	// returns the type to which T points is T is a pointer type, NullType otherwise
	// a) TypeTraits<T>::isReference
	// returns (at compile time) true if T is a reference type
	// b) TypeTraits<T>::ReferredType
	// returns the type to which T refers is T is a reference type, NullType
	// otherwise
	// c) TypeTraits<T>::isMemberPointer
	// returns (at compile time) true if T is a pointer to member type
	// d) TypeTraits<T>::isStdUnsignedInt
	// returns (at compile time) true if T is a standard unsigned integral type
	// e) TypeTraits<T>::isStdSignedInt
	// returns (at compile time) true if T is a standard signed integral type
	// f) TypeTraits<T>::isStdIntegral
	// returns (at compile time) true if T is a standard integral type
	// g) TypeTraits<T>::isStdFloat
	// returns (at compile time) true if T is a standard floating-point type
	// h) TypeTraits<T>::isStdArith
	// returns (at compile time) true if T is a standard arithmetic type
	// i) TypeTraits<T>::isStdFundamental
	// returns (at compile time) true if T is a standard fundamental type
	// j) TypeTraits<T>::isUnsignedInt
	// returns (at compile time) true if T is a unsigned integral type
	// k) TypeTraits<T>::isSignedInt
	// returns (at compile time) true if T is a signed integral type
	// l) TypeTraits<T>::isIntegral
	// returns (at compile time) true if T is a integral type
	// m) TypeTraits<T>::isFloat
	// returns (at compile time) true if T is a floating-point type
	// n) TypeTraits<T>::isArith
	// returns (at compile time) true if T is a arithmetic type
	// o) TypeTraits<T>::isFundamental
	// returns (at compile time) true if T is a fundamental type
	// p) TypeTraits<T>::ParameterType
	// returns the optimal type to be used as a parameter for functions that take Ts
	// q) TypeTraits<T>::isConst
	// returns (at compile time) true if T is a const-qualified type
	// r) TypeTraits<T>::NonConstType
	// removes the 'const' qualifier from T, if any
	// s) TypeTraits<T>::isVolatile
	// returns (at compile time) true if T is a volatile-qualified type
	// t) TypeTraits<T>::NonVolatileType
	// removes the 'volatile' qualifier from T, if any
	// u) TypeTraits<T>::UnqualifiedType
	// removes both the 'const' and 'volatile' qualifiers from T, if any
	////////////////////////////////////////////////////////////////////////////////

	template <typename T>
	class TTypeTraits
	{
	private:
		template <class U> struct TPointerTraits
		{
			enum { result = false };
			typedef NullType PointeeType;
		};

		template <class U> struct TPointerTraits<U*>
		{
			enum { result = true };
			typedef U PointeeType;
		};

		template <class U> struct TReferenceTraits
		{
			enum { result = false };
			typedef U ReferredType;
		};

		template <class U> struct TReferenceTraits<U&>
		{
			enum { result = true };
			typedef U ReferredType;
		};

		template <class U> struct TPToMTraits
		{
			enum { result = false };
		};

		template <class U, class V>
		struct TPToMTraits<U V::*>
		{
			enum { result = true };
		};

		template <class U> struct TUnConst
		{
			typedef U Result;
			enum { isConst = 0 };
		};

		template <class U> struct TUnConst<const U>
		{
			typedef U Result;
			enum { isConst = 1 };
		};

		template <class U> struct TUnVolatile
		{
			typedef U Result;
			enum { isVolatile = 0 };
		};

		template <class U> struct TUnVolatile<volatile U>
		{
			typedef U Result;
			enum { isVolatile = 1 };
		};

	public:
		enum { isPointer = TPointerTraits<T>::result};
		typedef typename TPointerTraits<T>::PointeeType PointeeType;

		enum { isReference = TReferenceTraits<T>::result };
		typedef typename TReferenceTraits<T>::ReferredType ReferredType;

		enum { isMemberPointer = TPToMTraits<T>::result };

		enum { isStdUnsignedInt = 
			TL::TIndexOf<Private::StdUnsignedInts, T>::result >= 0 };
		enum { isStdSignedInt = 
			TL::TIndexOf<Private::StdSignedInts, T>::result >= 0 };
		enum { isStdIntegral = isStdUnsignedInt || isStdSignedInt ||
			TL::TIndexOf<Private::StdOtherInts, T>::result >= 0 };
		enum { isStdFloat = TL::TIndexOf<Private::StdFloats, T>::result >= 0 };
		enum { isStdArith = isStdIntegral || isStdFloat };
		enum { isStdFundamental = isStdArith || isStdFloat || 
			Conversion<T, void>::sameType };

		enum { isUnsignedInt = isStdUnsignedInt || TIsCustomUnsignedInt<T>::value };
		enum { isSignedInt = isStdSignedInt || TIsCustomSignedInt<T>::value };
		enum { isIntegral = isStdIntegral || isUnsignedInt || isSignedInt };
		enum { isFloat = isStdFloat || TIsCustomFloat<T>::value };
		enum { isArith = isIntegral || isFloat };
		enum { isFundamental = isStdFundamental || isArith || isFloat };

		typedef typename TSelect<isStdArith || isPointer || isMemberPointer,
			T, ReferredType&>::Result
			ParameterType;

		enum { isConst = TUnConst<T>::isConst };
		typedef typename TUnConst<T>::Result NonConstType;
		enum { isVolatile = TUnVolatile<T>::isVolatile };
		typedef typename TUnVolatile<T>::Result NonVolatileType;
		typedef typename TUnVolatile<typename TUnConst<T>::Result>::Result 
			UnqualifiedType;
	};
}

////////////////////////////////////////////////////////////////////////////////
// Change log:
// June 20, 2001: ported by Nick Thurn to gcc 2.95.3. Kudos, Nick!!!
////////////////////////////////////////////////////////////////////////////////

#endif //_INCLUDE_KG3DLOKITYPETRAIT_H_
