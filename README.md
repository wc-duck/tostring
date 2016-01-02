[![Build Status](https://travis-ci.org/wc-duck/tostring.svg?branch=master)](https://travis-ci.org/wc-duck/tostring)
[![Build status](https://ci.appveyor.com/api/projects/status/4ee230jjbw8d4on9?svg=true)](https://ci.appveyor.com/project/wc-duck/tostring)

# About:
Small, single-file, TOSTR() macro to use with printf() and friends.

This is a small macro/lib that I have used quite a bit in my own code that others might like as well. It might not suite everyone
and it has its drawbacks, but it has worked great for me.

The macro TOSTR() will alloc a fixed size string on the stack and call a user-defined function to fill that string depending on
the inputs type. As the macro defines an unnamed object this will need to be used as an argument to a function since the object
will go out of scope directly otherwise.

# Example
```c++
#include "tostring.h"

// ... First lets define a type that we want to add TOSTR() support for ...
struct my_vec3
{
   float x;
   float y;
   float z;
   
   // ... some operators here ...
};

// ... and now lets define a TOSTR-implementation ...
TOSTR_DEFINE_TYPE_FUNCTION( my_vec3, // the type
                            32 )     // 31 chars should be enough ( last one is the null-terminator )
{
    out.put( "{ %.02f, %.02f, %.02f }",   // out.put() have the same format as printf() 
             value.x, value.y, value.z ); // value will be a const& to the value passed to TOSTR()
}

void my_function_that_print_a_vec3( my_vec3 a, my_vec3 b )
{
    printf("a + b = %s\n", TOSTR( a + b ) ); // should print "a + b = { 1.23, 3.34, 5.67 }"
}

```

# Licence:

```
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
```
