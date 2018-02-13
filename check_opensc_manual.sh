#!/bin/bash
# set -x

SOURCE_PATH=~/devel/OpenSC/

# find all the manual pages in src/tools
TOOLS=`find "${SOURCE_PATH}/doc/tools" -name "*.1.xml" | sed -E -e "s|.*/([a-z0-9-]*).*|\1|"`

for T in $TOOLS; do
	SWITCHES=`${SOURCE_PATH}/src/tools/${T} 2>&1 | awk '{if (match($0,"--[a-zA-Z0-9-]*",a) != 0) print a[0]} {if (match($0," -[a-zA-Z0-9]",a) != 0) print a[0]}'`
	for S in $SWITCHES; do
		grep -q -- "$S" ${SOURCE_PATH}/doc/tools/${T}.1.xml || echo "${T}: missing switch $S"
	done
done
