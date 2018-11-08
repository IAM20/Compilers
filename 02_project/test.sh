#! /bin/bash

./bin/cminus_flex ./tests/gcd.cm > gcd-test.log
printf "Progress 1/8\n"
cat gcd-test.log
sleep .5
printf "\n\n"
./bin/cminus_flex ./tests/fact.cm > fact-test.log
echo "Progress 2/8"
cat fact-test.log
sleep .5
printf "\n\n"
./bin/cminus_flex ./tests/fault.cm > fault-test.log
echo "Progress 3/8"
cat fault-test.log
sleep .5
printf "\n\n"

./bin/cminus_flex ./tests/all-comment.cm > all-comment-test.log
echo "Progress 4/8"
cat all-comment-test.log
sleep .5
printf "\n\n"

./bin/cminus_flex ./tests/hello-cminus.cm > hello-cminus-test.log
cat hello-cminus-test.log
echo "Progress 5/8"
sleep .5
printf "\n\n"
./bin/cminus_flex ./tests/eof.cm > eof-test.log
cat eof-test.log
echo "Progress 6/8"
sleep .5
printf "\n\n"
./bin/cminus_flex ./tests/array.cm > array-test.log
cat array-test.log
echo "Progress 7/8"
printf "\n\n"
sleep .5
./bin/cminus_flex ./tests/2_fault.cm > 2_fault-test.log
cat 2_fault-test.log
echo "Progress 8/8"
echo "Check log files"
