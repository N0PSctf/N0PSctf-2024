#!/usr/bin/env bash

./lizwol flag.txt flag.enc
mkdir -p Lizwol
cp flag.enc	Lizwol/flag.enc
cp lizwol	Lizwol/lizwol

# Test
./solve flag.enc flag_recover.txt
diff flag.txt flag_recover.txt