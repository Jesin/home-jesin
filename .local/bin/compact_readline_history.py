#!/usr/bin/env pypy
# vim:ft=python:noet:sw=4:ts=4
import fileinput
import sys


def unique_items(xs):
	seen = set()
	for x in xs:
		if x not in seen:
			yield x
			seen.add(x)


def main(*argv):
	fin = fileinput.FileInput(argv)
	xs = tuple(fin)
	fin.close()
	del fin
	xs = reversed(tuple(unique_items(reversed(xs))))
	fout = sys.stdout
	for x in xs:
		fout.write(x)


if __name__ == '__main__':
	main(*sys.argv[1:])
