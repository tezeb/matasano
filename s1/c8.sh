#!/bin/bash
MAX=0;
MAX_S='';

while read f; do
	V=`echo "$f" | sed -r 's/(.{32})/\1\n/g' | sort | uniq -c | cut -f 7 -d ' ' | sort -ru | head -1`;
	if [[ $V -gt $MAX ]]; then
		MAX="$V";
		MAX_S="$f";
	fi;
done < 8.txt;
echo -e "Max count: $MAX\n$MAX_S"
