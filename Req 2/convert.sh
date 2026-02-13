#!/bin/bash

#============================ REQ 2A ============================

function convert
{
    # DEFAULT: HEX TO DEC
    if [ "$#" -eq 1 ]; then
        # $(( base#number )): Arithmetic expansion in Bash tells it to interpret the value as base-16 (hexadecimal) then to convert it internally to decimal.
        echo $((16#$1))

    # HEX TO DEC
    elif [ "$1" -eq 1 ]; then
        echo $((16#$2))

    # DEC TO HEX
    elif [ "$1" -eq 2 ]; then
        # %X: convert integer to hexadecimal (uppercase letters A–F)
        printf "%X\n" "$2"
    fi
}
