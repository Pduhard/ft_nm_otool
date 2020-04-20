#!/bin/bash

test_dir="test_files"
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
   '-arch little'
   '-arch any'
   '-arch big -arch ppc -arch i386'
   '-arch x86_64'
   '-arch all'
   '-arch ppc'
   '-arch ppc -arch x86_64 -arch i386'
   '-pgno -arch all'
   '-AP -t d -a'
   '-ag'
   '-AP'
   '-oP'
   '-ns __TEXT __text'
   '-jl -P -td -arch x86_64'
   '-pra'
   '-Ao'
   '-uU'
   '-t d -U -ongP'
   '-PAPA'
   '-agnoprjlAP -t x -arch all'
   '-ls __DATA __data'
   '-ls __TEXT __text'
   '-ls __DATA __bss'
   '-ls __DATA __text'
   '-Prp'
   '-fAs __TEXT __text'
);

R="\e[0;31m"
G="\e[0;32m"
Y="\e[0;33m"
N="\e[0m"
B="\e[1m"
score=0;
bin="./ft_otool"
ref_bin="otool -t"
# bin=""
total=$(ls -1 ${test_dir} | wc -l);
total=$(echo $total "*" ${#flags[@]} | bc);
diff_opt=0;
stop_opt=0;

for arg in $*
do
  if [ $arg == "nm" ]
  then
    bin="./ft_nm"
    ref_bin="nm"
  fi
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
      a=$(diff <($ref_bin $flag $filepath 2>/dev/null) <($bin $flag $filepath 2>/dev/null))
    else
      a=$(diff <($ref_bin $filepath 2>/dev/null) <($bin $filepath 2>/dev/null))
    fi
    if [ ${#a} -eq 0 ]
    then
      printf "${G}${B}[ OK ][ $ref_bin $flag $filepath ]${N}\n"
      score=$(($score + 1))
    else
    {
      printf "\n"
      $ref_bin $filepath 2>/dev/null 1>/dev/null
      ref_bin_ret=$?
      $bin $filepath 2>/dev/null 1>/dev/null
      if [ $ref_bin_ret -eq 139 ] && [ $? -ne 139 ]; then
        score=$(($score + 1))
        printf "${Y}${B}[ $ref_bin SEGV ][ $ref_bin $flag $filepath ]${N}\n"
      elif [ $? -eq 139 ]; then
        printf "${R}${B}[ SEGV ][ $ref_bin $flag $filepath ]${N}\n"
      else
        printf "${R}${B}[ KO ][ $ref_bin $flag $filepath ]${N}\n"
      fi
      if [ $diff_opt -eq 1 ]
      then
        if [ -n "$flag" ]
        then
          diff <($ref_bin $flag $filepath 2>/dev/null) <($bin $flag $filepath 2>/dev/null)
        else
          diff <($ref_bin $filepath 2>/dev/null) <($bin $filepath 2>/dev/null)
        fi
      fi
      if [ $stop_opt -eq 1 ]
      then
        exit 0
      fi
    }
    fi
    # $ref_bin $filepath 2>/dev/null 1>/dev/null
    # if [ $? -eq 139 ]
    # then
    #   printf "${R}${B}[$ref_bin $filepath : SEGV] ${N}"
    # else
    #   printf "${G}${B}[$ref_bin $filepath : OK] ${N}"
    # fi
    # $bin $filepath 2>/dev/null 1>/dev/null
    # if [ $? -ne 0 ]
    # then
    #   printf "${R}${B}[ft_$ref_bin $filepathm : SEGV] ${N}\n"
    # else
    #   printf "${G}${B}[ft_$ref_bin $filepathm : OK] ${N}\n"
    # fi
  done
done

if [ -n "$flag" ]
then
  a=$(diff <($ref_bin $flag $filelist 2>/dev/null) <($bin $flag $filelist 2>/dev/null))
else
  a=$(diff <($ref_bin $filelist 2>/dev/null) <($bin $filelist 2>/dev/null))
fi


total=$(($total + 1))
if [ $ref_bin_ret -eq 139 ] && [ $? -ne 139 ]; then
  score=$(($score + 1))
  printf "${Y}${B}[ $ref_bin SEGV ][ $ref_bin $flag $test_dir/* ]${N}\n"
elif [ $? -eq 139 ]; then
  printf "${R}${B}[ SEGV ][ $ref_bin $flag $test_dir/* ]${N}\n"
else
  printf "${R}${B}[ KO ][ $ref_bin $flag $test_dir/* ]${N}\n"
fi

if [ $score -eq $total ]
then
  printf "\n\t${G}${B}Final: $score / $((${total} + 0))${N}\n\n"
else
  printf "\n\t${R}${B}Final: $score / $((${total} + 0))${N}\n\n"
fi
