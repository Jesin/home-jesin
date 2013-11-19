#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

[ -f ~/.profile ] && [ -r ~/.profile ] && . ~/.profile
export HISTCONTROL=ignoreboth
PS1='[\u@\h \W]\$ '
