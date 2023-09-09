#!/bin/bash

if [ $# -ne 4 ]; then
    echo "Usage: $0 <input.txt> <interval> <avg|var> <output.png>"
    exit 1
fi

INPUT=$1
INTERVAL=$2
STAT=$3
OUTPUT=$4

TEMP=$(mktemp)


cat "$INPUT" | ./stats --interval "$INTERVAL" --stat "$STAT" > "$TEMP"

gnuplot - <<EOF
set terminal png enhanced notransparent nointerlace truecolor font "Liberation, 20" size 2000,1400
set out "$OUTPUT"
set title "Graph Title"
set xlabel "X-axis Description"
set ylabel "Y-axis Description"
plot "< cat $TEMP" using (\$1/100) with lines title "Statistics"
set out
EOF

rm "$TEMP"
