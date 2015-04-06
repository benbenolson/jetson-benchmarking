library("ggplot2")

frames60 <- read.csv("frames60.csv")
frames1200 <- read.csv("frames1200.csv")

png("frames60.png", width=1600, height=900, res=300)
ggplot(frames60, aes(x=numthreads, y=time, col=exec)) +
  geom_line()

png("frames1200.png", width=1600, height=900, res=300)
ggplot(frames1200, aes(x=numthreads, y=time, col=exec)) +
  geom_line()

dev.off()
