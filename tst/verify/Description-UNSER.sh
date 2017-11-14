# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# rese: 2017/Oct/13
# Version: 1.0.00
# Content: Testing file for commiting.
# Description: Tests descriptor UNSER.
# Future add-on's: many.

if [ "$1" != "nocompile" ]
then
    make Description-UNSER $@
fi

./bin/Description-UNSER res/Description-test-images/Texture/cameraman.pgm > im01UNSER.txt
./bin/Description-UNSER res/Description-test-images/Texture/house.pgm > im02UNSER.txt
./bin/Description-UNSER res/Description-test-images/Texture/jetplane.pgm > im03UNSER.txt
./bin/Description-UNSER res/Description-test-images/Texture/lake.pgm > im04UNSER.txt
./bin/Description-UNSER res/Description-test-images/Texture/lena_color_256.pgm > im05UNSER.txt
./bin/Description-UNSER res/Description-test-images/Texture/lena_color_512.pgm > im06UNSER.txt
./bin/Description-UNSER res/Description-test-images/Texture/lena_gray_256.pgm > im07UNSER.txt
./bin/Description-UNSER res/Description-test-images/Texture/lena_gray_512.pgm > im08UNSER.txt
./bin/Description-UNSER res/Description-test-images/Texture/livingroom.pgm > im09UNSER.txt
./bin/Description-UNSER res/Description-test-images/Texture/mandril_color.pgm > im10UNSER.txt
./bin/Description-UNSER res/Description-test-images/Texture/mandril_gray.pgm > im11UNSER.txt
./bin/Description-UNSER res/Description-test-images/Texture/peppers_gray.pgm > im12UNSER.txt
./bin/Description-UNSER res/Description-test-images/Texture/pirate.pgm > im13UNSER.txt
./bin/Description-UNSER res/Description-test-images/Texture/walkbridge.pgm > im14UNSER.txt
./bin/Description-UNSER res/Description-test-images/Texture/woman_blonde.pgm > im15UNSER.txt
./bin/Description-UNSER res/Description-test-images/Texture/woman_darkhair.pgm > im16UNSER.txt

cat  *UNSER.txt> dat/unser.txt
rm *UNSER.txt


cmp=`diff dat/unser.txt res/Description-expected-results/Texture/unser.txt`

if [ "1$cmp" == "1" ]
then
    echo "Description-UNSER............ ok."
else
    echo "Description-UNSER............ failed."
fi
