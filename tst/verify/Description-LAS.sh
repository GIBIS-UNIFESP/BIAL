# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# rese: 2017/Oct/13
# Version: 1.0.00
# Content: Testing file for commiting.
# Description: Tests descriptor LAS.
# Future add-on's: many.

if [ "$1" != "nocompile" ]
then
    make Description-LAS $@
fi

./bin/Description-LAS res/Description-test-images/Texture/cameraman.pgm > im01LAS.txt
./bin/Description-LAS res/Description-test-images/Texture/house.pgm > im02LAS.txt
./bin/Description-LAS res/Description-test-images/Texture/jetplane.pgm > im03LAS.txt
./bin/Description-LAS res/Description-test-images/Texture/lake.pgm > im04LAS.txt
./bin/Description-LAS res/Description-test-images/Texture/lena_color_256.pgm > im05LAS.txt
./bin/Description-LAS res/Description-test-images/Texture/lena_color_512.pgm > im06LAS.txt
./bin/Description-LAS res/Description-test-images/Texture/lena_gray_256.pgm > im07LAS.txt
./bin/Description-LAS res/Description-test-images/Texture/lena_gray_512.pgm > im08LAS.txt
./bin/Description-LAS res/Description-test-images/Texture/livingroom.pgm > im09LAS.txt
./bin/Description-LAS res/Description-test-images/Texture/mandril_color.pgm > im10LAS.txt
./bin/Description-LAS res/Description-test-images/Texture/mandril_gray.pgm > im11LAS.txt
./bin/Description-LAS res/Description-test-images/Texture/peppers_gray.pgm > im12LAS.txt
./bin/Description-LAS res/Description-test-images/Texture/pirate.pgm > im13LAS.txt
./bin/Description-LAS res/Description-test-images/Texture/walkbridge.pgm > im14LAS.txt
./bin/Description-LAS res/Description-test-images/Texture/woman_blonde.pgm > im15LAS.txt
./bin/Description-LAS res/Description-test-images/Texture/woman_darkhair.pgm > im16LAS.txt

cat  *LAS.txt> dat/las.txt
rm *LAS.txt


cmp=`diff dat/las.txt res/Description-expected-results/Texture/las.txt`

if [ "1$cmp" == "1" ]
then
    echo "Description-LAS............ ok."
else
    echo "Description-LAS............ failed."
fi
