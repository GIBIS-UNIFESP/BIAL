# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# rese: 2017/Oct/13
# Version: 1.0.00
# Content: Testing file for commiting.
# Description: Tests descriptor ACC.
# Future add-on's: many.

if [ "$1" != "nocompile" ]
then
    make Description-ACC $@
fi

./bin/Description-ACC res/Description-test-images/Color/cameraman.ppm > im01ACC.txt
./bin/Description-ACC res/Description-test-images/Color/house.ppm > im02ACC.txt
./bin/Description-ACC res/Description-test-images/Color/jetplane.ppm > im03ACC.txt
./bin/Description-ACC res/Description-test-images/Color/lake.ppm > im04ACC.txt
./bin/Description-ACC res/Description-test-images/Color/lena_color_256.ppm > im05ACC.txt
./bin/Description-ACC res/Description-test-images/Color/lena_color_512.ppm > im06ACC.txt
./bin/Description-ACC res/Description-test-images/Color/lena_gray_256.ppm > im07ACC.txt
./bin/Description-ACC res/Description-test-images/Color/lena_gray_512.ppm > im08ACC.txt
./bin/Description-ACC res/Description-test-images/Color/livingroom.ppm > im09ACC.txt
./bin/Description-ACC res/Description-test-images/Color/mandril_color.ppm > im10ACC.txt
./bin/Description-ACC res/Description-test-images/Color/mandril_gray.ppm > im11ACC.txt
./bin/Description-ACC res/Description-test-images/Color/peppers_gray.ppm > im12ACC.txt
./bin/Description-ACC res/Description-test-images/Color/pirate.ppm > im13ACC.txt
./bin/Description-ACC res/Description-test-images/Color/walkbridge.ppm > im14ACC.txt
./bin/Description-ACC res/Description-test-images/Color/woman_blonde.ppm > im15ACC.txt
./bin/Description-ACC res/Description-test-images/Color/woman_darkhair.ppm > im16ACC.txt

cat  *ACC.txt> dat/acc.txt
rm *ACC.txt


cmp=`diff dat/acc.txt res/Description-expected-results/Color/acc.txt`

if [ "1$cmp" == "1" ]
then
    echo "Description-ACC............ ok."
else
    echo "Description-ACC............ failed."
fi
