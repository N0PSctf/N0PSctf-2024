#!/bin/bash -x

convert -background cyan -fill orange -pointsize 72 \
          -size 1500x caption:`cat flag.txt` \
          flag.tiff

./felix_smart flag.tiff > flag.enc
