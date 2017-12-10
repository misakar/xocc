# Makefile for xocc

.PHONY: xocc clean

xocc: src/main.o src/xolex.o src/xo.h src/xosyntax.c
	$(CC) $^ -g -o $@

clean:
	rm -rf xocc src/main.o src/xolex.o src/xosyntax.o
