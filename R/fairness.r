require(ggplot2)
require(data.table)

# Load data
df = read.csv('../nebula.csv', header = FALSE, sep = ",", quote = "\"", dec = ".")
colnames(df) = c("LockName", "NumThreads", "Count", "ThreadNum", "Time");
df$FullName = paste(df$LockName, "-", df$NumThreads, sep = " ")

df = df[df$NumThreads==8,]
fair = aggregate(df, list(df$FullName, df$ThreadNum), FUN=length)

ggplot(fair, aes(x=Group.1, y=NumThreads)) + 
  geom_boxplot(outlier.colour="black", outlier.shape=16, outlier.size=2) +
  xlab("Lock")+
  theme(axis.text.x = element_text(angle = 90))

ggsave(paste("../report/fig/fairness_", "all", ".pdf", sep=""), height=8, width=12, dpi=1000)

