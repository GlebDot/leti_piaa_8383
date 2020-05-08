FILES=Tests/Jokers/*
for test in $FILES
do
  echo "Processing $test file:"
  echo "Input data:"
  cat $test
  echo "Output data:"
  ./main_j < $test
done