#!/bin/sh
[ xtrue = "x$JESIN_PROFILE_WAS_SOURCED" ] && return 0

{
	[ -r /etc/profile.d/vte.sh ] && . /etc/profile.d/vte.sh

	alias packer='packer --preview'
	alias cp='cp -i' mv='mv -i' rm='rm -i'

	[ "x$TERM" != xdumb ] && [ -z "$ls_options" ] && ls --color=auto /dev/null >&2 && ls_options='--color=auto'
	alias ls="ls $ls_options -F"
	alias l='ls -l' la='ls -la' lA='ls -lA' lh='ls -lAh'

	command -v vim >&2 && export VISUAL='vim -p'
	[ -z "$VISUAL" ] || export EDITOR="$VISUAL"
	[ -n "$DISPLAY" ] || export DISPLAY=':0'
	[ -n "$BROWSER" ] || export BROWSER="$(command -v chromium):$(command -v firefox):$(command -v midori):$(command -v konqueror):$(command -v epiphany):$(command -v luakit):$(command -v surf):$(command -v elinks):$(command -v links):$(command -v w3m)"
	[ -n "$NPROC" ] || export NPROC="$(nproc || grep -c '^processor\>' /proc/cpuinfo || printf %s 2)"
	[ -n "$OMP_NUM_THREADS" ] || export OMP_NUM_THREADS="$NPROC"
	[ -n "$MAKEFLAGS" ] || export MAKEFLAGS="-j$NPROC"
	#[ -n "$GHCRTS" ] || export GHCRTS=-N
	[ -n "$PACMAN" ] || command -v pacmatic >&2 && export PACMAN=pacmatic
} 2>/dev/null

[ -r ~/.profile.local ] && . ~/.profile.local

JESIN_PROFILE_WAS_SOURCED=true
