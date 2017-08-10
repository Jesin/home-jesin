#!/usr/bin/env python3
# vim:ft=python:noet:sw=4:ts=4
import contextlib, hashlib
import os, stat, sys


def hash_file(h, f):
	with contextlib.ExitStack() as janitor:
		try: f.readinto
		except AttributeError:
			f = janitor.enter_context(open(f, 'rb', 0))
		m = janitor.enter_context(memoryview(bytearray(131072)))
		while True:
			i = f.readinto(m)
			if i <= 0:
				break
			h.update(m[:i])
	return h


def hash_all_files(paths, htoi=None, itop=None, hasher=hashlib.blake2b):
	if htoi is None: htoi = {}
	if itop is None: itop = {}
	for p in paths:
		ino = None
		try:
			if p.is_file(follow_symlinks=False):
				ino = p.inode()
			elif not p.is_dir(follow_symlinks=False):
				continue
			p = p.path
		except AttributeError:
			s = os.lstat(p)
			if stat.S_ISREG(s.st_mode):
				ino = s.st_ino
			elif not stat.S_ISDIR(s.st_mode):
				continue
		if ino is None:
			with os.scandir(p) as cs:
				hash_all_files(cs, htoi, itop, hasher)
		else:
			ps = itop.setdefault(ino, [])
			if not ps:
				htoi.setdefault(hash_file(hasher(), p).digest(), []).append(ino)
			ps.append(p)
	return (htoi, itop)


def main(*args):
	args = map(os.fsencode, args)
	htoi, itop = hash_all_files(args)
	del args
	htoi = sorted(htoi.items())
	out = sys.stdout.buffer
	for (h, inodes) in htoi:
		h = h.hex().encode()
		inodes.sort()
		for i in inodes:
			paths = itop[i]
			i = hex(i).lstrip('0Xx').encode()
			paths.sort()
			for p in paths:
				out.write(h)
				out.write(b'\t')
				out.write(i)
				out.write(b'\t')
				out.write(p)
				out.write(b'\n')


if __name__ == '__main__':
	main(*sys.argv[1:])
