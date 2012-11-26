#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return


[ -r ~/.profile ] && . ~/.profile
export HISTCONTROL=ignoreboth
alias ls='ls --color=auto'
PS1='[\u@\h \W]\$ '
