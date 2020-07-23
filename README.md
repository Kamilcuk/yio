# :notebook: Yio Input Output Library

> A misplaced decimal point will always end up where it will do the greatest damage.  
> -- unknown  :fire: :ambulance:

:star: `Yio Input Output` library is aiming to be python-like type-safe `printf` and `scanf` replacement for C language.



[[_TOC_]]

# dev

[doxygen documentation](https://kamcuk.gitlab.io/yio/doxygen/index.html) [cdash dashboard](https://cdash.karta.dyzio.pl/index.php?project=Yio)

master: ![pipeline status badge](https://gitlab.com/kamcuk/yio/badges/master/pipeline.svg)
devel: ![pipeline status badge](https://gitlab.com/kamcuk/yio/badges/devel/pipeline.svg)

last cdash build: ![Passed tests](https://img.shields.io/badge/dynamic/json?color=blue&label=Passed%20tests&query=pass&url=https%3A%2F%2Fkamcuk.gitlab.io%2Fyio%2Fbadge.json)
![Failed tests](https://img.shields.io/badge/dynamic/json?color=blue&label=Failed%20tests&query=fail&url=https%3A%2F%2Fkamcuk.gitlab.io%2Fyio%2Fbadge.json)
![Tests time](https://img.shields.io/badge/dynamic/json?color=blue&label=Tests%20time&query=time&url=https%3A%2F%2Fkamcuk.gitlab.io%2Fyio%2Fbadge.json)

# Installation

### Debian

```
sudo apt-key adv --keyserver hkp://pool.sks-keyservers.net --recv-keys 9AB6D219060C0B5B
sudo apt-key adv -a --export 9AB6D219060C0B5B | sudo apt-key add -
echo 'deb https://kamcuk.gitlab.io/yio/debian buster main' | sudo tee -a /etc/apt/sources.list.d/yio.list
sudo apt-get update
sudo apt-get install -y yio
```

### Ubuntu

```
sudo apt-key adv --keyserver hkp://pool.sks-keyservers.net --recv-keys 9AB6D219060C0B5B
sudo apt-key adv -a --export 9AB6D219060C0B5B | sudo apt-key add -
echo 'deb https://kamcuk.gitlab.io/yio/ubuntu focal main' | sudo tee -a /etc/apt/sources.list.d/yio.list
sudo apt-get update
sudo apt-get install -y yio

```

### Archlinux

```
sudo pacman-key --keyserver hkp://pool.sks-keyservers.net --recv-keys 9AB6D219060C0B5B
sudo pacman-key --lsign-key 9AB6D219060C0B5B
sudo cp /etc/pacman.conf /etc/pacman.con.backup
printf "%s\n" '' '[yio]' 'Server = https://kamcuk.gitlab.io/yio/archlinux/$arch/' '' | sudo tee -a /etc/pacman.conf
sudo pacman -Sy --noconfirm yio
```

### Alpine

```
apk add -X https://kamcuk.gitlab.io/yio/alpine/ --allow-untrusted yio
```

WARNING: The following does not work, I don't know how to make it work, because the public key is wrong and I don't know how to import it or maybe sign it properly, dunno.

```
wget 'https://kamcuk.gitlab.io/yio/alpine/kamilcukrowski@gmail.com.rsa.pub' -O /etc/apk/keys/kamilcukrowski@gmail.com.rsa.pu
echo 'https://kamcuk.gitlab.io/yio/alpine/' >> /etc/apk/repositories
apk add yio
```

## Build, compilation and CMake

Add this repo to your project as a git submodule and add the main CMakeLists.txt as a subdirectory.
I think this would be the preferred way to use this library.

### Build dependencies

- `cmake` is used for compilation
- `m4` is used as templating system for file generation
- `bash` and `make` are used for various administration tasks

### Compilation and installation with CMake

```
cmake -S . -B _build
cmake --build _build --target install
```

### CMake as add subdirectory

```
# Ie. clone the library your preferred way 
if(NOT EXISTS third_party/yio)
	execute_process(
		COMMAND git clone https://gitlab.com/kamcuk/yio/ third_party/yio
	)
endif()

add_subdirectory(yio)
target_link_libraries(yourtarget yio)
```

### CMake with find_package after installation

```
find_package(yio REQUIRED)
target_link_libraries(yourtarget yio::yio)
```

# :beginner: Examples :heart_eyes:

> Chuck Norris counted to infinity... twice.  
> -- unknown :repeat:

The following program does what you think it does:
<a href="https://ce.karta.dyzio.pl/#g:!((g:!((g:!((h:codeEditor,i:(fontScale:14,j:1,lang:___c,selection:(endColumn:15,endLineNumber:7,positionColumn:15,positionLineNumber:7,selectionStartColumn:15,selectionStartLineNumber:7,startColumn:15,startLineNumber:7),source:'%23include+%3Cyio.h%3E%0A%0A%23include+%3Cyio.h%3E%0Aint+main()+%7B%0A++++yprint(%22How+old+are+you%3F%5Cn%22)%3B%0A++++int+a%3B%0A++++yscan(%26a)%3B%0A++++yprint(%22You+are+%22,+a,+%22+years+old.%5Cn%22)%3B%0A%7D%0A'),l:'5',n:'0',o:'C+source+%231',t:'0')),k:100,l:'4',m:50,n:'0',o:'',s:0,t:'0'),(g:!((h:executor,i:(argsPanelShown:'1',compilationPanelShown:'0',compiler:gcc1010,compilerOutShown:'0',execArgs:'',execStdin:'25',fontScale:14,j:1,lang:___c,libs:!(),options:'',source:1,stdinPanelShown:'0'),l:'5',n:'0',o:'x86-64+gcc+10.1.0+Executor+(Editor+%231)+C',t:'0')),header:(),l:'4',m:50,n:'0',o:'',s:0,t:'0')),l:'3',n:'0',o:'',t:'0')),version:4">Try it out!</a>

```
#include <yio.h>
int main() {
    yprint("How old are you?\n");
    int a;
    yscan(&a);
    yprint("You are ", a, " years old.\n");
}
```

You can change the type of `a` to `long` or `short` and both functions `yscan` and `yprint` will properly work.
There are two "set" of functions available - one that end with `f` and other do not.
Those that end with `f` are to be read as "yio print Format" and accept a python-like formatting string specifier:
<a href="https://ce.karta.dyzio.pl/#g:!((g:!((g:!((h:codeEditor,i:(fontScale:14,j:1,lang:___c,selection:(endColumn:5,endLineNumber:5,positionColumn:5,positionLineNumber:5,selectionStartColumn:5,selectionStartLineNumber:5,startColumn:5,startLineNumber:5),source:'%23include+%3Cyio.h%3E%0A%0Aint+main()+%7B%0A++++yprintf(%22%7B:%3E10%7D+%7B:%3C10%7D%5Cn%22,+%22time%22,+%22value%22)%3B%0A++++yprintf(%22%7B:%3E10%7D+%7B:%3C10%7D%5Cn%22,+(time_t)123,+456ull)%3B%0A%7D%0A%0A'),l:'5',n:'0',o:'C+source+%231',t:'0')),k:100,l:'4',m:50,n:'0',o:'',s:0,t:'0'),(g:!((h:executor,i:(argsPanelShown:'1',compilationPanelShown:'0',compiler:gcc1010,compilerOutShown:'0',execArgs:'',execStdin:'',fontScale:14,j:1,lang:___c,libs:!(),options:'',source:1,stdinPanelShown:'1'),l:'5',n:'0',o:'x86-64+gcc+10.1.0+Executor+(Editor+%231)+C',t:'0')),header:(),l:'4',m:50,n:'0',o:'',s:0,t:'0')),l:'3',n:'0',o:'',t:'0')),version:4">Try it out!</a>

```
yprintf("{:>10} {:<10}\n", "time", "value");
yprintf("{:>10} {:<10}\n", (time_t)123, 456ull);
```

In addition the library also accepts a _custom modifier_ functions.
Such function are specially crafted and can affect the formatting output of the function.
They are used with no-`f`-suffix-version of the function to specify formatting options, or to specify explicit type to print the variable witt.
They can also accepts a formatting modifiers:
<a href="https://ce.karta.dyzio.pl/#g:!((g:!((g:!((h:codeEditor,i:(fontScale:14,j:1,lang:___c,selection:(endColumn:77,endLineNumber:4,positionColumn:77,positionLineNumber:4,selectionStartColumn:77,selectionStartLineNumber:4,startColumn:77,startLineNumber:4),source:'%23include+%3Cyio.h%3E%0Aint+main()+%7B%0A++++yprint(yppfmt(%22%7B:%3E10%7D%22),+%22time%22,+%22+%22,+yppfmt(%22%7B:%3C10%7D%22),+%22value%22,+%22%5Cn%22)%3B%0A++++yprint(yppfmt(%22%7B:%3E10%7D%22),+(time_t)123,+%22+%22,+yppfmt(%22%7B:%3C10%7D%22),+456,+%22%5Cn%22)%3B%0A%7D%0A'),l:'5',n:'0',o:'C+source+%231',t:'0')),k:100,l:'4',m:50,n:'0',o:'',s:0,t:'0'),(g:!((h:executor,i:(argsPanelShown:'1',compilationPanelShown:'0',compiler:gcc1010,compilerOutShown:'0',execArgs:'',execStdin:'25',fontScale:14,j:1,lang:___c,libs:!(),options:'',source:1,stdinPanelShown:'0'),l:'5',n:'0',o:'x86-64+gcc+10.1.0+Executor+(Editor+%231)+C',t:'0')),header:(),l:'4',m:50,n:'0',o:'',s:0,t:'0')),l:'3',n:'0',o:'',t:'0')),version:4">Try it out!</a>

```
yprint(yppfmt("{:>10}"), "time", " ", yppfmt("{:<10}"), "value", "\n");
yprint(yppfmt("{:>10}"), (time_t)123, " ", yppfmt("{:<10}"), 456, "\n");
```

Mny more examples are available in [tests/01_examples](tests/01_examples) and overall in [test](test) directory.
And of course the ultimate `yformat` that allows for allocating the buffer and works just as you would expect:
<a href="https://ce.karta.dyzio.pl/#g:!((g:!((g:!((h:codeEditor,i:(fontScale:14,j:1,lang:___c,selection:(endColumn:5,endLineNumber:4,positionColumn:5,positionLineNumber:4,selectionStartColumn:5,selectionStartLineNumber:4,startColumn:5,startLineNumber:4),source:'%23include+%3Cyio.h%3E%0Aint+main()+%7B%0A++++char+*str+%3D+yformat(%22Hello%22,+(char)!'+!')%3B%0A++++if+(str+%3D%3D+NULL)+abort()%3B%0A++++str+%3D+yreformat(str,+%22world%22,+%22%5Cn%22)%3B%0A++++if+(str+%3D%3D+NULL)+abort()%3B%0A++++yprint(str)%3B+//+prints+!'Hello+world!'%0A++++free(str)%3B%0A%7D%0A'),l:'5',n:'0',o:'C+source+%231',t:'0')),k:100,l:'4',m:50,n:'0',o:'',s:0,t:'0'),(g:!((h:executor,i:(argsPanelShown:'1',compilationPanelShown:'0',compiler:gcc1010,compilerOutShown:'0',execArgs:'',execStdin:'25',fontScale:14,j:1,lang:___c,libs:!(),options:'',source:1,stdinPanelShown:'0'),l:'5',n:'0',o:'x86-64+gcc+10.1.0+Executor+(Editor+%231)+C',t:'0')),header:(),l:'4',m:50,n:'0',o:'',s:0,t:'0')),l:'3',n:'0',o:'',t:'0')),version:4">Try it out!</a>


```
   char *str = yformat("Hello", (char)' ');
   if (str == NULL) abort();
   str = yreformat(str, "world", "\n");
   if (str == NULL) abort();
   yprint(str); // prints 'Hello world'
   free(str);
```

### Namespaces:

 - `y*` - Many common symbols, like `yprintf` or `yppfmt` or `yscanf`.
 - `yio_*` - Public functions and symbol for implementators of implementators of _custom modifiers_.
 - `YIO_*` - Macros, options and constants.
 - `_yIO_*` - All pletoria of private symbols that is used by the library.

# Specification and internal info

Generally the idea is to adhere to [standard format specification](https://en.cppreference.com/w/cpp/utility/format/formatter#Standard_format_specification),
but many, many features are implemented yet.

- All standard types work
- `b` `B` `x` `X` `o` `O` `c` `d` work
- `<` `>` `^` work
- `g` `G` `f` `F` `a` `A` `e` `E` work
    - There are three "backends" to print floating points.
      The library will prefer to use `strfrom*` functions if they are available.
      You can use YIO_PRINT_FLOAT_WITH macro to choose the printing function, see yio/yio/manip/print_float.h
- No, `L` locale specific is not implemented. Locale is handled in a chaotic way.
- Specifying `arg-id` is not implemented.
 

# Archaic and references

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

# :passport_control: Author and license

Written by :japanese_ogre: Kamil Cukrowski 2020  \<kamilcukrowski :postbox: gmail.com\>.

I would be happy to rephrase the license to be more like newlib, so users can use it 
in embedded targets, but I do not know really how.

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


