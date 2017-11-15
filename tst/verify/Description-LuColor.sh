# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# rese: 2017/Oct/13
# Version: 1.0.00
# Content: Testing file for commiting.
# Description: Tests descriptor LuColor.
# Future add-on's: many.

if [ "$1" != "nocompile" ]
then
    make Description-LuColor $@
fi

./bin/Description-LuColor res/Description-test-images/Color/cameraman.ppm > im01LuColor.txt
./bin/Description-LuColor res/Description-test-images/Color/house.ppm > im02LuColor.txt
./bin/Description-LuColor res/Description-test-images/Color/jetplane.ppm > im03LuColor.txt
./bin/Description-LuColor res/Description-test-images/Color/lake.ppm > im04LuColor.txt
./bin/Description-LuColor res/Description-test-images/Color/lena_color_256.ppm > im05LuColor.txt
./bin/Description-LuColor res/Description-test-images/Color/lena_color_512.ppm > im06LuColor.txt
./bin/Description-LuColor res/Description-test-images/Color/lena_gray_256.ppm > im07LuColor.txt
./bin/Description-LuColor res/Description-test-images/Color/lena_gray_512.ppm > im08LuColor.txt
./bin/Description-LuColor res/Description-test-images/Color/livingroom.ppm > im09LuColor.txt
./bin/Description-LuColor res/Description-test-images/Color/mandril_color.ppm > im10LuColor.txt
./bin/Description-LuColor res/Description-test-images/Color/mandril_gray.ppm > im11LuColor.txt
./bin/Description-LuColor res/Description-test-images/Color/peppers_gray.ppm > im12LuColor.txt
./bin/Description-LuColor res/Description-test-images/Color/pirate.ppm > im13LuColor.txt
./bin/Description-LuColor res/Description-test-images/Color/walkbridge.ppm > im14LuColor.txt
./bin/Description-LuColor res/Description-test-images/Color/woman_blonde.ppm > im15LuColor.txt
./bin/Description-LuColor res/Description-test-images/Color/woman_darkhair.ppm > im16LuColor.txt

cat  *LuColor.txt> dat/lucolor.txt
rm *LuColor.txt


cmp=`diff dat/lucolor.txt res/Description-expected-results/Color/lucolor.txt`

if [ "1$cmp" == "1" ]
then
    echo "Description-LuColor............ ok."
else
    echo "Description-LuColor............ failed."
fi
