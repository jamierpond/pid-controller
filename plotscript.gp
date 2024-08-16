# Set the output terminal (you can change this to 'png' or other formats if needed)
set terminal pngcairo

# Set the output file name
set output 'data_plot.png'

# Set the title of the plot
set title 'Data Plot'

# Set labels for x and y axes
set xlabel 'Index'
set ylabel 'Value'

# Plot the data
plot 'data.txt' with linespoints title 'Data'

# If you want to display the plot on screen (optional)
# set terminal wxt
# replot
