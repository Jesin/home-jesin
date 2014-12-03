#!/bin/sh
[ xtrue = "x$JESIN_PROFILE_WAS_SOURCED" ] && return 0

[ -r ~/.profile.local ] && . ~/.profile.local

{
	alias packer='packer --preview'
	alias cp='cp -i'
	alias mv='mv -i'
	alias rm='rm -i'

	[ "x$TERM" != xdumb ] && [ -z "$ls_options" ] && ls --color=auto /dev/null >&2 && ls_options='--color=auto'

	#a1# execute \kbd{@a@}:\quad ls with colors
	alias ls="ls -bCF $ls_options"
	#a1# execute \kbd{@a@}:\quad list all files, with colors
	alias la="ls -la"
	alias lA="ls -lA"
	#a1# long colored list, without dotfiles (@a@)
	alias ll="ls -l"
	#a1# long colored list, human readable sizes (@a@)
	alias lh="ls -hAl"
	#a1# List files, append qualifier to filenames \\&\quad(\kbd{/} for directories, \kbd{@} for symlinks ...)
	alias l="ls -lF"

	command -v vim >&2 && export VISUAL='vim -p'
	[ -z "$VISUAL" ] || export EDITOR=$VISUAL
	[ -n "$DISPLAY" ] || export DISPLAY=':0.0'
	[ -n "$BROWSER" ] || export BROWSER="$(command -v xdg-open):$(command -v chromium):$(command -v firefox):$(command -v elinks):$(command -v links):$(command -v w3m)"
	[ -n "$OMP_NUM_THREADS" ] || export OMP_NUM_THREADS="$(nproc || grep -c '^processor\>' /proc/cpuinfo || printf %s 2)"
	[ -n "$NPROC" ] || export NPROC=$OMP_NUM_THREADS
	[ -n "$MAKEFLAGS" ] || export MAKEFLAGS=-j$NPROC
	#[ -n "$GHCRTS" ] || export GHCRTS=-N
	[ -n "$PACMAN" ] || command -v pacmatic >&2 && export PACMAN=pacmatic

	#eval "$(/usr/bin/keychain --inherit any-once --eval -Q -q --noask id_ecdsa id_rsa)" >&2 || true
} 2>/dev/null

JESIN_PROFILE_WAS_SOURCED=true
