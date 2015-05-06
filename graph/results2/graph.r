library("ggplot2")
library("plyr")

cudacores_and_split <- read.csv("cudacores_and_split.csv")
numthreads_and_split <- read.csv("numthreads_and_split.csv")

png("cudacores_and_split.png", width=3200, height=1800, res=150)
ggplot(cudacores_and_split, aes(x=cudacores, y=time, fill=reorder(as.factor(split), -split))) +
  geom_area(position="dodge") + 
  geom_text(aes(label=split, colour=reorder(as.factor(split), -split)), size=4, vjust=-1, hjust=1, show_guide=FALSE) +
  scale_x_continuous(breaks=seq(0,512,by=64)) + 
  scale_y_continuous(breaks=seq(0,80,by=4)) +
  xlab("Cuda Cores") +
  ylab("Frames per Second") +
  geom_line(aes(colour=reorder(as.factor(split), -split)), show_guide=FALSE) +
  guides(fill=guide_legend(title="Split"))

png("numthreads_and_split.png", width=3200, height=1800, res=150)
ggplot(numthreads_and_split, aes(x=numthread, y=time, fill=reorder(as.factor(split), -split))) +
  geom_area(position="dodge") + 
  geom_text(aes(label=split, colour=reorder(as.factor(split), -split)), size=4, vjust=-1, hjust=1, show_guide=FALSE) +
  scale_x_continuous(breaks=seq(1,150,by=1)) + 
  scale_y_continuous(breaks=seq(0,80,by=4)) +
  xlab("Number of Threads") +
  ylab("Frames per Second") +
  geom_line(aes(colour=reorder(as.factor(split), -split)), show_guide=FALSE) +
  guides(fill=guide_legend(title="Split")) + 
  coord_cartesian(xlim = c(1, 20)) 

dev.off()
