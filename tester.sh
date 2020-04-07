#!/bin/bash

test_dir="/usr/sbin"
filelist=$(ls -1 ${test_dir})
R="\e[0;31m"
G="\e[0;32m"
N="\e[0m"
B="\e[1m"
score=0;
total=$(ls -1 ${test_dir} | wc -l);
diff_opt=0;
stop_opt=0;
for arg in $*
do
  if [ $arg == "-d" ] || [ $arg == "-diff" ] || [ $arg == "-ds" ] || [ $arg == "-sd" ]
  then
    diff_opt=1
  fi
  if [ $arg == "-s" ] || [ $arg == "-stop" ] || [ $arg == "-ds" ] || [ $arg == "-sd" ]
  then
    stop_opt=1
  fi
done

for x in $filelist;
do
  filepath="${test_dir}/${x}"
  a=$(diff <(nm $filepath 2>/dev/null) <(./ft_nm $filepath 2>/dev/null))
  if [ ${#a} -eq 0 ]
  then
    printf "${G}${B}[ OK ][ $filepath ]${N}\n"
    score=$(($score + 1))
  else
  {
    printf "${R}${B}[ KO ][ $filepath ]${N}\n"
    if [ $diff_opt -eq 1 ]
    then
      diff <(nm $filepath ) <(./ft_nm $filepath)
    fi
    if [ $stop_opt -eq 1 ]
    then
      exit 0
    fi
  }
  fi
done;

if [ $score -eq $total ]
then
  printf "\n\t${G}${B}Final: $score / $((${total} + 0))${N}\n\n"
else
  printf "\n\t${R}${B}Final: $score / $((${total} + 0))${N}\n\n"
fi
