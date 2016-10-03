EXPECTED_ARGS=6
E_BADARGS=65

if [ $# -lt $EXPECTED_ARGS ];
then
    echo "Usage: $0 <src_dir> <tgt_subdir> <canny_threshold> <detection_threshold> <canny_levels> <clean_edges>"
    exit $E_BADARGS
fi

if [ ! -d ${2}/HoughTransform_${3}_${4}_${5}_${6} ]; then
    mkdir -p ${2}/HoughTransform_${3}_${4}_${5}_${6}
    if [ $? -ne 0 ];
    then
        echo "Error creating output directory."
    else
        cd $1;
        file=`ls *.ppm*`;
        cd -;
        
        for s in ${file}; do
            Plate-Finder ${1}/${s} ${2}/HoughTransform_${3}_${4}_${5}_${6}/${s} ${3} ${4} ${5} ${6} >> ${2}/HoughTransform_${3}_${4}_${5}_${6}/${s}.txt
        done
    fi
fi
