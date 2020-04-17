FILES=Tests/Shift/*
for test in $FILES
do
  echo "Processing $test file:"
  echo "Input data:"
  cat $test
  echo "Output data:"
  ./LR4_shift < $test
done
