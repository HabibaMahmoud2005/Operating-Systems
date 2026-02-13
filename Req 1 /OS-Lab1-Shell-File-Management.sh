#!/bin/bash

rm -rf Lab1
mkdir Lab1

cp words.txt Lab1/
cp numbers.txt Lab1/

paste Lab1/words.txt Lab1/numbers.txt > Lab1/MergedContent.txt

head -n 3 Lab1/MergedContent.txt

sort Lab1/MergedContent.txt > Lab1/SortedMergedContent.txt

echo "The sorted file is :"
cat Lab1/SortedMergedContent.txt

chmod a-r Lab1/SortedMergedContent.txt

sort Lab1/MergedContent.txt | uniq

if ! tr "a-z" "A-Z" < Lab1/SortedMergedContent.txt > Lab1/CapitalSortedMergedContent.txt
then
    echo "Conversion failed due to access denial."
fi

chmod u+r Lab1/SortedMergedContent.txt
tr "a-z" "A-Z" < Lab1/SortedMergedContent.txt > Lab1/CapitalSortedMergedContent.txt

grep -n '^w' Lab1/MergedContent.txt | grep '[0-9]$'

cat Lab1/MergedContent.txt | tr "i" "o" > Lab1/NewMergedContent.txt

paste Lab1/MergedContent.txt Lab1/NewMergedContent.txt
