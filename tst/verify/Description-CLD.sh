# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# rese: 2017/Oct/13
# Version: 1.0.00
# Content: Testing file for commiting.
# Description: Tests descriptor CLD.
# Future add-on's: many.

if [ "$1" != "nocompile" ]
then
    make Description-CLD $@
fi

./bin/Description-CLD res/Description-test-images/Color/cameraman.ppm > im01CLD.txt
./bin/Description-CLD res/Description-test-images/Color/house.ppm > im02CLD.txt
./bin/Description-CLD res/Description-test-images/Color/jetplane.ppm > im03CLD.txt
./bin/Description-CLD res/Description-test-images/Color/lake.ppm > im04CLD.txt
./bin/Description-CLD res/Description-test-images/Color/lena_color_256.ppm > im05CLD.txt
./bin/Description-CLD res/Description-test-images/Color/lena_color_512.ppm > im06CLD.txt
./bin/Description-CLD res/Description-test-images/Color/lena_gray_256.ppm > im07CLD.txt
./bin/Description-CLD res/Description-test-images/Color/lena_gray_512.ppm > im08CLD.txt
./bin/Description-CLD res/Description-test-images/Color/livingroom.ppm > im09CLD.txt
./bin/Description-CLD res/Description-test-images/Color/mandril_color.ppm > im10CLD.txt
./bin/Description-CLD res/Description-test-images/Color/mandril_gray.ppm > im11CLD.txt
./bin/Description-CLD res/Description-test-images/Color/peppers_gray.ppm > im12CLD.txt
./bin/Description-CLD res/Description-test-images/Color/pirate.ppm > im13CLD.txt
./bin/Description-CLD res/Description-test-images/Color/walkbridge.ppm > im14CLD.txt
./bin/Description-CLD res/Description-test-images/Color/woman_blonde.ppm > im15CLD.txt
./bin/Description-CLD res/Description-test-images/Color/woman_darkhair.ppm > im16CLD.txt

cat  *CLD.txt> dat/cld.txt
rm *CLD.txt


cmp=`diff dat/cld.txt res/Description-expected-results/Color/cld.txt`

if [ "1$cmp" == "1" ]
then
    echo "Description-CLD............ ok."
else
    echo "Description-CLD............ failed."
fi
