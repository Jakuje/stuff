#!/bin/bash
# set -x

# pull a client options from manual page
OPTS=`grep "\.It Cm " ssh_config.5 | cut -f 3 -d " "`
for T in $OPTS; do
	grep "$T" readconf.c | grep -q dump_ || \
		grep -i $T readconf.c | grep -q "printf(" || \
		echo "Missing dump of $T option in ssh_config";
done
# reports some false-positives

# pull a server options from manual page
OPTS=`grep "\.It Cm " sshd_config.5 | cut -f 3 -d " "`
for T in $OPTS; do
	grep "$T" servconf.c | grep -q dump_ || \
		grep -i $T servconf.c | grep -q -i "printf(" || \
		echo "Missing dump of $T option in sshd_config";
done
