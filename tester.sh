#!/bin/bash

test_dir="test_file"
filelist=$(ls -1 ${test_dir})
flags=(
   ''
   '-a'
   '-g'
   '-n'
   '-o'
   '-p'
   '-r'
   '-u'
   '-U'
   # '-m'
   # '-x'
   '-j'
   '-l'
   '-A'
   '-P'
   '-P -t d'
   '-P -t o'
   '-P -t x'
   '-s '
   '-s __DATA __data'
   '-s __DATA __text'
   '-s __DATA __bss'
   '-s __TEXT __text'
   '-s __data __DATA'
   # '-arch all'
);
R="\e[0;31m"
G="\e[0;32m"
Y="\e[0;33m"
N="\e[0m"
B="\e[1m"
score=0;
total=$(ls -1 ${test_dir} | wc -l);
total=$(echo $total "*" ${#flags[@]} | bc);
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

for flag in "${flags[@]}"
do
  printf "|$flag|\n"
  for x in $filelist
  do
    filepath="${test_dir}/${x}"
    if [ -n "$flag" ]
    then
      a=$(diff <(nm $flag $filepath 2>/dev/null) <(./ft_nm $flag $filepath 2>/dev/null))
    else
      a=$(diff <(nm $filepath 2>/dev/null) <(./ft_nm $filepath 2>/dev/null))
    fi
    if [ ${#a} -eq 0 ]
    then
      printf "${G}${B}[ OK ][ nm $flag $filepath ]${N}\n"
      score=$(($score + 1))
    else
    {
      printf "\n"
      nm $filepath 2>/dev/null 1>/dev/null
      nm_ret=$?
      ./ft_nm $filepath 2>/dev/null 1>/dev/null
      if [ $nm_ret -eq 139 ] && [ $? -ne 139 ]; then
        score=$(($score + 1))
        printf "${Y}${B}[ nm SEGV ][ nm $flag $filepath ]${N}\n"
      elif [ $? -eq 139 ]; then
        printf "${R}${B}[ SEGV ][ nm $flag $filepath ]${N}\n"
      else
        printf "${R}${B}[ KO ][ nm $flag $filepath ]${N}\n"
      fi
      if [ $diff_opt -eq 1 ]
      then
        if [ -n "$flag" ]
        then
          diff <(nm $flag $filepath 2>/dev/null) <(./ft_nm $flag $filepath 2>/dev/null)
        else
          diff <(nm $filepath 2>/dev/null) <(./ft_nm $filepath 2>/dev/null)
        fi
      fi
      if [ $stop_opt -eq 1 ]
      then
        exit 0
      fi
    }
    fi
    # nm $filepath 2>/dev/null 1>/dev/null
    # if [ $? -eq 139 ]
    # then
    #   printf "${R}${B}[nm $filepath : SEGV] ${N}"
    # else
    #   printf "${G}${B}[nm $filepath : OK] ${N}"
    # fi
    # ./ft_nm $filepath 2>/dev/null 1>/dev/null
    # if [ $? -ne 0 ]
    # then
    #   printf "${R}${B}[ft_nm $filepathm : SEGV] ${N}\n"
    # else
    #   printf "${G}${B}[ft_nm $filepathm : OK] ${N}\n"
    # fi
  done
done

if [ $score -eq $total ]
then
  printf "\n\t${G}${B}Final: $score / $((${total} + 0))${N}\n\n"
else
  printf "\n\t${R}${B}Final: $score / $((${total} + 0))${N}\n\n"
fi
