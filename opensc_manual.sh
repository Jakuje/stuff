#!/bin/bash

pkcs11-tool 2>&1 | awk '{if (match($0,"--[a-zA-Z0-9-]*",a) != 0) print a[0]} {if (match($0," -[a-zA-Z0-9]",a) != 0) print a[0]}' > pkcs11-tool.switches
while read SW; do
	grep -q -- "$SW" ~/trunk/OpenSC/doc/tools/pkcs11-tool.1.xml || echo "missing $SW"
done < pkcs11-tool.switches
