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

#include "greatest.h"
#include "../tostring.h"

struct two_ints
{
	int a;
	int b;
};

TOSTR_DEFINE_TYPE_FUNCTION( two_ints, 16 ) { out.put( "%d, %d", value.a, value.b ); }

struct with_string
{
	const char* str;
};

TOSTR_DEFINE_TYPE_FUNCTION( with_string, 8 ) { out.put( "%s", value.str ); }

struct append_me
{
	int a;
	int b;
	int c;
};

TOSTR_DEFINE_TYPE_FUNCTION( append_me, 16 )
{
	out.put( "%d", value.a );
	out.put( " " );
	out.put( "%d", value.b );
	out.put( " " );
	out.put( "%d", value.c );
}

TEST tostring_basic()
{
	two_ints v = { 13, 37 };
	ASSERT_STR_EQ( "13, 37", TOSTR(v) );
	return 0;
}

TEST tostring_overflow()
{
	with_string v1 = { "1234567890" };
	ASSERT_STR_EQ( "1234567", TOSTR(v1) );
	return 0;
}

TEST tostring_append()
{
	append_me v1 = { 12, 34, 56 };
	ASSERT_STR_EQ( "12 34 56", TOSTR(v1) );
	return 0;
}

GREATEST_SUITE( tostring )
{
	RUN_TEST( tostring_basic );
	RUN_TEST( tostring_overflow );
	RUN_TEST( tostring_append );
}

GREATEST_MAIN_DEFS();

int main( int argc, char **argv )
{
    GREATEST_MAIN_BEGIN();
    RUN_SUITE( tostring );
    GREATEST_MAIN_END();
}
