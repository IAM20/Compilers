i=1

for file in tests/*.cm
do
  ./bin/cminus_flex ${file} > ${i}.log
  cat ${i}.log
  printf "\n\n"
  
  if [ $i = 11 ]; then
    echo "######################################################"
    echo "###    From now on compiler must report errors!    ###"
    echo "######################################################"  
    echo ""
    echo ""
  fi
  
  ((i++))
done
