#!/usr/bin/zsh

if [ $# -lt 4 ] ; then
    echo "Usage: $0 [path_to_img_directory] [img_constant_name] [nof_digits_in_img_postfix] [path_to_output_dir]"
    exit
fi

deletions=$(find ${4} -name "*.mp4" -print)

if [[ "$deletions" != "" ]] ; then
    echo "existing mp4 files were found: ${deletions}"
    echo -n "Delete? (y/n) "
    read answer

    if [[ "$answer" == "y" || "$answer" == "Y" ]] ; then
        rm $deletions
    else
        echo "Not deleting, exiting."
        exit
    fi
fi

ffmpeg -framerate 120 -i ${1}${2}_%0${3}d.png -c:v libx264 -profile:v high -crf 20 -pix_fmt yuv420p ${4}${2}_output.mp4

