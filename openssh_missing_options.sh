#!/bin/bash
# set -x
CONFIG=`mktemp`

# pull a client options from manual page
OPTS=`grep "\.It Cm " ssh_config.5 | cut -f 3 -d " "`
for T in $OPTS; do
	grep "$T" readconf.c | grep -q dump_ || \
		grep -i $T readconf.c | grep -q "printf(" || \
		./ssh -G test -F /dev/null | grep -q -i "$T" || \
		echo "Missing dump of $T option in ssh_config";
done
# reports some false-positives

# pull a server options from manual page
KEY=`mktemp`
rm "$KEY"
ssh-keygen -f "$KEY" -N "" > /dev/null
echo "HostKey $KEY" > $CONFIG
OPTS=`grep "\.It Cm " sshd_config.5 | cut -f 3 -d " "`
for T in $OPTS; do
	grep "$T" servconf.c | grep -q dump_ || \
		grep -i $T servconf.c | grep -q -i "printf(" || \
		./sshd -T -f $CONFIG | grep -q -i "$T" || \
		echo "Missing dump of $T option in sshd_config";
done
rm $CONFIG
rm $KEY
