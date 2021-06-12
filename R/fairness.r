require(ggplot2)
library(matrixStats)

threads = c()
for (i in 0:63) {
  threads = append(threads, paste("Thread", i, sep="."))
}

# Load data
df1 = read.csv('../nebula_2.csv', header = TRUE, sep = ",", quote = "\"", dec = ".")
df2 = read.csv('../nebula_16.csv', header = TRUE, sep = ",", quote = "\"", dec = ".")
df3 = read.csv('../nebula_128.csv', header = TRUE, sep = ",", quote = "\"", dec = ".")
df4 = read.csv('../nebula_1024.csv', header = TRUE, sep = ",", quote = "\"", dec = ".")

df = rbind(df1, df2, df3, df4)

df$FullName = paste(df$LockName, "-", df$NumThreads, sep = " ")


df$SD = rowSds(as.matrix(df[,threads]), na.rm = TRUE)
df$Count = rowSums(as.matrix(df[,threads]), na.rm = TRUE)
df$Unfairness = df$SD / df$Count * sqrt(df$NumThreads)

fair = aggregate(df$Unfairness, list(df$LockName, df$NumThreads), FUN = mean)
colnames(fair) = c("LockName", "NumThreads", "Unfairness");

ggplot(data=fair, aes(x=NumThreads, y=Unfairness, group=LockName, colour=LockName)) +
  geom_line()+
  geom_point()

ggsave(paste("../report/fig/fairness_", "all", ".pdf", sep=""), height=8, width=12, dpi=1000)

fair = fair[fair$LockName != "TTAS Lock",]
fair = fair[fair$LockName != "TAS Lock",]

ggplot(data=fair, aes(x=NumThreads, y=Unfairness, group=LockName, colour=LockName)) +
  geom_line()+
  geom_point()

ggsave(paste("../report/fig/fairness_", "no_tas", ".pdf", sep=""), height=8, width=12, dpi=1000)

