#!/bin/bash

logfile="/var/log/Xorg.0.log"

grep "WW" "$logfile" | sed 's/^.*WW)/Warning:/' > X_info_warn.log
grep "II" "$logfile" | sed 's/^.*II)/Information:/' >> X_info_warn.log

cat X_info_warn.log
