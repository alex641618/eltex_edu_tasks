#!/bin/bash

gcc password.c -o password -fno-stack-protector -g
./password < pass.txt
rm -rf password
