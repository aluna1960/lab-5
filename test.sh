#!/bin/bash

for N in {1..50}
do
    ./client $(((($N % 7)+5)*100)) &
done
wait
