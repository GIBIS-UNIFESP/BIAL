# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# rese: 2017/Oct/13
# Version: 1.0.00
# Content: Testing file for commiting.
# Description: Tests descriptor BIC.
# Future add-on's: many.

if [ "$1" != "nocompile" ]
then
    make Description-BIC $@
fi

./bin/Description-BIC res/Description-test-images/Color/cameraman.ppm > im01BIC.txt
./bin/Description-BIC res/Description-test-images/Color/house.ppm > im02BIC.txt
./bin/Description-BIC res/Description-test-images/Color/jetplane.ppm > im03BIC.txt
./bin/Description-BIC res/Description-test-images/Color/lake.ppm > im04BIC.txt
./bin/Description-BIC res/Description-test-images/Color/lena_color_256.ppm > im05BIC.txt
./bin/Description-BIC res/Description-test-images/Color/lena_color_512.ppm > im06BIC.txt
./bin/Description-BIC res/Description-test-images/Color/lena_gray_256.ppm > im07BIC.txt
./bin/Description-BIC res/Description-test-images/Color/lena_gray_512.ppm > im08BIC.txt
./bin/Description-BIC res/Description-test-images/Color/livingroom.ppm > im09BIC.txt
./bin/Description-BIC res/Description-test-images/Color/mandril_color.ppm > im10BIC.txt
./bin/Description-BIC res/Description-test-images/Color/mandril_gray.ppm > im11BIC.txt
./bin/Description-BIC res/Description-test-images/Color/peppers_gray.ppm > im12BIC.txt
./bin/Description-BIC res/Description-test-images/Color/pirate.ppm > im13BIC.txt
./bin/Description-BIC res/Description-test-images/Color/walkbridge.ppm > im14BIC.txt
./bin/Description-BIC res/Description-test-images/Color/woman_blonde.ppm > im15BIC.txt
./bin/Description-BIC res/Description-test-images/Color/woman_darkhair.ppm > im16BIC.txt

cat  *BIC.txt> dat/bic.txt
rm *BIC.txt


cmp=`diff dat/bic.txt res/Description-expected-results/Color/bic.txt`

if [ "1$cmp" == "1" ]
then
    echo "Description-BIC............ ok."
else
    echo "Description-BIC............ failed."
fi
