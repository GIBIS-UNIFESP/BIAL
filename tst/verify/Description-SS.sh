# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# rese: 2017/Oct/13
# Version: 1.0.00
# Content: Testing file for commiting.
# Description: Tests descriptor SS.
# Future add-on's: many.

if [ "$1" != "nocompile" ]
then
    make Description-SS $@
fi

./bin/Description-SS res/Description-test-images/Shape/apple.pgm > im01SS.txt
./bin/Description-SS res/Description-test-images/Shape/bat.pgm > im02SS.txt
./bin/Description-SS res/Description-test-images/Shape/beetle.pgm > im03SS.txt
./bin/Description-SS res/Description-test-images/Shape/bird.pgm > im04SS.txt
./bin/Description-SS res/Description-test-images/Shape/Bone-8.pgm > im05SS.txt
./bin/Description-SS res/Description-test-images/Shape/brick-01.pgm > im06SS.txt
./bin/Description-SS res/Description-test-images/Shape/carriage-16.pgm > im07SS.txt
./bin/Description-SS res/Description-test-images/Shape/square.pgm > im08SS.txt

cat  *SS.txt> dat/ss.txt
rm *SS.txt


cmp=`diff dat/ss.txt res/Description-expected-results/Shape/ss.txt`

if [ "1$cmp" == "1" ]
then
    echo "Description-SS............ ok."
else
    echo "Description-SS............ failed."
fi
