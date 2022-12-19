#!/bin/sh

flex -l compilador.l
gcc lex.yy.c -o compilador