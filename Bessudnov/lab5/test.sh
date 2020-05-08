FILES=Tests/Aho-Korasik/*
for test in $FILES
do
  echo "Processing $test file:"
  echo "Input data:"
  cat $test
  echo "Output data:"
  ./main < $test
done