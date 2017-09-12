#!/bin/bash
# ~/.bash_logout
case "$-" in
 *i*)
	history -n
	history -w
	history -c
 ;;
esac
