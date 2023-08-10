
#!/bin/bash

# File considered for the program to check on for permissions and operations.
FILE_NAME="sample.txt"

# Messages set that will be used during the shell script execution.
FILE_CR_MSG="The file '${FILE_NAME}' created successfully."
FILE_WR_PERM_MSG="Write permission is provided to the ${FILE_NAME} file successfully."
FILE_APPEND_MSG="The O/P of the command 'ls -1' is appended successfully to the file - '${FILE_NAME}'."
FILE_NO_PER_MESSAGE="You are not having the required privileges for the file - '${FILE_NAME}'."

# Checking the existence of the file  "sample.txt" in the Present Working Directory (PWD).
if [ -e ${FILE_NAME} ]; then
  # Checking if the current user is the owner of the file.
  if [ $(stat -c '%U' ${FILE_NAME}) == $(whoami) ]; then
    # Checking if the "sample.txt" file has write permission.
    if [ -w ${FILE_NAME} ]; then
      # Now, the contents of the command "ls -1" will be appended to the "sample.txt" file.
      ls -1 >> ${FILE_NAME}
      echo ${FILE_APPEND_MSG}
    else
      # Granting write permission to the "sample.txt" file using "chmod".
      chmod u+w ${FILE_NAME}
      echo ${FILE_WR_PERM_MSG}
      # Now the contents of the command "ls -1" will be appended to the file after provision of write permission.
      ls -1 >> ${FILE_NAME}
      echo ${FILE_APPEND_MSG}
    fi
  else
    # Displaying proper message if the user is not having the owner privileges of that file.
    echo ${FILE_NO_PER_MESSAGE}
  fi
else
  # In case the file "sample.txt" does not exist, we will be creating that. 
  touch ${FILE_NAME}
  echo ${FILE_CR_MSG}
fi
