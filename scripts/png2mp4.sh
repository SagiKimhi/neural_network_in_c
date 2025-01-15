#!/bin/zsh

if [ $# -lt 2 ] ; then
    echo "Usage: $0 [input-dir] [output-filename]"
    echo "Produces an mp4 animation video from a directory of sequential png images"
    exit
fi

ffmpeg -framerate 60 -i $1/*_%05d.png -c:v libx264 -profile:v high -crf 20 -pix_fmt yuv420p $2.png
