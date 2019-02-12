#!/usr/bin/python3
# vim:ft=python:noet:sw=4:ts=4
import fileinput
import sys


try:
	dict_iteritems = dict.iteritems
except AttributeError:
	dict_iteritems = dict.items


def compact_ssh_known_hosts(iterlines):
	hostdict = {}
	markerdict = {}
	commentlines = []
	if isinstance(iterlines, dict):
		iterlines = (','.join(v) + ' ' + ' '.join(k) for k,v in dict_iteritems(iterlines))
	for rawline in iterlines:
		if rawline and '\n' == rawline[-1]:
			rawline = rawline[:-1]
		line = rawline.strip()
		if (not line) or '#' == line[0]:
			commentlines.append(rawline)
			continue
		del rawline
		if '@' == line[0]:
			line = line.split(None, 3)
			hosts = dict.fromkeys(line.pop(1).split(','))
			line = tuple(line)
			try:
				markerdict[line].update(hosts)
			except KeyError:
				markerdict[line] = hosts
			continue
		line = line.split(None, 2)
		hosts = dict.fromkeys(line.pop(0).split(','))
		line = tuple(line)
		try:
			hostdict[line].update(hosts)
		except KeyError:
			hostdict[line] = hosts
	return (hostdict, markerdict, commentlines)


def main(*args):
	fin = fileinput.FileInput(args)
	del args
	hostdict, markerdict, commentlines = compact_ssh_known_hosts(fin)
	fin.close()
	del fin
	fout = sys.stdout
	for k,v in dict_iteritems(hostdict):
		fout.write(','.join(v))
		fout.write(' ')
		fout.write(' '.join(k))
		fout.write('\n')
	for k,v in dict_iteritems(markerdict):
		fout.write(k[0])
		fout.write(' ')
		fout.write(','.join(v))
		fout.write(' ')
		fout.write(' '.join(k[1:]))
		fout.write('\n')
	for line in commentlines:
		fout.write(line)
		fout.write('\n')


if __name__ == '__main__':
	main(*sys.argv[1:])
