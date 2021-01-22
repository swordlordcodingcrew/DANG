#!/bin/bash

FILE="test.yml"
_dfiles="world/**/*.tsx"

rm $FILE

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
