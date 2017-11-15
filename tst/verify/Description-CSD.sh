# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# rese: 2017/Oct/13
# Version: 1.0.00
# Content: Testing file for commiting.
# Description: Tests descriptor CSD.
# Future add-on's: many.

if [ "$1" != "nocompile" ]
then
    make Description-CSD $@
fi

./bin/Description-CSD res/Description-test-images/Color/cameraman.ppm > im01CSD.txt
./bin/Description-CSD res/Description-test-images/Color/house.ppm > im02CSD.txt
./bin/Description-CSD res/Description-test-images/Color/jetplane.ppm > im03CSD.txt
./bin/Description-CSD res/Description-test-images/Color/lake.ppm > im04CSD.txt
./bin/Description-CSD res/Description-test-images/Color/lena_color_256.ppm > im05CSD.txt
./bin/Description-CSD res/Description-test-images/Color/lena_color_512.ppm > im06CSD.txt
./bin/Description-CSD res/Description-test-images/Color/lena_gray_256.ppm > im07CSD.txt
./bin/Description-CSD res/Description-test-images/Color/lena_gray_512.ppm > im08CSD.txt
./bin/Description-CSD res/Description-test-images/Color/livingroom.ppm > im09CSD.txt
./bin/Description-CSD res/Description-test-images/Color/mandril_color.ppm > im10CSD.txt
./bin/Description-CSD res/Description-test-images/Color/mandril_gray.ppm > im11CSD.txt
./bin/Description-CSD res/Description-test-images/Color/peppers_gray.ppm > im12CSD.txt
./bin/Description-CSD res/Description-test-images/Color/pirate.ppm > im13CSD.txt
./bin/Description-CSD res/Description-test-images/Color/walkbridge.ppm > im14CSD.txt
./bin/Description-CSD res/Description-test-images/Color/woman_blonde.ppm > im15CSD.txt
./bin/Description-CSD res/Description-test-images/Color/woman_darkhair.ppm > im16CSD.txt

cat  *CSD.txt> dat/csd.txt
rm *CSD.txt


cmp=`diff dat/csd.txt res/Description-expected-results/Color/csd.txt`

if [ "1$cmp" == "1" ]
then
    echo "Description-CSD............ ok."
else
    echo "Description-CSD............ failed."
fi
