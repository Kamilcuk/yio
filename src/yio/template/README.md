# Templated directory

## Basics

Files out of this directory have replaced:

 - `Ω` is replaced by `m4_OMEGA` in `m4_config_template()`
 - `π` is replaced by `m4_PI` in `m4_config_template()`
 
Configurations:
- for normal strings: `s/Ω//` and `s/π//`
- for wide strings: `s/Ω/W/` and `s/π/w/`
- for `char16_t` strings: `s/Ω/C16/` and `s/π/c16/`
- for `char32_t` strings: `s/Ω/U/` and `s/π/u/`

On my keyboard Ω and π can be written easily with key combinations 
alt+q and alt+shift+q. This is the reason why am I using them.
When coding with this project, the following setting may be helpful:

```
 xmodmap -e 'keycode 24 = q Q q Q Greek_pi Greek_OMEGA Greek_pi'
```

It will map `alt+q` and `alt+shift+q` combinations to `π` and `Ω`.

## Go on

So each file in this direcory is preprocessed in order:

- First a cmake script replaces all `π` and `Ω` occurences by specified
  replacement. The cmake script and function is in this directory.
- Then `m4` preprocesses each file. M4 first sources the configuration 
  file from destination templating directory, see `src/yio/yio/template.m4`.
  The macros `m4_template_chooser` and `m4_template_chooser2` may be used to choose
  current context while writing functions.
- Then the files are processed.

Each file in this directory tree results in 4!! files.

The tests are also written in `tests/tepmlated`.
