require(ggplot2)
require(data.table)

# Load data
df = read.csv('../nebula.csv', header = FALSE, sep = ",", quote = "\"", dec = ".")
colnames(df) = c("LockName", "Count", "NumThreads", "ThreadNum", "Time");
df$FullName = paste(df$LockName, "-", df$NumThreads, sep = " ")
df$ThreadFactor = as.factor(df$NumThreads)

setDT(df)
df[ , TimeDiff := Time - shift(Time), by = c("FullName", "ThreadNum")]
setDF(df)

df = df[!is.na(df$TimeDiff), ]
df = df[df$TimeDiff < 2000, ]
df = df[df$TimeDiff >= 0, ]

for (level in levels(df$LockName)) {
  ggplot(df[df$LockName == level,], aes(x=ThreadFactor, y=TimeDiff)) + 
    geom_boxplot(outlier.colour="black", outlier.shape=16, outlier.size=2) +
    ggtitle(level) +
    xlab("Number of Threads")
  # theme(axis.text.x = element_text(angle = 90))
  
  ggsave(paste("../report/fig/time_", level, ".pdf", sep=""), height=6, width=8, dpi=1000)
}

ggplot(df, aes(x=FullName, y=TimeDiff)) + 
  geom_boxplot(outlier.colour="black", outlier.shape=16, outlier.size=2) +
  xlab("Lock")+
  theme(axis.text.x = element_text(angle = 90))

ggsave(paste("../report/fig/time_", "all", ".pdf", sep=""), height=4, width=12, dpi=1000)
