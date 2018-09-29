# Makefile-based project build system

## Default architecture x86

ARCH:=x86
NAME=Hello

include config.mk

all: pre-build $(TARGET)


pre-build::			# Commands to be run before the build

post-build::		# Commands to be run after the build

clean::				# Include here any custom clean command
	@rm -f bin/*
	@find . -name '*.o' -exec rm -f {} \;