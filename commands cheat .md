# Linux & Bash Lab 1 --- Command Cheat Sheet

## Linux Basics

-   **Single root filesystem**: `/` is the top-level directory; no drive
    letters.
-   **Terminal-centric workflow** even when GUI exists.

------------------------------------------------------------------------

## File Permissions (`ls -l`)

Example:

    drwxrwxrwx 2 muhammad group1 4096 Feb 15 18:19 dir
    -rwxrwxrwx 1 muhammad group1   18 Feb 15 18:27 test.txt

  Field          Meaning
  -------------- -------------------
  d / -          Directory or file
  rwx (owner)    Owner permissions
  rwx (group)    Group permissions
  rwx (others)   Everyone else
  2              Hard links
  muhammad       Owner
  group1         Group
  4096           Size (bytes)
  Feb 15 18:19   Timestamp
  dir            Name

### Permission Codes

-   r=4, w=2, x=1\
-   `7 = rwx`, `5 = r-x`, `4 = r--`

### chmod

    chmod u+rx test.c
    chmod 544 test.c
    chmod u-wx test.c

------------------------------------------------------------------------

## Core Linux Commands

  Command      Description
  ------------ -------------------------
  `man cmd`    Manual page
  `sudo cmd`   Run as root
  `pwd`        Print working directory
  `cd path`    Change directory
  `touch f`    Create file
  `mkdir d`    Create directory
  `rmdir d`    Remove empty dir
  `rm f`       Delete file
  `rm -r d`    Recursive delete
  `cp a b`     Copy
  `mv a b`     Move/rename

------------------------------------------------------------------------

## Viewing & Editing Streams

### echo / Redirection

    echo "1 2 5" > file
    echo "4 5 6" >> file

### cat / head / tail

    cat file
    head -n 2 file
    tail -n 2 file

### Combine Files

    cat f1 f2
    paste f1 f2

------------------------------------------------------------------------

## Text Processing

### seq

    seq 1 2 5

### sort / uniq

    sort file
    uniq file     # requires sorted input

### tr

    cat file | tr s z

------------------------------------------------------------------------

## grep & Regex

    grep -ni y file

Flags: - `-i` ignore case - `-n` line numbers

### Regex Atoms

  Symbol      Meaning
  ----------- ----------
  \^          start
  \$          end
  .           any char
  \*          zero+
  \[ae\]      set
  \[\^0-9\]   negated
  {x,y}       repeats
              

Examples:

    grep '^T[^\s]*s' file
    grep '0{2}|3{3,}' file

------------------------------------------------------------------------

# Bash Scripting

## Script Skeleton

    #!/bin/bash
    echo Hello World

Run:

    chmod +x file.sh
    ./file.sh

------------------------------------------------------------------------

## Variables (NO spaces)

    x=5
    str="Hello"

------------------------------------------------------------------------

## Arithmetic

    let y=y+1
    y=$((y+1))
    y=`expr $y + 1`
    y=$((2**3))

------------------------------------------------------------------------

## String Length & Substrings

    ${#s}
    ${s:0:3}
    ${s: -4}

------------------------------------------------------------------------

## Arrays

    arr=(a b c)
    echo ${arr[*]}

------------------------------------------------------------------------

## Conditionals

    if [ "$A" = "$B" ]; then
      echo equal
    else
      echo no
    fi

------------------------------------------------------------------------

## Loops

### for

    for i in {1..5..2}; do echo $i; done

### while

    while [ $x -lt 10 ]; do
      let x++
    done

### until

    until [ $x -lt 10 ]; do
      let x--
    done

------------------------------------------------------------------------

## Functions

    increment(){
      local counter=0
      inc=${1:-1}
      for i in `seq 10`; do
        echo $counter
        let counter+=inc
      done
    }

------------------------------------------------------------------------

## Extra Tips

-   Always quote variables: "\$var"
-   Use `set -euo pipefail` for safer scripts.
-   `chmod 755 script.sh` is common.
-   Pipe-heavy workflows benefit from intermediate `tee`.

------------------------------------------------------------------------

End of Cheat Sheet.
