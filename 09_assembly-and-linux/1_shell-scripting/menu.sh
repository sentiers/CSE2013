#!/bin/sh
echo 'menu test program'
stop=0                  # reset loop termination flag
while test $stop -eq 0          # loop until done
        do
                echo
                echo -n 'you choice? '  # prompt
                read reply              # read response
                echo
                case $reply in          # process response
                        1)
                                echo 'Current Date:'
                                date            # display date
                                ;;
                        2|3)
                                echo 'Current Working Directory:'
                                pwd             # display working directory
                                ;;
                        4)
                                echo 'Stop Program'
                                stop=1          # set loop termination flag
                                ;;
                        5)
                                echo 'Multiplication Table:'
                                for i in 2 3 4 5 6 7 8 9
                                do
                                        for j in 1 2 3 4 5 6 7 8 9
                                        do
                                                echo -n " $i*$j=`expr $i \* $j`"
                                        done
                                        echo
                                done
                                ;;
                        *)                      # default
                                echo illegal choice             # error
                                ;;
                esac
        done
