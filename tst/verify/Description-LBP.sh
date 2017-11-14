# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# rese: 2017/Oct/13
# Version: 1.0.00
# Content: Testing file for commiting.
# Description: Tests descriptor LBP.
# Future add-on's: many.

if [ "$1" != "nocompile" ]
then
    make Description-LBP $@
fi

./bin/Description-LBP res/Description-test-images/Texture/cameraman.pgm > im01LBP.txt
./bin/Description-LBP res/Description-test-images/Texture/house.pgm > im02LBP.txt
./bin/Description-LBP res/Description-test-images/Texture/jetplane.pgm > im03LBP.txt
./bin/Description-LBP res/Description-test-images/Texture/lake.pgm > im04LBP.txt
./bin/Description-LBP res/Description-test-images/Texture/lena_color_256.pgm > im05LBP.txt
./bin/Description-LBP res/Description-test-images/Texture/lena_color_512.pgm > im06LBP.txt
./bin/Description-LBP res/Description-test-images/Texture/lena_gray_256.pgm > im07LBP.txt
./bin/Description-LBP res/Description-test-images/Texture/lena_gray_512.pgm > im08LBP.txt
./bin/Description-LBP res/Description-test-images/Texture/livingroom.pgm > im09LBP.txt
./bin/Description-LBP res/Description-test-images/Texture/mandril_color.pgm > im10LBP.txt
./bin/Description-LBP res/Description-test-images/Texture/mandril_gray.pgm > im11LBP.txt
./bin/Description-LBP res/Description-test-images/Texture/peppers_gray.pgm > im12LBP.txt
./bin/Description-LBP res/Description-test-images/Texture/pirate.pgm > im13LBP.txt
./bin/Description-LBP res/Description-test-images/Texture/walkbridge.pgm > im14LBP.txt
./bin/Description-LBP res/Description-test-images/Texture/woman_blonde.pgm > im15LBP.txt
./bin/Description-LBP res/Description-test-images/Texture/woman_darkhair.pgm > im16LBP.txt

cat  *LBP.txt> dat/lbp.txt
rm *LBP.txt


cmp=`diff dat/lbp.txt res/Description-expected-results/Texture/lbp.txt`

if [ "1$cmp" == "1" ]
then
    echo "Description-LBP............ ok."
else
    echo "Description-LBP............ failed."
fi
