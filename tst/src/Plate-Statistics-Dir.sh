EXPECTED_ARGS=2
E_BADARGS=65

if [ $# -lt $EXPECTED_ARGS ];
then
    echo "Usage: $0 <gt_dir> <tgt_subdir>"
    exit $E_BADARGS
fi

cd $1;
file=`ls *.pgm.gz`;
cd -;

for s in ${file}; do
    a=`basename $s ".pgm.gz"`;
    echo "Plate-Statistics ${2}/${a}.ppm.gz.txt ${1}/${s} > ${2}/${a}-stats.txt"
    Plate-Statistics ${2}/${a}.ppm.gz.txt ${1}/${s} > ${2}/${a}-stats.txt
done

