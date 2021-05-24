require(ggplot2)
require(data.table)

# Load data
df = read.csv('../locks.csv', header = FALSE, sep = ",", quote = "\"", dec = ".")
colnames(df) = c("LockName", "NumThreads", "Count", "ThreadNum", "Time");
df$Title = paste(df$LockName, "-", df$NumThreads, sep = " ")

setDT(df)
df[ , TimeDiff := Time - shift(Time), by = c("Title", "ThreadNum")]
setDF(df)

df = df[df$TimeDiff < 100, ]

ggplot(df, aes(x=Title, y=TimeDiff)) + 
  geom_boxplot(outlier.colour="black", outlier.shape=16, outlier.size=2) +
  xlab("Lock") +
  theme(axis.text.x = element_text(angle = 90))

ggsave("../report/fig/boxplot_all.pdf", height=4, width=10, dpi=1000)
