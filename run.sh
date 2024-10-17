set -e

# compile
~/.config/bin/scripts/cpp-compile c

# run
./build/bin/pid_controller > ./data.txt

# plot
gnuplot ./plotscript.gp

# open
open ./data_plot.png

cat ./data.txt
