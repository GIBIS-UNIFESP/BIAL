# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# rese: 2017/Oct/13
# Version: 1.0.00
# Content: Testing file for commiting.
# Description: Tests descriptor EHD.
# Future add-on's: many.

if [ "$1" != "nocompile" ]
then
    make Description-EHD $@
fi

./bin/Description-EHD res/Description-test-images/Texture/cameraman.pgm > im01EHD.txt
./bin/Description-EHD res/Description-test-images/Texture/house.pgm > im02EHD.txt
./bin/Description-EHD res/Description-test-images/Texture/jetplane.pgm > im03EHD.txt
./bin/Description-EHD res/Description-test-images/Texture/lake.pgm > im04EHD.txt
./bin/Description-EHD res/Description-test-images/Texture/lena_color_256.pgm > im05EHD.txt
./bin/Description-EHD res/Description-test-images/Texture/lena_color_512.pgm > im06EHD.txt
./bin/Description-EHD res/Description-test-images/Texture/lena_gray_256.pgm > im07EHD.txt
./bin/Description-EHD res/Description-test-images/Texture/lena_gray_512.pgm > im08EHD.txt
./bin/Description-EHD res/Description-test-images/Texture/livingroom.pgm > im09EHD.txt
./bin/Description-EHD res/Description-test-images/Texture/mandril_color.pgm > im10EHD.txt
./bin/Description-EHD res/Description-test-images/Texture/mandril_gray.pgm > im11EHD.txt
./bin/Description-EHD res/Description-test-images/Texture/peppers_gray.pgm > im12EHD.txt
./bin/Description-EHD res/Description-test-images/Texture/pirate.pgm > im13EHD.txt
./bin/Description-EHD res/Description-test-images/Texture/walkbridge.pgm > im14EHD.txt
./bin/Description-EHD res/Description-test-images/Texture/woman_blonde.pgm > im15EHD.txt
./bin/Description-EHD res/Description-test-images/Texture/woman_darkhair.pgm > im16EHD.txt

cat  *EHD.txt> dat/ehd.txt
rm *EHD.txt


cmp=`diff dat/ehd.txt res/Description-expected-results/Texture/ehd.txt`

if [ "1$cmp" == "1" ]
then
    echo "Description-EHD............ ok."
else
    echo "Description-EHD............ failed."
fi
