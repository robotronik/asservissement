#!/bin/sh

while read line
do
    echo 1>&2 "########################################"
    echo 1>&2 "$line"
    echo 1>&2 "########################################"

    echo "$line"
    sleep 0.5
done < $1
