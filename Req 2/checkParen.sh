#!/bin/bash
function checkParen
{
    input_str="$1"
    counter=0

    for (( i=0; i<${#input_str}; i++ ))
    do
        char="${input_str:$i:1}"

        if [ "$char" = "(" ]; then
            ((counter++))
        else
            ((counter--))
        fi

        if [ $counter -lt 0 ]; then
            echo 0
            return
        fi
    done

    if [ $counter -eq 0 ]; then
        echo 1
    else
        echo 0
    fi
}
