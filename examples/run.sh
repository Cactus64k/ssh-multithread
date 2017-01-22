#!/bin/bash

mkdir -p /tmp/ssh-multithread
ssh-multithread --script=ololo.sh --machines=ololo.cfg --output_folder=/tmp/ssh-multithread --threads=4
read -p "Press enter to continue"
