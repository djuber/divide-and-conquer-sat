#!/bin/sh
# build cnf.exe : move to parent
make windows
cp cnf.exe ..
make clean

# create zip archive of current directory
cd ../..
rm -f CS408-Project1-Uber.zip;
zip -r CS408-Project1-Uber.zip divide-and-conquer-sat

rm -f divide-and-conquer-sat/cnf.exe
