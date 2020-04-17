FILES=Tests/KMP/*
for test in $FILES
do
  echo "Processing $test file:"
  echo "Input data:"
  cat $test
  echo "Output data:"
  ./LR4_KMP < $test
done
