# Introduction

Kiewtai is a HEM (aka plugin) for the [Hiew](http://www.hiew.ru/) hex editor
that makes all the binary parsers from the [Kaitai](https://kaitai.io/) project
available. This means you can get all the fields marked and decoded for dozens
of popular file formats. You can also use the Kaitai format to write a new
template for a file format you're analyzing.

![Screenshot](kiewtai.png)

> Click [here](https://formats.kaitai.io/) to see a list of all the formats
> supported by Kaitai.


# Installation

> If you don't want to build it yourself, check out the
> [releases](https://github.com/taviso/kiewtai/releases)  tab


Copy `kiewtai.hem` to your `hem` folder, which should be where you installed
`hiew`.

# Usage

Press `F11` and Select `Kiewtai: Kaitai Struct format parsers`.

You will be shown a list of all supported parsers, select the one you want.

Kiewtai will highlight the different fields, and add a comment describing the
field.

![Kiewtai JPEG Demo](kiewtai-jpg.gif)

# Advanced Usage

> If you want Kiewtai to analyze a section of a larger file, for example you
> have a firmware blob or filesystem image, Simply mark the section you want
> analyzed with `*` first. If you work in DFIR, you might call this "carving".

 - Press `F2` to toggle between Simple and Detailed parsing.

The default parsing mode is verbose, try this if you prefer.

 - Press `F3` to enable or disable comments.

Kiewtai will add comments to Hiew describing each field, these are displayed as
you navigate around. You can also browse and search them with `F12`.

- Press `F4` to enable or disable markers.

Kiewtai will add color markers by default so you can easily see where the
different fields are. Press `F4` if you don't like this.

- Press `F5` to search for a parser.

The list of parsers is quite long, press `F5` and enter some search terms if
you  like.

![Kiewtai EXE Demo](kiewtai-exe.gif)

# Notes

> If you're a Hiew user and want to help make better documentation, click 
> [here](https://github.com/taviso/hiewdocs)!

This project uses the following third party tools libraries:

- Hiew v8.69 and the HEM SDK v0.53 [http://www.hiew.ru/](http://www.hiew.ru/)
- Kaitai Struct  v0.8 [https://kaitai.io/](https://kaitai.io/)
- Duktape v2.5.0 [https://duktape.org/](https://duktape.org/)

Please feel free to file an issue for any bugs, missing features or documentation!

# Building

> If you don't want to build it yourself, check out the
> [releases](https://github.com/taviso/kiewtai/releases)  tab

I used [GNU make](http://gnuwin32.sourceforge.net/packages/make.htm) and Visual
Studio 2019 to develop Kiewtai.

I use the "Build Tools" variant of Visual Studio, and the only components I
have selected are MSVC and the SDK.

This project uses submodules for some of the dependencies, be sure that you're
using a command like this to fetch all the required code.

```
git submodule update --init --recursive

```

# Author

Tavis Ormandy <taviso@gmail.com>
