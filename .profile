#!/bin/sh
[ xtrue = "x$JESIN_PROFILE_WAS_SOURCED" ] && return 0

{
	alias packer='packer --preview'
	alias cp='cp -i'
	alias mv='mv -i'
	alias rm='rm -i'

	if command -v lockit >&2; then
		alias slock=lockit
	elif command -v i3lock >&2; then
		alias lockit='i3lock -dc 000000'
		alias slock=lockit
	elif command -v slock >&2; then
		alias lockit=slock
	fi

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

	set +a
	command -v vim >&2 && VISUAL='vim -p'
	[ -z "$VISUAL" ] || EDITOR=$VISUAL
	[ -n "$DISPLAY" ] || DISPLAY=':0.0'
	[ -n "$BROWSER" ] || BROWSER="$(command -v xdg-open):$(command -v chromium):$(command -v firefox):$(command -v elinks):$(command -v links):$(command -v w3m)"
	[ -n "$OMP_NUM_THREADS" ] || OMP_NUM_THREADS="$(nproc || grep -c '^processor\>' /proc/cpuinfo || printf %s 3)"
	[ -n "$NPROC" ] || NPROC=$OMP_NUM_THREADS
	[ -n "$MAKEFLAGS" ] || MAKEFLAGS=-j$NPROC
	#[ -n "$GHCRTS" ] || GHCRTS=-N
	[ -n "$PACMAN" ] || command -v pacmatic >&2 && PACMAN=pacmatic
	set -a

	#eval "$(/usr/bin/keychain --inherit any-once --eval -Q -q --noask id_ecdsa id_rsa)" >&2 || true
	#printf %s\\n 'Make sure you know exactly how many problems you have to do.' 'Even if you have done most of it, there might be more left than you think.'
} 2>/dev/null

[ -r ~/.profile.local ] && . ~/.profile.local

JESIN_PROFILE_WAS_SOURCED=true
