#!/bin/sh
# gets called by wofi on every userinput change
# $1 passed is the search_text
# output is used as label
if [[ $1 == \?\?* ]]; then
	echo -n "Search: ${1:2}"
fi

if [[ $1 == \=\=* ]]; then
	expr="${1:2}"
    echo -n "$(echo "$expr" | bc -l)"
	exit 0
fi

exit 0
