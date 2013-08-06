#!/bin/sh
[ xtrue = "x$JESIN_PROFILE_WAS_SOURCED" ] && return 0

alias packer='packer --preview'

if command -v lockit >/dev/null 2>&1; then
	alias slock=lockit
elif command -v i3lock >/dev/null 2>&1; then
	alias lockit='i3lock -dc 000000'
	#alias lockit='i3lock -dc 082211'
	alias slock=lockit
elif command -v slock >/dev/null 2>&1; then
	alias lockit=slock
fi


[ "x$TERM" != xdumb ] && [ -z "$ls_options" ] && ls --color=auto /dev/null >/dev/null 2>&1 && ls_options='--color=auto'

#a1# execute \kbd{@a@}:\quad ls with colors
alias ls="ls -b -CF $ls_options"
#a1# execute \kbd{@a@}:\quad list all files, with colors
alias la="ls -la $ls_options"
#a1# long colored list, without dotfiles (@a@)
alias ll="ls -l $ls_options"
#a1# long colored list, human readable sizes (@a@)
alias lh="ls -hAl $ls_options"
#a1# List files, append qualifier to filenames \\&\quad(\kbd{/} for directories, \kbd{@} for symlinks ...)
alias l="ls -lF $ls_options"

[ -z "$BROWSER" ] && export BROWSER="$(command -v xdg-open 2>/dev/null):$(command -v chromium 2>/dev/null):$(command -v firefox 2>/dev/null):$(command -v elinks 2>/dev/null):$(command -v links 2>/dev/null)"
[ -z "$DISPLAY" ] && export DISPLAY=':0.0'
[ -z "${OMP_NUM_THREADS}" ] && export OMP_NUM_THREADS="$(nproc 2>/dev/null || grep -c '^processor\>' /proc/cpuinfo 2>/dev/null)"
[ -z "$NPROC" ] && export NPROC="${OMP_NUM_THREADS}"
[ -z "$MAKEFLAGS" ] && export MAKEFLAGS="-j$((NPROC+1))"

[ -z "$PACMAN" ] && command -v pacmatic >/dev/null 2>&1 && export PACMAN=pacmatic

eval "$(/usr/bin/keychain --inherit any-once --eval -Q -q --noask id_ecdsa id_rsa 2> /dev/null)" >/dev/null 2>&1 || true

echo 'Make sure you know exactly how many problems you have to do.'
echo 'Even if you have done most of it, there might be more left than you think.'

[ -r ~/.profile.local ] && . ~/.profile.local

JESIN_PROFILE_WAS_SOURCED=true
