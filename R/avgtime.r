require(ggplot2)
require(data.table)

mean.trim = function(x) {mean(x, trim = 0.02)}
  
# Load data
df = read.csv('../nebula.csv', header = FALSE, sep = ",", quote = "\"", dec = ".")
colnames(df) = c("LockName", "Iteration", "Count", "NumThreads", "ThreadNum", "Time");
df$FullName = paste(df$LockName, "-", df$NumThreads, sep = " ")

setDT(df)
df[ , TimeDiff := Time - shift(Time), by = c("FullName", "Iteration")]
setDF(df)

df = df[!is.na(df$TimeDiff), ]

avgtime = aggregate(df$TimeDiff, list(df$LockName, df$NumThreads), FUN=mean.trim)
colnames(avgtime) = c("LockName", "NumThreads", "Time");

ggplot(data=avgtime, aes(x=NumThreads, y=Time, group=LockName, colour=LockName)) +
  geom_line() +
  geom_point() +
  ylab("0.02-trimmed mean time between consecutive lock() in ns") +
  xlab("Number of threads") 

ggsave(paste("../report/fig/meantime_", "all", ".pdf", sep=""), height=8, width=12, dpi=1000)

