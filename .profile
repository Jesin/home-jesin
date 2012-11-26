#alias lockit='i3lock -c 082211'
if command -v lockit >/dev/null 2>&1; then
	alias slock=lockit
elif command -v i3lock >/dev/null 2>&1; then
	alias lockit=i3lock -c 000000
	alias slock=lockit
elif command -v slock >/dev/null 2>&1; then
	alias lockit=slock
fi

if [ -n "$BROWSER" ] && command -v $BROWSER >/dev/null 2>&1; then
	true
elif command -v chromium >/dev/null 2>&1; then
	BROWSER=chromium
elif command -v firefox >/dev/null 2>&1; then
	BROWSER=firefox
fi
export BROWSER

eval $(keychain --eval --quiet --noask id_ecdsa id_rsa 2> /dev/null) >/dev/null 2>&1 || true
