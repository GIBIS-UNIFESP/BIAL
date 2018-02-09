# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# rese: 2017/Oct/13
# Version: 1.0.00
# Content: Testing file for commiting.
# Description: Tests descriptor LCH.
# Future add-on's: many.

if [ "$1" != "nocompile" ]
then
    make Description-LCH $@
fi

./bin/Description-LCH res/Description-test-images/Color/cameraman.ppm > im01LCH.txt
./bin/Description-LCH res/Description-test-images/Color/house.ppm > im02LCH.txt
./bin/Description-LCH res/Description-test-images/Color/jetplane.ppm > im03LCH.txt
./bin/Description-LCH res/Description-test-images/Color/lake.ppm > im04LCH.txt
./bin/Description-LCH res/Description-test-images/Color/lena_color_256.ppm > im05LCH.txt
./bin/Description-LCH res/Description-test-images/Color/lena_color_512.ppm > im06LCH.txt
./bin/Description-LCH res/Description-test-images/Color/lena_gray_256.ppm > im07LCH.txt
./bin/Description-LCH res/Description-test-images/Color/lena_gray_512.ppm > im08LCH.txt
./bin/Description-LCH res/Description-test-images/Color/livingroom.ppm > im09LCH.txt
./bin/Description-LCH res/Description-test-images/Color/mandril_color.ppm > im10LCH.txt
./bin/Description-LCH res/Description-test-images/Color/mandril_gray.ppm > im11LCH.txt
./bin/Description-LCH res/Description-test-images/Color/peppers_gray.ppm > im12LCH.txt
./bin/Description-LCH res/Description-test-images/Color/pirate.ppm > im13LCH.txt
./bin/Description-LCH res/Description-test-images/Color/walkbridge.ppm > im14LCH.txt
./bin/Description-LCH res/Description-test-images/Color/woman_blonde.ppm > im15LCH.txt
./bin/Description-LCH res/Description-test-images/Color/woman_darkhair.ppm > im16LCH.txt

cat  *LCH.txt> dat/lch.txt
rm *LCH.txt


cmp=`diff dat/lch.txt res/Description-expected-results/Color/lch.txt`

if [ "1$cmp" == "1" ]
then
    echo "Description-LCH............ ok."
else
    echo "Description-LCH............ failed."
fi
