# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# rese: 2017/Oct/13
# Version: 1.0.00
# Content: Testing file for commiting.
# Description: Tests descriptor CCV.
# Future add-on's: many.

if [ "$1" != "nocompile" ]
then
    make Description-CCV $@
fi

./bin/Description-CCV res/Description-test-images/Color/cameraman.ppm > im01CCV.txt
./bin/Description-CCV res/Description-test-images/Color/house.ppm > im02CCV.txt
./bin/Description-CCV res/Description-test-images/Color/jetplane.ppm > im03CCV.txt
./bin/Description-CCV res/Description-test-images/Color/lake.ppm > im04CCV.txt
./bin/Description-CCV res/Description-test-images/Color/lena_color_256.ppm > im05CCV.txt
./bin/Description-CCV res/Description-test-images/Color/lena_color_512.ppm > im06CCV.txt
./bin/Description-CCV res/Description-test-images/Color/lena_gray_256.ppm > im07CCV.txt
./bin/Description-CCV res/Description-test-images/Color/lena_gray_512.ppm > im08CCV.txt
./bin/Description-CCV res/Description-test-images/Color/livingroom.ppm > im09CCV.txt
./bin/Description-CCV res/Description-test-images/Color/mandril_color.ppm > im10CCV.txt
./bin/Description-CCV res/Description-test-images/Color/mandril_gray.ppm > im11CCV.txt
./bin/Description-CCV res/Description-test-images/Color/peppers_gray.ppm > im12CCV.txt
./bin/Description-CCV res/Description-test-images/Color/pirate.ppm > im13CCV.txt
./bin/Description-CCV res/Description-test-images/Color/walkbridge.ppm > im14CCV.txt
./bin/Description-CCV res/Description-test-images/Color/woman_blonde.ppm > im15CCV.txt
./bin/Description-CCV res/Description-test-images/Color/woman_darkhair.ppm > im16CCV.txt

cat  *CCV.txt> dat/ccv.txt
rm *CCV.txt


cmp=`diff dat/ccv.txt res/Description-expected-results/Color/ccv.txt`

if [ "1$cmp" == "1" ]
then
    echo "Description-CCV............ ok."
else
    echo "Description-CCV............ failed."
fi
