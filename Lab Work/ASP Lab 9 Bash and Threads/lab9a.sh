#!/bin/bash

# File Name that will be used to write final output after merging all text files(max 5) content
appendOutputFile="output.txt"
checkFilesNum=0

#When no input file name is supplied.
if [ $# -eq 0 ]; then
  echo "Please enter atleast 1 input file. Now proceeding with exit..."
  exit 1
fi

# Validating so as to allow max 5 valid text file names that will be supplied as args.
(( $# > 5 )) && echo "You are allowed to enter MAX 5 file names as args for supplying I/P." && exit 1

# Checking to make sure if the text file is a valid text file or it's invalid one.
for eachFile in "$@"; do
  case "$eachFile" in
    *.txt)
      if [ ! -f "$eachFile" ]; then
        echo "Issue Encountered: This is invalid textual file - '$eachFile'.Please check."
        exit 1
      fi
      ;;
    *)
      echo "Issue Encountered: This is invalid textual file with the .txt extension - '$eachFile'. Please check."
      exit 1
      ;;
  esac
done

# Combining text from all the valid text files and writing that to the final O/P File.
> "$appendOutputFile" # Clear the output file if it already exists.

checkFilesNum=1
until [ $checkFilesNum -gt $# ]; do
  cat "${!checkFilesNum}" >> "$appendOutputFile"
  ((checkFilesNum++))
done

echo "Text from $# file(s) merged successfully and finally saved into the $appendOutputFile."