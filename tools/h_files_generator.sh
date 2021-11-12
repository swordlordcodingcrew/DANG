#!/bin/bash
# This script is part of the SwordLord DANG Game Framework
# (c) 2020-2021 by SwordLord - the coding crew

# Check correct amount of parameters
if (( $# != 1 )); then
    echo "Illegal number of parameters"
    exit
fi

# Param 1 is the file to write into, param 2 is the file list to search through
_dfiles=$1

echo "Add these files to your CMakeList.txt"

for f in $_dfiles
do
  SOURCE=$(echo 'cat //tileset/image/@source' | xmllint --shell $f | awk -F\" 'NR % 2 == 0 { print $2 }')
  NAME=$(echo 'cat //tileset/@name' | xmllint --shell $f | awk -F\" 'NR % 2 == 0 { print $2 }')

  SOURCE=${SOURCE//..\/..\//}

  OLDIFS=$IFS
  IFS='/'		# hyphen (-) is set as delimiter
  read -ra ARR <<< "$SOURCE"	# str is read into an array as tokens separated by IFS
  IFS=$OLDIFS		# reset to default value after usage

  FILENAME=${ARR[-1]}

#  python3 -m ttblit --debug  pack --force --config ${CMAKE_CURRENT_SOURCE_DIR}/${FILE} --output ${CMAKE_CURRENT_BINARY_DIR}

  #PALETTEFILE=$(echo "$SOURCE" | sed 's/.png/.gpl/')
  #if [ -f "$PALETTEFILE" ]; then

    # switched to our own aseprite exporter
    ~/dev/apps/aseprite/aseprite -b -v $SOURCE -script-param symbol_name=$NAME -script-param output_file=rsrc/gfx/$FILENAME.h -script ~/.config/aseprite/scripts/aseprite_dang_exporter.lua
    #~/data/development/aseprite/build/bin/aseprite -b -v $SOURCE -script-param symbol_name=$NAME -script-param output_file=rsrc/gfx/$FILENAME.h -script ~/.config/aseprite/scripts/aseprite_dang_exporter.lua
    #python3 -m ttblit image --input_file $SOURCE --input_type image --output_file rsrc/gfx/$FILENAME.h --output_format c_header --symbol_name $NAME --force
    echo "rsrc/gfx/$FILENAME.h"

done
