#!/bin/sh
# Script gets called when executing the external entry in wofi
# param $1 is the search_text from wofi
if [[ $1 == \?\?* ]]; then
	firefox --new-window "google.com/search?q=${1:2}"
fi
