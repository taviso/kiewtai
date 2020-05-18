include GNUmakefile.common

KSC=cmd.exe /c kaitai-struct-compiler
KFLAGS=--debug --target javascript

formats=$(wildcard parsers/*.js)

all: parsers
	$(MAKE) command.exe
	$(MAKE) kiewtai.hem

parsers:
	-$(KSC) $(KFLAGS) -I formats -d $@ $(wildcard formats/*/*.ksy)

# I don't know how this mess works
parsers.h: parsers
	for i in parsers/*.js; do                                               \
	    printf "extern char binary_%s_start[];\n" $${i//[.\/]/_};           \
	    printf "extern size_t binary_%s_size;\n" $${i//[.\/]/_};            \
	done > $@
	printf "static KAITAI_PARSER KaitaiParsers[] = {\n" >> $@
	for i in parsers/*.js; do                                               \
	    filename=$${i#*/};                                                  \
	    printf "{ \"%s\", binary_%s_start, (ULONG) &binary_%s_size },\n"    \
	        $${filename%.*} $${i//[.\/]/_} $${i//[.\/]/_};                  \
	done >> $@
	printf "};\n" >> $@
	# Special Case: Jpeg requires Exif
	cat parsers/Exif.js parsers/Jpeg.js > ExifJpegCombined.js
	mv ExifJpegCombined.js parsers/Jpeg.js

kaitai.obj: | parsers.h
kiewtai.obj: | parsers.h

command.exe: kaitai.obj duktape.obj PolyFill.obj KaitaiStream.obj pako_inflate.obj $(formats:.js=.obj)

kiewtai.dll: kaitai.obj duktape.obj PolyFill.obj KaitaiStream.obj pako_inflate.obj $(formats:.js=.obj) hiewgate.obj

%.obj: %.js
	objcopy -I binary -O pe-i386 $^ $@

%.js: %.ksy
	cmd.exe /c kaitai-struct-compiler --debug --target javascript $^

%.hem: %.dll
	cp $< $@

clean::
	rm -rf parsers parsers.h *.hem

