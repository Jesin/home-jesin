#!/bin/bash
# ~/.bashrc

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

shopt -s histappend
HISTCONTROL=ignoreboth:erasedups
HISTSIZE=8192
export HISTCONTROL HISTSIZE
PS1='\[\e[0m\][\u@\h \w]${?#0}\$ '
#PROMPT_COMMAND="history -n; history -w; history -c; history -r; $PROMPT_COMMAND"
