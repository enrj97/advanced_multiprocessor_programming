require(ggplot2)

mean.trim = function(x) {mean(x, trim = 0.05)}

# Load data
df1 = read.csv('../nebula_2.csv', header = TRUE, sep = ",", quote = "\"", dec = ".")
df2 = read.csv('../nebula_16.csv', header = TRUE, sep = ",", quote = "\"", dec = ".")
df3 = read.csv('../nebula_128.csv', header = TRUE, sep = ",", quote = "\"", dec = ".")
df4 = read.csv('../nebula_1024.csv', header = TRUE, sep = ",", quote = "\"", dec = ".")

df = rbind(df1, df2, df3, df4)

df$FullName = paste(df$LockName, "-", df$NumThreads, sep = " ")

avgmaxtime = aggregate(df$Time, list(df$LockName, df$NumThreads, df$CSIterations), FUN=mean.trim)
colnames(avgmaxtime) = c("LockName", "NumThreads", "CSIterations", "Time");

for (i in c(2, 16, 128, 1024)) {
  ggplot(data=avgmaxtime[avgmaxtime$CSIterations == i,], aes(x=NumThreads, y=Time, group=LockName, colour=LockName, shape=LockName)) +
    geom_line() +
    geom_point() +
    ylab("0.05-trimmed mean time to completition in microseconds") +
    xlab("Number of threads") 
  
  ggsave(paste("../report/fig/meantime_", i, "_all", ".pdf", sep=""), height=8, width=12, dpi=1000)
}

avgmaxtime = avgmaxtime[avgmaxtime$LockName != "FilterLock",]
for (i in c(2, 16, 128, 1024)) {
  ggplot(data=avgmaxtime[avgmaxtime$CSIterations == i,], aes(x=NumThreads, y=Time, group=LockName, colour=LockName, shape=LockName)) +
    geom_line() +
    geom_point() +
    ylab("0.05-trimmed mean time to completition in microseconds") +
    xlab("Number of threads") 
  
  ggsave(paste("../report/fig/meantime_", i, "_no_filter", ".pdf", sep=""), height=8, width=12, dpi=1000)
}
