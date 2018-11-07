#!/bin/bash
index=0

for ((i=1;i<=9;i++))
do
   #let index+=$i
   ffplay out$i.mov -autoexit
done
