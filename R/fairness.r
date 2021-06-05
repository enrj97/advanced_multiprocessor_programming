require(ggplot2)
require(data.table)

# Load data
df = read.csv('../nebula.csv', header = FALSE, sep = ",", quote = "\"", dec = ".")
colnames(df) = c("LockName", "Count", "NumThreads", "ThreadNum", "Time");
df$FullName = paste(df$LockName, "-", df$NumThreads, sep = " ")

fair = aggregate(df, list(df$LockName, df$NumThreads, df$ThreadNum), FUN=length)
fair$Fairness = fair$Count * fair$Group.2 / 1024

ggplot(fair, aes(x=Group.1, y=Fairness)) + 
  geom_boxplot(outlier.colour="black", outlier.shape=16, outlier.size=2) +
  xlab("Lock")+
  theme(axis.text.x = element_text(angle = 90))

ggsave(paste("../report/fig/fairness_", "all", ".pdf", sep=""), height=8, width=12, dpi=1000)


# Load data
df = read.csv('../nebula.csv', header = FALSE, sep = ",", quote = "\"", dec = ".")
colnames(df) = c("LockName", "Count", "NumThreads", "ThreadNum", "Time");
df$FullName = paste(df$LockName, "-", df$NumThreads, sep = " ")

fair = aggregate(df, list(df$LockName, df$NumThreads, df$ThreadNum), FUN=length)
fair$Fairness = fair$Count * fair$Group.2 / 1024

fair$Group.2 = as.factor(fair$Group.2)
fair2 = aggregate(fair$Fairness, list(fair$Group.1, fair$Group.2), FUN=sd)

ggplot(data=fair2, aes(x=Group.2, y=x, group=Group.1, colour=Group.1)) +
  geom_line()+
  geom_point()

ggsave(paste("../report/fig/fairness_", "all", ".pdf", sep=""), height=8, width=12, dpi=1000)