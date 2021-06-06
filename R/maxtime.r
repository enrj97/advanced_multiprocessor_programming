require(ggplot2)

mean.trim = function(x) {mean(x, trim = 0.02)}

# Load data
df = read.csv('../nebula.csv', header = FALSE, sep = ",", quote = "\"", dec = ".")
colnames(df) = c("LockName", "Iteration", "Count", "NumThreads", "ThreadNum", "Time");
df$FullName = paste(df$LockName, "-", df$NumThreads, sep = " ")


maxtime = aggregate(df$Time, list(df$LockName, df$NumThreads, df$Iteration), FUN=max)
colnames(maxtime) = c("LockName", "NumThreads", "Iteration", "Time");

avgmaxtime = aggregate(maxtime$Time, list(maxtime$LockName, maxtime$NumThreads), FUN=mean.trim)
colnames(avgmaxtime) = c("LockName", "NumThreads", "Time");

ggplot(data=avgmaxtime, aes(x=NumThreads, y=Time, group=LockName, colour=LockName)) +
  geom_line() +
  geom_point() +
  ylab("0.02-trimmed mean time to completition in ns") +
  xlab("Number of threads") 

ggsave(paste("../report/fig/meantime_", "all", ".pdf", sep=""), height=8, width=12, dpi=1000)

