#!/bin/bash

# This script counts the number of files in a directory(for both relative/absolute paths).
# and it can also count the number of files based on the specific extension.

# Function to display the usage of the command for this program.
command_usage() {
  echo "Correct Command Usage - $0 <directory_path> [<file_extension>]"
  exit 1
}

# Check if the correct number of arguments have been passed
if [ $# -lt 1 ]; then
  command_usage
fi

# Assigning the passed arguments to variables.
directory_path=$1
file_extension=$2

# It will return the number of files with the provided extension in the dir path.
if [ -z "$file_extension" ]; then
  num_of_files=$(find "$directory_path" -type f | wc -l)
else
  num_of_files=$(find "$directory_path" -type f -name "*$file_extension" | wc -l)
fi

# Function to display the count of files
display_count_of_files() {
  if [ $num_of_files -eq 0 ]; then
    if [ -z "$file_extension" ]; then
      echo "There were no files in the I/P directory path - $directory_path"
    else
      echo "There were no files found with extension $file_extension in the directory path - $directory_path"
    fi
  else
    if [ -z "$file_extension" ]; then
      echo "File Count in the directory path - $directory_path: $num_of_files"
    else
      echo "File Count having extension $file_extension in the directory path - $directory_path: $num_of_files"
    fi
  fi
}

# Call the function to display the count of files
display_count_of_files

#ILLUSTRATIONS USED:- 
#./lab8b.sh /home/lnu8/Documents/lab8
#./lab8b.sh /home/lnu8/Documents/lab8 sh
#./lab8b.sh ~/Documents 
#./lab8b.sh ~/Documents sh