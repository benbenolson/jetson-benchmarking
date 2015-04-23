library("ggplot2")

wash_frames60 <- read.csv("wash_frames60.csv")
wash_frames1200 <- read.csv("wash_frames1200.csv")
jetson_frames60 <- read.csv("jetson_frames60.csv")
jetson_frames1200 <- read.csv("jetson_frames1200.csv")
pi_frames60 <- read.csv("pi_frames60.csv")
marszx3_frames60 <- read.csv("marszx3_frames60.csv")
combined_frames60_nothread <- read.csv("combined_frames60_nothread.csv")
combined_frames60_thread <- read.csv("combined_frames60_thread.csv")
combined_frames60_threadpool <- read.csv("combined_frames60_threadpool.csv")

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

png("jetson_frames1200.png", width=1600, height=900, res=150)
ggplot(jetson_frames1200, aes(x=numthreads, y=time, col=exec)) +
  geom_line() +
  scale_x_continuous(breaks=seq(0,300,by=10)) + 
  scale_y_continuous(breaks=seq(0,100,by=5)) + 
  ylab("Frames Per Second") + 
  xlab("Number of Threads")

png("pi_frames60.png", width=1600, height=900, res=150)
ggplot(pi_frames60, aes(x=numthreads, y=time, col=exec)) +
  geom_line() +
  scale_x_continuous(breaks=seq(0,300,by=10)) + 
  scale_y_continuous(breaks=seq(0,5,by=0.25)) + 
  ylab("Frames Per Second") + 
  xlab("Number of Threads")

png("marszx3_frames60.png", width=1600, height=900, res=150)
ggplot(marszx3_frames60, aes(x=numthreads, y=time, col=exec)) +
  geom_line() +
  scale_x_continuous(breaks=seq(0,300,by=10)) + 
  scale_y_continuous(breaks=seq(0,10,by=0.5)) + 
  ylab("Frames Per Second") + 
  xlab("Number of Threads")

png("combined_frames60_nothread.png", width=1600, height=900, res=150)
ggplot(combined_frames60_nothread, aes(x=numthreads, y=time, col=exec)) +
  geom_line() +
  scale_x_continuous(breaks=seq(0,300,by=10)) + 
  scale_y_continuous(breaks=seq(0,100,by=5)) + 
  ylab("Frames Per Second") + 
  xlab("Number of Threads")

png("combined_frames60_thread.png", width=1600, height=900, res=150)
ggplot(combined_frames60_thread, aes(x=numthreads, y=time, col=exec)) +
  geom_line() +
  scale_x_continuous(breaks=seq(0,300,by=10)) + 
  scale_y_continuous(breaks=seq(0,100,by=5)) + 
  ylab("Frames Per Second") + 
  xlab("Number of Threads")

png("combined_frames60_threadpool.png", width=1600, height=900, res=150)
ggplot(combined_frames60_threadpool, aes(x=numthreads, y=time, col=exec)) +
  geom_line() +
  scale_x_continuous(breaks=seq(0,300,by=10)) + 
  scale_y_continuous(breaks=seq(0,100,by=5)) + 
  ylab("Frames Per Second") + 
  xlab("Number of Threads")

dev.off()
