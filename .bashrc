#!/bin/bash
# ~/.bashrc

# If not running interactively, don't do anything
case "$-" in
 *i*) ;;
 *) return 0 ;;
esac

shopt -s histappend
HISTCONTROL=ignoreboth:erasedups
HISTSIZE=-1
export HISTCONTROL HISTSIZE
PS1='\[\e[0m\][\u@\h \w]${?#0}\$ '

jesBashSync() {
	history -n
	history -w
	history -c
	history -r
}

[ -r ~/.profile ] && . ~/.profile
