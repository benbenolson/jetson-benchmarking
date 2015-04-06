library("ggplot2")

wash_frames60 <- read.csv("wash_frames60.csv")
wash_frames1200 <- read.csv("wash_frames1200.csv")
jetson_frames60 <- read.csv("jetson_frames60.csv")

png("wash_frames60.png", width=1600, height=900, res=300)
ggplot(wash_frames60, aes(x=numthreads, y=time, col=exec)) +
  geom_line()

png("wash_frames1200.png", width=1600, height=900, res=300)
ggplot(wash_frames1200, aes(x=numthreads, y=time, col=exec)) +
  geom_line()

png("jetson_frames60.png", width=1600, height=900, res=300)
ggplot(jetson_frames60, aes(x=numthreads, y=time, col=exec)) +
  geom_line()

dev.off()
