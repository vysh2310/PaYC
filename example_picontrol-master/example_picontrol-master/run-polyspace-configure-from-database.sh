#!/bin/bash
PROG=PID
time polyspace-configure -compiler-cache-path psccache -prog $PROG -allow-overwrite -output-project ./verif/$PROG.psprj -output-options-file verif/$PROG.opts -compilation-database build/compile_commands.json
