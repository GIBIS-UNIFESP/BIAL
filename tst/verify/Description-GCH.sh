# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# rese: 2017/Oct/13
# Version: 1.0.00
# Content: Testing file for commiting.
# Description: Tests descriptor GCH.
# Future add-on's: many.

if [ "$1" != "nocompile" ]
then
    make Description-GCH $@
fi

./bin/Description-GCH res/Description-test-images/Color/cameraman.ppm > im01GCH.txt
./bin/Description-GCH res/Description-test-images/Color/house.ppm > im02GCH.txt
./bin/Description-GCH res/Description-test-images/Color/jetplane.ppm > im03GCH.txt
./bin/Description-GCH res/Description-test-images/Color/lake.ppm > im04GCH.txt
./bin/Description-GCH res/Description-test-images/Color/lena_color_256.ppm > im05GCH.txt
./bin/Description-GCH res/Description-test-images/Color/lena_color_512.ppm > im06GCH.txt
./bin/Description-GCH res/Description-test-images/Color/lena_gray_256.ppm > im07GCH.txt
./bin/Description-GCH res/Description-test-images/Color/lena_gray_512.ppm > im08GCH.txt
./bin/Description-GCH res/Description-test-images/Color/livingroom.ppm > im09GCH.txt
./bin/Description-GCH res/Description-test-images/Color/mandril_color.ppm > im10GCH.txt
./bin/Description-GCH res/Description-test-images/Color/mandril_gray.ppm > im11GCH.txt
./bin/Description-GCH res/Description-test-images/Color/peppers_gray.ppm > im12GCH.txt
./bin/Description-GCH res/Description-test-images/Color/pirate.ppm > im13GCH.txt
./bin/Description-GCH res/Description-test-images/Color/walkbridge.ppm > im14GCH.txt
./bin/Description-GCH res/Description-test-images/Color/woman_blonde.ppm > im15GCH.txt
./bin/Description-GCH res/Description-test-images/Color/woman_darkhair.ppm > im16GCH.txt

cat  *GCH.txt> dat/gch.txt
rm *GCH.txt


cmp=`diff dat/gch.txt res/Description-expected-results/Color/gch.txt`

if [ "1$cmp" == "1" ]
then
    echo "Description-GCH............ ok."
else
    echo "Description-GCH............ failed."
fi
