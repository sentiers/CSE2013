#! /bin/bash
# This is a sample Born shell script.
echo Current Time: "$(date +%H)시 $(date +%M)분 $(date +%S)초"
who --count > usercount
echo "$(echo User Count: )" "$(sed -n 2p usercount)"
du -hs > filesize
echo File Size: "$(<filesize)"