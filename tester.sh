#!/bin/bash


test_dir="test_files"
ft_nm_path="./ft_nm"
ft_otool_path="./ft_otool"
dir_list=(
    'test_files'
    # 'corrupted_files'
    '/usr/bin'
)
nm_flags=(
   ''
   # '-a'
   # '-gA'
   # '-n'
   # '-o'
   # '-p'
   # '-r'
   # '-u'
   # '-U'
   # '-m'
   # '-x'
   # '-j'
   # '-l'
   # '-A'
   # '-P'
   # '-P -t d'
   # '-P -t o'
   # '-P -t x'
   # '-s '
   # '-s __DATA __data'
   # '-s __DATA __text'
   # '-s __DATA __bss'
   # '-s __TEXT __text'
   # '-s __data __DATA'
   # '-arch little'
   # '-arch any'
   # '-arch big -arch ppc -arch i386'
   # '-arch x86_64'
   # '-arch all'
   # '-arch ppc'
   # '-arch ppc -arch x86_64 -arch i386'
   # '-pgno -arch all'
   # '-AP -t d -a'
   # '-ag'
   # '-AP'
   # '-oP'
   # '-ns __TEXT __text'
   # '-jl -P -td -arch x86_64'
   # '-pra'
   # '-Ao'
   # '-uU'
   # '-t d -U -ongP'
   # '-PAPA'
   # '-agnoprjlAP -t x -arch all'
   # '-ls __DATA __data'
   # '-ls __TEXT __text'
   # '-ls __DATA __bss'
   # '-ls __DATA __text'
   # '-Prp'
   # '-fAs __TEXT __text'
);

otool_flags=(
   ''
   # '-f'
   # '-a'
   # '-h'
   # '-l'
   '-t'
   # '-d'
   # '-s '
   # '-s __DATA __data'
   # '-s __DATA __text'
   # '-s __DATA __bss'
   # '-s __TEXT __text'
   # '-s __data __DATA'
   # '-arch little'
   # '-arch any'
   # '-arch big -arch ppc -arch i386'
   # '-arch x86_64'
   # '-arch all'
   # '-arch ppc'
   # '-arch ppc -arch x86_64 -arch i386'
);

R="\e[0;31m"
G="\e[0;32m"
Y="\e[0;33m"
N="\e[0m"
B="\e[1m"
score=0;
# bin="./ft_otool"
# ref_bin="otool -t"
# bin=""
# total=$(ls -1 ${test_dir} | wc -l);
# total=$(echo $total "*" ${#nm_flags[@]} | bc);
diff_opt=0;
stop_opt=0;

ref_bin=(
  'nm'
  'otool'
)

for arg in $*
do
  if [ $arg == "nm" ]
  then
    if [ ${#ref_bin[@]} -eq 2 ]
    then
      ref_bin=('nm')
      # bin=('./ft_nm')
    else
      ref_bin=("${ref_bin[@]}" 'nm')
      # bin=("${bin}" 'nm')
    fi
  fi
  if [ $arg == "otool" ]
  then
    if [ ${#ref_bin[@]} -eq 2 ]
    then
      ref_bin=('otool')
      # bin=('./ft_otool')
    else
      ref_bin=("${ref_bin[@]}" 'otool')
      # bin=("${bin}" 'otool')
    fi
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

for rbin in "${ref_bin[@]}"
do
  if [ $rbin == "nm" ]
  then
    flaglst=("${nm_flags[@]}");
    bin=$ft_nm_path;
  else
    flaglst=("${otool_flags[@]}");
    bin=$ft_otool_path;
  fi
  for dir in "${dir_list[@]}"
  do
    for flag in "${flaglst[@]}"
    do
      printf "|$flag bin = $bin|\n"
      filelist=$(ls -1 ${dir})
      for x in $filelist
      do
        total=$(($total + 1))
        filepath="${dir}/${x}"
        if [ -n "$flag" ]
        then
          a=$(diff <($rbin $flag $filepath 2>/dev/null) <($bin $flag $filepath 2>/dev/null))
        else
          a=$(diff <($rbin $filepath 2>/dev/null) <($bin $filepath 2>/dev/null))
        fi
        if [ ${#a} -eq 0 ]
        then
          printf "${G}${B}[ OK ][ $rbin $flag $filepath ]${N}\n"
          score=$(($score + 1))
        else
        {
          printf "\n"
          if [ -n "$flag" ]
          then
            $rbin $flag $filepath 2>/dev/null 1>/dev/null
            rbin_ret=$?
            $bin $flag $filepath 2>/dev/null 1>/dev/null
            # a=$(diff <(s$rbin $flag $filepath 2>/dev/null) <($bin $flag $filepath 2>/dev/null))
          else
            $rbin $filepath 2>/dev/null 1>/dev/null
            rbin_ret=$?
            $bin $filepath 2>/dev/null 1>/dev/null
            # a=$(diff <($rbin $filepath 2>/dev/null) <($bin $filepath 2>/dev/null))
          fi

          if [ $rbin_ret -eq 139 ] && [ $? -ne 139 ]; then
            score=$(($score + 1))
            printf "${Y}${B}[ $rbin SEGV ][ $rbin $flag $filepath ]${N}\n"
          elif [ $? -eq 139 ]; then
            printf "${R}${B}[ SEGV ][ $rbin $flag $filepath ]${N}\n"
          else
            printf "${R}${B}[ KO ][ $rbin $flag $filepath ]${N}\n"
          fi
          if [ $diff_opt -eq 1 ]
          then
            if [ -n "$flag" ]
            then
              diff <($rbin $flag $filepath 2>/dev/null) <($bin $flag $filepath 2>/dev/null)
            else
              diff <($rbin $filepath 2>/dev/null) <($bin $filepath 2>/dev/null)
            fi
          fi
          if [ $stop_opt -eq 1 ]
          then
            exit 0
          fi
        }
        fi
      done
    done
  done
done

if [ $score -eq $total ]
then
  printf "\n\t${G}${B}Final: $score / $((${total} + 0))${N}\n\n"
else
  printf "\n\t${R}${B}Final: $score / $((${total} + 0))${N}\n\n"
fi
