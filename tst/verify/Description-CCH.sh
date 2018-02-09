# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# rese: 2017/Oct/13
# Version: 1.0.00
# Content: Testing file for commiting.
# Description: Tests descriptor CCH.
# Future add-on's: many.

if [ "$1" != "nocompile" ]
then
    make Description-CCH $@
fi

./bin/Description-CCH res/Description-test-images/Color/cameraman.ppm > im01CCH.txt
./bin/Description-CCH res/Description-test-images/Color/house.ppm > im02CCH.txt
./bin/Description-CCH res/Description-test-images/Color/jetplane.ppm > im03CCH.txt
./bin/Description-CCH res/Description-test-images/Color/lake.ppm > im04CCH.txt
./bin/Description-CCH res/Description-test-images/Color/lena_color_256.ppm > im05CCH.txt
./bin/Description-CCH res/Description-test-images/Color/lena_color_512.ppm > im06CCH.txt
./bin/Description-CCH res/Description-test-images/Color/lena_gray_256.ppm > im07CCH.txt
./bin/Description-CCH res/Description-test-images/Color/lena_gray_512.ppm > im08CCH.txt
./bin/Description-CCH res/Description-test-images/Color/livingroom.ppm > im09CCH.txt
./bin/Description-CCH res/Description-test-images/Color/mandril_color.ppm > im10CCH.txt
./bin/Description-CCH res/Description-test-images/Color/mandril_gray.ppm > im11CCH.txt
./bin/Description-CCH res/Description-test-images/Color/peppers_gray.ppm > im12CCH.txt
./bin/Description-CCH res/Description-test-images/Color/pirate.ppm > im13CCH.txt
./bin/Description-CCH res/Description-test-images/Color/walkbridge.ppm > im14CCH.txt
./bin/Description-CCH res/Description-test-images/Color/woman_blonde.ppm > im15CCH.txt
./bin/Description-CCH res/Description-test-images/Color/woman_darkhair.ppm > im16CCH.txt

cat  *CCH.txt> dat/cch.txt
rm *CCH.txt


cmp=`diff dat/cch.txt res/Description-expected-results/Color/cch.txt`

if [ "1$cmp" == "1" ]
then
    echo "Description-CCH............ ok."
else
    echo "Description-CCH............ failed."
fi
