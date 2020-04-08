# :notebook: Yio Input Output Library

> A misplaced decimal point will always end up where it will do the greatest damage.  
> -- unknown  :fire: :ambulance:

:star: `Yio Input Output` library is aiming to be python-like type-safe `printf` and `scanf` replacement for C language.

![pipeline status badge](https://gitlab.com/kamcuk/yio/badges/master/pipeline.svg) ![coverage report](https://gitlab.com/kamcuk/yio/badges/master/coverage.svg)

### Namespaces:

 - `y*` - Many common symbols.
 - `yio_*` - Functions and macros for implementators.
 - `_yIO_*` - All pletoria of private symbols.
 
## :beginner: Examples :heart_eyes:

> Chuck Norris counted to infinity... twice.  
> -- unknown :repeat:

The following program does what you think it does:

```
#include <yio.h>
int main() {
    yprint("How old are you?\n");
    int a;
    yscan(&a);
    yprint("You are ", a, " years old.\n");
}
```

There are two set of functions available - one that end with `f` and other do not. Those that end with `f` are to be read as "yio print Format" and accept a formatting string:

```
   yprintf("{:>10} {:<10}\n", "time", "value");
   yprintf("{:>10} {:<10}\n", (time_t)123, 456ull);
```

But no worries, the no-`f`-suffix version also accept a formatting modifiers:

```
   yprint(yppfmt("{:>10}"), "time", " ", yppfmt("{:<10}"), "value", "\n");
   yprint(yppfmt("{:>10}"), (time_t)123, " ", yppfmt("{:<10}"), 456, "\n");
```

The style takes some time to get used to, but it's worth it. Maaany more examples are available in [tests/01_examples](tests/01_examples) directory.

And of course the ultimate `format`:

```
   char *str = yformat("Hello", (char)' ');
   if (str == NULL) abort();
   str = yreformat(str, "world", "\n");
   if (str == NULL) abort();
   yprint(str); // prints 'Hello world'
   free(str);
```

## :cyclone: History and design choices :school:

> [You should read more history books.](https://www.youtube.com/watch?v=tiiI5UbySSw)  
> -- Captain Jean-Luc Picard :stars: :ship:

So at first there was nothing. Then I learned about [overloading macro on number of arguments](https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments) and of course abuot newest C11 addition - `_Generic`.

> If you want to succeed, double your failure rate.  
> -- Thomas J. Watson :computer:

The first design of this library had `x` in front of it and aimed for something like this:

```
xprint("How old are you?\n");
xscan(&i);
xprint("You are ", xfmt("#*.*", 5, 10, i), " years old.\n");
```

Separately, I started to create a design for `o` in front of it, that would work just like C++ alternatives:

```
oprint("How old are you", oendl());
oscan(&i);
oprint("You are ", ofmt_fill('#'), ofmt_width(10), ofmt_precision(5), i, " years old.", oendl());
```

But I didn't like that design. What's the point of creating something people don't want to use. Using `std::cout` is so much typing and that is the reason [fmtlib/fmt](https://github.com/fmtlib/fmt) get's trackion and is so popular. We want something in between - something with good checking and semi-good static type-checking. We want printf, but we want something flexible. We want printf, but we want to be able to specify custom types and format specifiers, to "overload" them and affect them.

That's why [python Format String](https://docs.python.org/3/library/string.html#formatstrings) is so nice - it allows to specify that in a short form. The problem is the static type checking, we can only do _some_ of that in C, not all. I wish there would be `consteval` in C, but that would mean compiler has to do double job.

Finally I settlet for:

```
yprint("How old are you?", yendl());
yscan(&i);
yprint("You are ", yiofmt("#10.5"), i, " years old.", yendl());
```

Which is basically the same as C++ version, but just adds a custom flags modifier/function `yiofmt` that transforms the string in C specification to flags. But this, this is not enough. Cause we want `printf(_("locale strings %s \n"), "string")` which are really hard to do with streams. Also, we want choose position of arguments with that.

This also allowed for custom overloads with custom functions:

```
#define yioarr(arr, count, sep) /* some _Generic magic with custom callback*/
const int arr[10];
yprint("Array is ", yioarr(arr, 5, ", "), "\n");
```

Then I wen to implementing python like syntax:

```
yprintf("How old are you?{}", yendl());
yscanf("{}", &i);
yprintf("You are {} years old.{}", yiofmt("#10.5"), i, yendl());
```

But that is not nice - some of the arguments get a `{}`, but some don't. 
Only those get a `{}` which output anything. This is confusing. 
This breaks existent practice. My C eye is telling me - the number of `{}`
doesn't match the count of arguments. Really, even
`yprintf("You are {}{} years old.{}", yiofmt("#10.5"), i, yendl());` would be more readable.

So I decided to go with python/C++ mix:

```
yprintf("You are {:#10.5} years old.{}", i, yendl());
```

No the `:#10.5` doesn't really print the number. It is parsed the same way as `yiofmt("#10.5")` - it only set's some internal flags. Even with using `#str` macro stringify operator we could even do this:

```
yprintf("{arg1} {arg2}", arg2, arg1);
```

because the names of the variables could be stringified with `#` operator and
then we would have a table we would need to look for the proper variables name. 
it would be a mess with expanding macros, but anyway. But that offers
no static type checking against the number of arguments.
Because of how I decided to pass contexts to callbacks are checked here, both approuches are possibel 

But that is bad also. Because we don't want "one central point of all possible combinations". 
Because [customizing printf](https://www.gnu.org/software/libc/manual/html_node/Customizing-Printf.html) 
is hard and it shouldn't work that way. It should be easy to provide your own formatting 
options and types. It should be easy to parse your own options. Not that way.

So let's take another look at what we want:

1. Include what you use. Your machine does not have float or the float implementation 
   is horrible? No matter, as long as you don't print a float, you don't need to compile 
   all the float stuff.
2. Easy to customize. Easy to add your own handling functions.

And I think I've got it. It works the same as above, but the formatting is not done 
by printf. The formatting is done at the handler site. And for each type you have a 
handler.

So the float should be that way:

1. The printf prints everything up until first `{`. The it extracts the string between 
   `{` and `}`, including optional two positional arguments if needed, and hands it 
   to the next handler function.
2. Each type has a handler, ex. `int` has ex. `_yIO_print_int` handler chosen with _Generic. 
   Now that handler parses two optional arguments with the format string for each type 
   _separately_.
3. Each handler has it's own logic how to handle the format string. 
 
## :family: User reviews :scroll:

> Do something with your life finally. When will you graduate?  
> -- :woman: Mom  

> You don't spend any time with me anymore. You just sit in front that computer all the time. I have enough!  
> -- :girl: gf

> ? I don't understand a thing you're saying.  
> -- :japanese_goblin: Brother

> Whoa, bro, let's get another beer with me? \<incomprehensible noises\>  
> -- :beers: friends  
> Yea, sure.  
> -- :beer: me

## :passport_control: Author

Written by :japanese_ogre: Kamil Cukrowski 2020  \<kamilcukrowski :postbox: gmail.com\>.

```
Yio Input Output library
Copyright (C) 2020 Kamil Cukrowski <kamilcukrowski :postbox: gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
```


