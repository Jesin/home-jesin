#!/bin/sh
[ -n "$JESIN_PROFILE_WAS_SOURCED" ] && return
JESIN_PROFILE_WAS_SOURCED=1

{
	alias cp='cp -i' mv='mv -i' #rm='rm -i'

	[ dumb != "$TERM" ] && [ -z "$ls_options" ] && ls --color=auto -Fb /dev/null && ls_options='--color=auto -Fb' || ls_options=-F
	alias ls="ls $ls_options"
	alias l='ls -l' la='ls -lA' lA='ls -lA' lh='ls -lAh'

	command -v vim && export VISUAL='vim -p'
	[ -z "$VISUAL" ] || export EDITOR="$VISUAL"
	[ -n "${LESS+x}" ] || export LESS=RX
	[ -n "$DISPLAY" ] || export DISPLAY=':0'
	[ -n "$BROWSER" ] || export BROWSER="$(command -v chromium):$(command -v firefox):$(command -v midori):$(command -v konqueror):$(command -v epiphany):$(command -v luakit):$(command -v surf):$(command -v elinks):$(command -v links):$(command -v w3m)"
	[ -n "$NPROC" ] || export NPROC="$(nproc || getconf _NPROCESSORS_ONLN || grep -c '^processor\>' </proc/cpuinfo || printf %s 3)"
	[ -n "$OMP_NUM_THREADS" ] || export OMP_NUM_THREADS="$NPROC"
	[ -n "$MAKEFLAGS" ] || export MAKEFLAGS="-j$NPROC"
	#[ -n "$GHCRTS" ] || export GHCRTS=-N
	[ -z "$PACMAN" ] && command -v pacmatic && export PACMAN=pacmatic
} >/dev/null 2>&1

[ -r ~/.profile.local ] && . ~/.profile.local
