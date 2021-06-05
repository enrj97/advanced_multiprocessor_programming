require(ggplot2)
require(data.table)

# Load data
df = read.csv('../nebula.csv', header = FALSE, sep = ",", quote = "\"", dec = ".")
colnames(df) = c("LockName", "Count", "NumThreads", "ThreadNum", "Time");
df$FullName = paste(df$LockName, "-", df$NumThreads, sep = " ")

setDT(df)
df[ , TimeDiff := Time - shift(Time), by = c("FullName", "ThreadNum")]
setDF(df)

df = df[!is.na(df$TimeDiff), ]
df = df[df$TimeDiff < 2000, ]
df = df[df$TimeDiff >= 0, ]

avgtime = aggregate(df$TimeDiff, list(df$LockName, df$NumThreads), FUN=mean)
colnames(avgtime) = c("LockName", "NumThreads", "Time");

ggplot(data=avgtime, aes(x=NumThreads, y=Time, group=LockName, colour=LockName)) +
  geom_line() +
  geom_point() +
  ylab("Mean time between lock() in ns") +
  xlab("Number of threads") 

ggsave(paste("../report/fig/meantime_", "all", ".pdf", sep=""), height=8, width=12, dpi=1000)

