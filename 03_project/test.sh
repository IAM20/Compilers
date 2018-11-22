#! /bin/bash

./bin/cminus_flex ./tests/01_array.cm > 01.log
cat 01.log
printf "\n\n"
./bin/cminus_flex ./tests/02_fact.cm > 02.log
cat 02.log
printf "\n\n"
./bin/cminus_flex ./tests/03_iter.cm > 03.log
cat 03.log
printf "\n\n"

./bin/cminus_flex ./tests/04_gcd.cm > 04.log
cat 04.log
printf "\n\n"

./bin/cminus_flex ./tests/05_hello-cminus.cm > 05.log
cat 05.log
printf "\n\n"

./bin/cminus_flex ./tests/06_dangling-else.cm > 06.log
cat 06.log
printf "\n\n"

./bin/cminus_flex ./tests/07_dangling-operator.cm > 07.log
cat 07.log
printf "\n\n"

./bin/cminus_flex ./tests/08_complicated.cm > 08.log
cat 08.log
printf "\n\n"

./bin/cminus_flex ./tests/09_global-var.cm > 09.log
cat 09.log
printf "\n\n"

./bin/cminus_flex ./tests/10_array-args.cm > 10.log
cat 10.log
printf "\n\n"

./bin/cminus_flex ./tests/11_empty-func.cm > 11.log
cat 11.log
printf "\n\n"

echo "######################################################
###    From now on compiler must report errors!    ###
###  Except 18_semantic-error.cm (No syntax error) ###
######################################################
"


./bin/cminus_flex ./tests/12_file-maybe-not-exist.cm > 12.log
cat 12.log
printf "\n\n"

./bin/cminus_flex ./tests/13_lot-semi.cm > 13.log
cat 13.log
printf "\n\n"

./bin/cminus_flex ./tests/14_global-expr.cm > 14.log
cat 14.log
printf "\n\n"

./bin/cminus_flex ./tests/15_wrong-expr.cm > 15.log
cat 15.log
printf "\n\n"

./bin/cminus_flex ./tests/16_emptycode.cm > 16.log
cat 16.log
printf "\n\n"

./bin/cminus_flex ./tests/17_eof-in-comment.cm > 17.log
cat 17.log
printf "\n\n"

./bin/cminus_flex ./tests/18_semantic-error.cm > 18.log
cat 18.log
printf "\n\n"

./bin/cminus_flex ./tests/19_real-empty.cm > 19.log
cat 19.log
printf "\n\n"

./bin/cminus_flex ./tests/20_cplusplus.cm > 20.log
cat 20.log
printf "\n\n"
