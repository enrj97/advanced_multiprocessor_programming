require(ggplot2)

# 256 runs for 128 batches
totalRuns = 256*128

# Load data
df = read.csv('../nebula.csv', header = FALSE, sep = ",", quote = "\"", dec = ".")
colnames(df) = c("LockName", "Iteration", "Count", "NumThreads", "ThreadNum", "Time");
df$FullName = paste(df$LockName, "-", df$NumThreads, sep = " ")

fair = aggregate(df$Count, list(df$LockName, df$NumThreads, df$ThreadNum), FUN=length, drop=FALSE)
colnames(fair) = c("LockName", "NumThreads", "ThreadNum", "Count");
fair = fair[fair$ThreadNum < fair$NumThreads,]
fair[is.na(fair$Count), "Count"] = 0
fair$Unfairness = fair$Count * fair$NumThreads / totalRuns

fairAggregate = aggregate(fair$Unfairness, list(fair$LockName, fair$NumThreads), FUN = sd)
colnames(fairAggregate) = c("LockName", "NumThreads", "Unfairness");
fairAggregate$Unfairness = fairAggregate$Unfairness / sqrt(fairAggregate$NumThreads)

ggplot(data=fairAggregate, aes(x=NumThreads, y=Unfairness, group=LockName, colour=LockName)) +
  geom_line()+
  geom_point()

ggsave(paste("../report/fig/fairness_", "all", ".pdf", sep=""), height=8, width=12, dpi=1000)

# Check that all have 128*128 iterations
aggregate(fair$Count, list(fair$LockName, fair$NumThreads), FUN = sum)
