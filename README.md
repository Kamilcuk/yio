
# :bowtie: YIO :sunglasses:

:star: `YIO Input Output` library is aiming to be python-like semi-type-safe `printf` and `scanf` replacement for C language. 

> A misplaced decimal point will always end up where it will do the greatest damage. 
> -- unknown  :fire: :ambulance:
        
## :beginner: Examples  :heart_eyes:

> Chuck Norris counted to infinity... twice. 
> -- unknown :repeat:

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
yprintf("{arg1} {arg2}", arg2, arg1);`
```

because the names of the variables could be stringified with `#` operator and
then we would have a table we would need to look for the proper variables name. 
it would be a mess with expanding macros, but anyway. But that offers
no static type checking against the number of arguments.
Because of how I decided to pass contexts to callbacks are checked here, both approuches are possibel 

## :family: User reviews :scroll:

> Do something with your life finally. When will you graduate?
> -- :woman: Mom  

>You don't spend any time with me anymore. You just sit in front that computer all the time. I have enough!
> -- :girl: Gf

> ?
> -- :japanese_ogre: brother


## :passport_control: Author

:japanese_goblin: Written by Kamil Cukrowski 2019

:alien: Copyright jointly under Beerware License and GPL 2.0 License.


