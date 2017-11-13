#!/bin/sh
[ -n "$JESIN_PROFILE_WAS_SOURCED" ] && return
JESIN_PROFILE_WAS_SOURCED=1

{
	alias cp='cp -i' ls='ls -F' mv='mv -i'
	[ dumb != "$TERM" ] && ls --color=auto -Fb /dev/null && alias ls='ls --color=auto -Fb'
	alias l='ls -l' la='ls -lA' lA='ls -lA' lh='ls -lAh'

	hash vim && export VISUAL='vim -p'
	[ -z "$VISUAL" ] || export EDITOR="$VISUAL"
	[ -n "${LESS+x}" ] || export LESS=RX
	[ -n "$DISPLAY" ] || export DISPLAY=':0'
	[ -n "$BROWSER" ] || export BROWSER="$(command -v chromium):$(command -v firefox):$(command -v midori):$(command -v konqueror):$(command -v epiphany):$(command -v luakit):$(command -v surf):$(command -v elinks):$(command -v links):$(command -v w3m)"
	[ -n "$NPROC" ] || export NPROC="$(nproc || getconf _NPROCESSORS_ONLN || grep -c '^processor\>' </proc/cpuinfo || printf %s 3)"
	[ -n "$OMP_NUM_THREADS" ] || export OMP_NUM_THREADS="$NPROC"
	[ -n "$MAKEFLAGS" ] || export MAKEFLAGS="-j$NPROC"
	#[ -n "$GHCRTS" ] || export GHCRTS=-N
	[ -z "$PACMAN" ] && hash pacmatic && export PACMAN=pacmatic
	[ -n "$TMUX_TMPDIR" ] || [ -z "$XDG_RUNTIME_DIR" ] || export TMUX_TMPDIR="$XDG_RUNTIME_DIR"
	[ -n "$XDG_CACHE_HOME" ] || export XDG_CACHE_HOME="$HOME/.cache"
	[ -n "$XDG_CONFIG_HOME" ] || export XDG_CONFIG_HOME="$HOME/.config"
	[ -n "$XDG_DATA_HOME" ] || export XDG_DATA_HOME="$HOME/.local/share"
} >/dev/null 2>&1

[ -r ~/.profile.local ] && . ~/.profile.local
