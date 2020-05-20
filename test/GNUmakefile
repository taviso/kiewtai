include ../GNUmakefile.common

.PHONY: test

all: test

# The tests are name testname.Format.dat and the golden logs are names
# testname.Format.txt
.INTERMEDIATE: $(patsubst %.dat,%.out,$(wildcard *.dat))

test: $(patsubst %.dat,%.out,$(wildcard *.dat))

%.out: %.dat %.log
	../command.exe $(patsubst .%,%,$(suffix $(basename $<))) $< > $@
	cmp --print-bytes $(<:.dat=.log) $@

# Generate golden logs if they don't exist.
.SECONDARY: $(patsubst %.dat,%.log,$(wildcard *.dat))

%.log: %.dat
	@echo No golden log for $<, generating...
	../command.exe $(patsubst .%,%,$(suffix $(basename $<))) $< > $@

clean::
	$(RM) *.out
