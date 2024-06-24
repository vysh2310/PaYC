#!/bin/bash
PROG=PID

make clean
time polyspace-configure -prog $PROG -allow-overwrite -output-project ./verif/$PROG.psprj -output-options-file verif/$PROG.opts make
