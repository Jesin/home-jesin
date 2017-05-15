#!/usr/bin/env python3
# vim:ft=python:noet:sw=4:ts=4
import collections
import fileinput
import sys


def latest_only(xs):
	res = collections.OrderedDict()
	for x in xs:
		try: res.move_to_end(x)
		except KeyError: res[x] = None
	return res


def main(*args):
	with fileinput.FileInput(args) as fin:
		del args
		xs = latest_only(fin)
	fout = sys.stdout
	for x in xs:
		fout.write(x)


if __name__ == '__main__':
	main(*sys.argv[1:])
