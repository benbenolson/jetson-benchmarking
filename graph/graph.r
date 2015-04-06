library("ggplot2")
library("scales")

number_ticks <- function(n) { function(limits) pretty(limits, n) }

wash_frames60 <- read.csv("wash_frames60.csv")
wash_frames1200 <- read.csv("wash_frames1200.csv")
jetson_frames60 <- read.csv("jetson_frames60.csv")

png("wash_frames60.png", width=1600, height=900, res=150)
ggplot(wash_frames60, aes(x=numthreads, y=time, col=exec)) +
  geom_line() +
  scale_x_continuous(breaks=seq(0,300,by=10)) + 
  scale_y_continuous(breaks=seq(0,100,by=5)) + 
  ylab("Frames Per Second") + 
  xlab("Number of Threads")

png("wash_frames1200.png", width=1600, height=900, res=150)
ggplot(wash_frames1200, aes(x=numthreads, y=time, col=exec)) +
  geom_line() + 
  scale_x_continuous(breaks=seq(0,300,by=10)) + 
  scale_y_continuous(breaks=seq(0,100,by=5)) + 
  ylab("Frames Per Second") + 
  xlab("Number of Threads")

png("jetson_frames60.png", width=1600, height=900, res=150)
ggplot(jetson_frames60, aes(x=numthreads, y=time, col=exec)) +
  geom_line() +
  scale_x_continuous(breaks=seq(0,300,by=10)) + 
  scale_y_continuous(breaks=seq(0,100,by=5)) + 
  ylab("Frames Per Second") + 
  xlab("Number of Threads")

dev.off()
