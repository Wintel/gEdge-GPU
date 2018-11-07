#!/bin/bash
#ffmpeg -f image2 -r 30 -i tmp8%d.bmp ./out8d.mov

#ffmpeg -y -i out.mov -i out8d.mov -filter_complex '[0:v:0] [1:v:0]  concat=n=2:v=1 [outv]' -map '[outv]' out8d.mov

var=$1;

ffmpeg -y -f image2 -r 30 -i tmp%d.bmp ./$var

#while [[! -e "out.mov" ]]
#  do
#   echo "Hello"
#  done

#var=$((var + 1)) 
rm *.bmp
