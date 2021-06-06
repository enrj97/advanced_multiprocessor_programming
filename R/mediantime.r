require(ggplot2)
require(data.table)

# Load data
df = read.csv('../nebula.csv', header = FALSE, sep = ",", quote = "\"", dec = ".")
colnames(df) = c("LockName", "Iteration", "Count", "NumThreads", "ThreadNum", "Time");
df$FullName = paste(df$LockName, "-", df$NumThreads, sep = " ")

setDT(df)
df[ , TimeDiff := Time - shift(Time), by = c("FullName", "Iteration")]
setDF(df)

df = df[!is.na(df$TimeDiff), ]

mediantime = aggregate(df$TimeDiff, list(df$LockName, df$NumThreads), FUN=median)
colnames(mediantime) = c("LockName", "NumThreads", "Time");

ggplot(data=mediantime, aes(x=NumThreads, y=Time, group=LockName, colour=LockName)) +
  geom_line() +
  geom_point() +
  ylab("Median time between consecutive lock() in ns") +
  xlab("Number of threads") 

ggsave(paste("../report/fig/mediantime_", "all", ".pdf", sep=""), height=8, width=12, dpi=1000)

