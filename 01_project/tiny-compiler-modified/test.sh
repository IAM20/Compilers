#! /bin/bash

./bin/tiny ../tests/gcd.cm > gcd-test.log
echo "Progress 1/6"
cat gcd-test.log
sleep .5
./bin/tiny ../tests/fact.cm > fact-test.log
echo "Progress 2/6"
cat fact-test.log
sleep .5
./bin/tiny ../tests/fault.cm > fault-test.log
echo "Progress 3/6"
cat fault-test.log
sleep .5
./bin/tiny ../tests/all-comment.cm > all-comment-test.log
echo "Progress 4/6"
cat all-comment-test.log
sleep .5
./bin/tiny ../tests/hello-cminus.cm > hello-cminus-test.log
cat hello-cminus-test.log
echo "Progress 5/6"
sleep .5
./bin/tiny ../tests/eof.cm > eof-test.log
cat eof-test.log
echo "Progress 6/6"
sleep .5
echo "Check test results *.log"
