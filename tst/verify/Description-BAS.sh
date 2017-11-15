# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# rese: 2017/Oct/13
# Version: 1.0.00
# Content: Testing file for commiting.
# Description: Tests descriptor BAS.
# Future add-on's: many.

if [ "$1" != "nocompile" ]
then
    make Description-BAS $@
fi

./bin/Description-BAS res/Description-test-images/Shape/apple.pgm > im01BAS.txt
./bin/Description-BAS res/Description-test-images/Shape/bat.pgm > im02BAS.txt
./bin/Description-BAS res/Description-test-images/Shape/beetle.pgm > im03BAS.txt
./bin/Description-BAS res/Description-test-images/Shape/bird.pgm > im04BAS.txt
./bin/Description-BAS res/Description-test-images/Shape/Bone-8.pgm > im05BAS.txt
./bin/Description-BAS res/Description-test-images/Shape/brick-01.pgm > im06BAS.txt
./bin/Description-BAS res/Description-test-images/Shape/carriage-16.pgm > im07BAS.txt
./bin/Description-BAS res/Description-test-images/Shape/square.pgm > im08BAS.txt

cat  *BAS.txt> dat/bas.txt
rm *BAS.txt


cmp=`diff dat/bas.txt res/Description-expected-results/Shape/bas.txt`

if [ "1$cmp" == "1" ]
then
    echo "Description-BAS............ ok."
else
    echo "Description-BAS............ failed."
fi
