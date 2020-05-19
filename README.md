# Introduction

Kiewtai is a HEM (aka plugin) for the [Hiew](http://www.hiew.ru/) hex editor
that makes all the binary parsers from the [Kaitai](https://kaitai.io/) project
available. This means you can get all the fields marked and decoded for dozens
of popular file formats. You can also use the Kaitai format to write a new
template for a file format you're analyzing.

> Click [here](https://formats.kaitai.io/) to see a list of all the formats
> supported by Kaitai.

Want to see it in action? See some [Screenshots](#screenshots)!


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

![Kiewtai JPEG Demo](doc/kiewtai-jpg.gif)

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

![Kiewtai EXE Demo](doc/kiewtai-exe.gif)

# Notes

> If you're a Hiew user and want to help make better documentation, click 
> [here](https://github.com/taviso/hiewdocs)!

This project uses the following third party tools libraries:

- Hiew v8.69 and the HEM SDK v0.53 [http://www.hiew.ru/](http://www.hiew.ru/)
- Kaitai Struct  v0.8 [https://kaitai.io/](https://kaitai.io/)
- Duktape v2.5.0 [https://duktape.org/](https://duktape.org/)

Please feel free to file an issue for any bugs, missing features or documentation!

# Screenshots

Here are some screenshots of different Kiewtai screens.

## Browsing a GIF header

You can see the  magic, version, descriptors, dimensions are all identified.

The comment shows Kiewtai knows the cursor is on the `applicationId` field.

> The individual R/G/B bytes are hilighted, which makes the data look
> stripey. If that's *too* much verbosity, press `F2` on the parser
> list and Kiewtay will reduce the level of detail it generates.

![Screenshot](doc/kiewtai.png)


## Show the recognized fields in an EXE file.

You can load multiple Kaitai parsers at once, this screenshot shows the
[DosMz](https://formats.kaitai.io/dos_mz/index.html) and
[MicrosoftPE](https://formats.kaitai.io/microsoft_pe/index.html) parsers
loaded simultaneously.

If you have an embedded file, simply
[mark](https://taviso.github.io/hiewdocs/#mark) it and Kiewtai will only
analyze that block.

![Kiewtai MZ/PE fields](doc/kiewtai-fields.png)

## Browsing the chunks of a PNG image.

The field names display as comments as you navigate around a file.

![Kiewtai PNG chunks](doc/kiewtai-png.png)

## Browsing the list of formats available.

Kaitai has parsers for dozens of popular formats already made, you can  see the
full list online [here](https://formats.kaitai.io/).

![Kiewtai Parser List](doc/kiewtai-formats.png)


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

- The Kaitai compiler, ksc, is required.
- Objcopy from GNU binutils is used to embed the Kaitai parsers.

# Author

Tavis Ormandy <taviso@gmail.com>
