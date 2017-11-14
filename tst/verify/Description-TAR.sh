# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# rese: 2017/Oct/13
# Version: 1.0.00
# Content: Testing file for commiting.
# Description: Tests descriptor TAR.
# Future add-on's: many.

if [ "$1" != "nocompile" ]
then
    make Description-TAR $@
fi

./bin/Description-TAR res/Description-test-images/Shape/apple.pgm > im01TAR.txt
./bin/Description-TAR res/Description-test-images/Shape/bat.pgm > im02TAR.txt
./bin/Description-TAR res/Description-test-images/Shape/beetle.pgm > im03TAR.txt
./bin/Description-TAR res/Description-test-images/Shape/bird.pgm > im04TAR.txt
./bin/Description-TAR res/Description-test-images/Shape/Bone-8.pgm > im05TAR.txt
./bin/Description-TAR res/Description-test-images/Shape/brick-01.pgm > im06TAR.txt
./bin/Description-TAR res/Description-test-images/Shape/carriage-16.pgm > im07TAR.txt
./bin/Description-TAR res/Description-test-images/Shape/square.pgm > im08TAR.txt

cat  *TAR.txt> dat/tar.txt
rm *TAR.txt


cmp=`diff dat/tar.txt res/Description-expected-results/Shape/tar.txt`

if [ "1$cmp" == "1" ]
then
    echo "Description-TAR............ ok."
else
    echo "Description-TAR............ failed."
fi
