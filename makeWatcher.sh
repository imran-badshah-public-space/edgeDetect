#!/bin/bash
# Watch for changes: https://stackoverflow.com/a/20436658

args=("$@")

# Check if the first argument is a number (watch time interval), else use the default 10 seconds
if (($# == 1))
then
    watchEvery=${args[1]}
else
    watchEvery=10
fi

# Make on first exec
make
# Monitor changes or file creations based on time interval
inotifywait -e create,modify --exclude '.*\.swp|.*\.git' -rmq . |
while read
do
    make
    sleep $watchEvery
done