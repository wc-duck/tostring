/*
   One-file TOSTR(), stack only, macro to be used together with printf and friends.
   This implementation might not work for everyone and might not work in all cases
   and has its limitations but it has worked great for me.

   Worth to know:
   - each TOSTR() allocates a fixed size buffer on the stack.
   - string will be truncated on overflow.

   www.github.com/wc-duck/tostring/

   version 0.1, January, 2016

   Copyright (C) 2016- Fredrik Kihlander

   This software is provided 'as-is', without any express or implied
   warranty.  In no event will the authors be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
      claim that you wrote the original software. If you use this software
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.

   Fredrik Kihlander
*/

#ifndef TOSTRING_H_INCLUDED
#define TOSTRING_H_INCLUDED

#include <stdio.h>
#include <stdarg.h>

/**
 * Generic macro to convert an object to string, the type of the object need to have a to-string function defined
 * by TOSTR_DEFINE_TYPE_FUNCTION for this to work.
 * The string will be allocated on the stack and only valid as long as the defined object is alive. The object
 * will not be named so it need to be used as an argument to a function otherwise it will go out of scope.
 *
 * @usage:
 *
 *   printf( "%s", TOSTR( my_obj ) );
 */
#define TOSTR( obj ) _TOSTR_buffer_holder<TOSTR_GET_STRING_SIZE(obj)>()( obj )

/**
 * Define function to convert type to stack-string. The function that will get defined will have get the value to output
 * as "value" and need to write its output with out.put(). out.put() can be called multiple times and the output will
 * then get appended.
 *
 * @param type to define function for
 * @param static size of buffer to allocate on stack for this string, if the compiler do not support decltype or __typeof__
 *        a fixed size buffer will be used.
 *
 * @usage:
 *
 *   struct my_type { int a; int b; }
 *
 *   TOSTR_DEFINE_TYPE_FUNCTION( my_type, 64 )
 *   {
 *      out.put( "a = %d and b = %d", value.a, value.b );
 *   }
 */
#define TOSTR_DEFINE_TYPE_FUNCTION( type, buffer_size ) \
	template<> struct _TOSTR_type_size<type> { enum { STRING_SIZE = buffer_size }; }; \
	template<> struct _TOSTR_type_size<type&> { enum { STRING_SIZE = buffer_size }; }; \
	template<> struct _TOSTR_type_size<const type> { enum { STRING_SIZE = buffer_size }; }; \
	template<> struct _TOSTR_type_size<const type&> { enum { STRING_SIZE = buffer_size }; }; \
	inline void _TOSTR_type_impl( _TOSTR_builder& out, const type& value )



// ... implementation details ...

template< typename T >
struct _TOSTR_type_size;

struct _TOSTR_builder
{
	int len;
	int size;
	char* buffer;
	_TOSTR_builder( char* _buffer, int buffer_size )
		: len(0)
		, size( buffer_size )
		, buffer( _buffer )
	{}

	inline void put( const char* fmt, ... )
	{
		va_list lst;
		va_start( lst, fmt );
		int res = vsnprintf( buffer + len, (size_t)(size - len), fmt, lst );
		if( res < 0 )
			len = size - 1;
		else
		{
			len += res;
			len = len < size - 1 ? len : size - 1;
		}
		buffer[ size - 1 ] = '\0';
		va_end( lst );
	}
};

/**
 * Implements a TOSTR macro to be able to convert different types of objects to strings
 * while still keeping all data on the stack.
 */
template< size_t SIZE >
struct _TOSTR_buffer_holder
{
	char str[SIZE];

	template< typename T >
	inline const char* operator() ( const T& val )
	{
		_TOSTR_builder b( str, sizeof(str) );
		_TOSTR_type_impl( b, val );
		return str;
	}
};

#if __cplusplus > 199711L || _MSC_VER >= 1600
	// ... compiler is c++11 compliant, decltype should be supported ...
	#define TOSTR_GET_STRING_SIZE( expr ) \
		_TOSTR_type_size<decltype( expr )>::STRING_SIZE
#elif defined(__GNUC__) || defined(__clang__)
	// ... gcc-specific extension __typeof__ ...
	#define TOSTR_GET_STRING_SIZE( expr ) \
		_TOSTR_type_size<__typeof__( expr )>::STRING_SIZE
#else
	// ... couldn't find any way to find the type of an expression, default to 128 byte buffer ...
	#define TOSTR_GET_STRING_SIZE( expr ) \
		(128)
#endif

#endif // TOSTRING_H_INCLUDED
