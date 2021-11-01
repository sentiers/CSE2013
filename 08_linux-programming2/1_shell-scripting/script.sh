#! /bin/bash
# This is a sample Born shell script.
echo “1. the process id of the shell is $$”
echo “2. the name of the script is $0”
echo “3. the first argument is $1”
echo “4. the second argument is $2”
echo “5. a list of all the arguments is $*”
echo “6. a list of all the arguments is $@”
echo “7. the script places the date into a temporary file called $1.$$”
date > $1.$$    #redirect the output of date