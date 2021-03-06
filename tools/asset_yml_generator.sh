#!/bin/bash
# This script is part of the SwordLord DANG Game Framework
# (c) 2020-2021 by SwordLord - the coding crew

# Check correct amount of parameters
if (( $# != 2 )); then
    echo "Illegal number of parameters"
    exit
fi

# Param 1 is the file to write into, param 2 is the file list to search through
FILE=$1
_dfiles=$2

# remove the old file, although there should be none laying around
if [ -f "$FILE" ]; then
  rm $FILE
fi

# Open file descriptor (fd) 3 for read/write on a text file.
exec 3<> $FILE

echo '# All these assets will be created in gfx.cpp, which CMake will'>&3
echo '# link automagically.'>&3
echo '# References can be picked up by including gfx.hpp'>&3
echo ''>&3
echo 'gfx.cpp:'>&3

for f in $_dfiles
do
  SOURCE=$(echo 'cat //tileset/image/@source' | xmllint --shell $f | awk -F\" 'NR % 2 == 0 { print $2 }')
  NAME=$(echo 'cat //tileset/@name' | xmllint --shell $f | awk -F\" 'NR % 2 == 0 { print $2 }')

  SOURCE=${SOURCE//..\/..\//}

  printf "  %s:\n    name: %s\n\n" $SOURCE $NAME >&3
done

# Close fd 3
exec 3>&-
