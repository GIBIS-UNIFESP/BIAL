# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# rese: 2017/Oct/13
# Version: 1.0.00
# Content: Testing file for commiting.
# Description: Tests descriptor SCD.
# Future add-on's: many.

if [ "$1" != "nocompile" ]
then
    make Description-SCD $@
fi

./bin/Description-SCD res/Description-test-images/Color/cameraman.ppm > im01SCD.txt
./bin/Description-SCD res/Description-test-images/Color/house.ppm > im02SCD.txt
./bin/Description-SCD res/Description-test-images/Color/jetplane.ppm > im03SCD.txt
./bin/Description-SCD res/Description-test-images/Color/lake.ppm > im04SCD.txt
./bin/Description-SCD res/Description-test-images/Color/lena_color_256.ppm > im05SCD.txt
./bin/Description-SCD res/Description-test-images/Color/lena_color_512.ppm > im06SCD.txt
./bin/Description-SCD res/Description-test-images/Color/lena_gray_256.ppm > im07SCD.txt
./bin/Description-SCD res/Description-test-images/Color/lena_gray_512.ppm > im08SCD.txt
./bin/Description-SCD res/Description-test-images/Color/livingroom.ppm > im09SCD.txt
./bin/Description-SCD res/Description-test-images/Color/mandril_color.ppm > im10SCD.txt
./bin/Description-SCD res/Description-test-images/Color/mandril_gray.ppm > im11SCD.txt
./bin/Description-SCD res/Description-test-images/Color/peppers_gray.ppm > im12SCD.txt
./bin/Description-SCD res/Description-test-images/Color/pirate.ppm > im13SCD.txt
./bin/Description-SCD res/Description-test-images/Color/walkbridge.ppm > im14SCD.txt
./bin/Description-SCD res/Description-test-images/Color/woman_blonde.ppm > im15SCD.txt
./bin/Description-SCD res/Description-test-images/Color/woman_darkhair.ppm > im16SCD.txt

cat  *SCD.txt> dat/scd.txt
rm *SCD.txt


cmp=`diff dat/scd.txt res/Description-expected-results/Color/scd.txt`

if [ "1$cmp" == "1" ]
then
    echo "Description-SCD............ ok."
else
    echo "Description-SCD............ failed."
fi
