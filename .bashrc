#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

[ -f ~/.profile ] && [ -r ~/.profile ] && . ~/.profile
export HISTCONTROL=erasedups:ignoreboth HISTSIZE=8192
PS1='\[\e[0m\][\u@\h \w]${?#0}\$ '
