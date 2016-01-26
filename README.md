# mkpw
mkpw is a tool to generate passwords in the style of "correctHorseBatteryStaple".

Inspiration for mkpw came from [Randall Munroe's xkcd comic #936 "Password strenght"](https://xkcd.com/936/).

![xkcd #936: Password strength](http://imgs.xkcd.com/comics/password_strength.png)

## Supported systems:

mkpw should work on any POSIX-compatible system.

## Compilation:

To compile mkpw, just compile it, for example using gcc:

```bash
gcc -o mkpw mkpw.c
```

## Usage:

```bash
mkpw [OPTIONS] [DICTFILE]
```

`[DICTFILE]` specifies the location of the dictionary file to use (default is dict.txt, which is a german dictionary file). The dictionary file shall be a whitespace-separated list of words.

Possible options are:

* `-c` enables camelCase for the generated password
* `-n [NUMBER]` uses `[NUMBER]` words to generate password
* `-?` prints help
