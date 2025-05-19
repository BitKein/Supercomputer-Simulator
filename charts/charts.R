

# Sample data
x <- c(0,273,607,638,934,1192,1239,2190,2310,2790,3744,6083,6486,9582,10284,10443,10776,11213,12754,12795,14966,16093,16225,16465,19160,19933,19974,22458,24173,24850,26789,27252,31002,31100,35354,36676,36712,36890,37440,37441,37867,39506,41701,53317,53317,53344,53929,58002,59550,59672,62451)   # X-axis values (time, categories, etc.)
y <- c(32,31,31,31,32,32,32,32,30,30,32,30,30,30,32,32,32,30,30,26,28,32,32,32,28,32,30,30,28,28,20,28,20,20,20,24,32,32,32,32,32,32,16,32,32,32,32,32,32,32,0)  # Y-axis values (data points)

setwd("C:/Users/amq18/Desktop/Supercomputer-Simulator/charts")
jpeg("my_plot.jpg", width = 1400, height = 600)  # Set file name & size


# Create a line chart
plot(x, y, type="o", col="blue", xlab="Momento de la ejecución", ylab="Número de cores en uso", main="Utilización")

dev.off()
