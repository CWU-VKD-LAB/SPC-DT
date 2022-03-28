from sklearn.model_selection import train_test_split
import pandas as pd

breastData = pd.read_excel("breast.xlsx")

X = breastData.drop(['class'], axis=1).values
y = breastData['class'].values

train, test = train_test_split(breastData, train_size=.90, test_size=.10, random_state=42, stratify=y)

train.to_csv(r'./breast_train.csv', index=False)
test.to_csv(r'./breast_test.csv', index=False)
train.to_csv(r'./breast_train.txt', sep='\t', line_terminator='\n', index=False)
test.to_csv(r'./breast_test.txt', sep='\t', line_terminator='\n', index=False)
