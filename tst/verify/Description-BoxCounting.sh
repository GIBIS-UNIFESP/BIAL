# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# rese: 2017/Oct/13
# Version: 1.0.00
# Content: Testing file for commiting.
# Description: Tests descriptor BoxCounting.
# Future add-on's: many.

if [ "$1" != "nocompile" ]
then
    make Description-BoxCounting $@
fi

./bin/Description-BoxCounting res/Description-test-images/Shape/apple.pgm > im01BoxCounting.txt
./bin/Description-BoxCounting res/Description-test-images/Shape/bat.pgm > im02BoxCounting.txt
./bin/Description-BoxCounting res/Description-test-images/Shape/beetle.pgm > im03BoxCounting.txt
./bin/Description-BoxCounting res/Description-test-images/Shape/bird.pgm > im04BoxCounting.txt
./bin/Description-BoxCounting res/Description-test-images/Shape/Bone-8.pgm > im05BoxCounting.txt
./bin/Description-BoxCounting res/Description-test-images/Shape/brick-01.pgm > im06BoxCounting.txt
./bin/Description-BoxCounting res/Description-test-images/Shape/carriage-16.pgm > im07BoxCounting.txt
./bin/Description-BoxCounting res/Description-test-images/Shape/square.pgm > im08BoxCounting.txt

cat  *BoxCounting.txt> dat/boxcounting.txt
rm *BoxCounting.txt


cmp=`diff dat/boxcounting.txt res/Description-expected-results/Shape/boxcounting.txt`

if [ "1$cmp" == "1" ]
then
    echo "Description-BoxCounting............ ok."
else
    echo "Description-BoxCounting............ failed."
fi
