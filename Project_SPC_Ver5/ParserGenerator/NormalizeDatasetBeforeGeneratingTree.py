from sklearn import preprocessing
from sklearn.preprocessing import LabelEncoder
import pandas as pd

dataFile = pd.read_csv("iris.data")

print(dataFile.head())

print(dataFile['class'].unique())

labelEncoder = LabelEncoder()

labelEncoder.fit(dataFile['class'])

classLabels = labelEncoder.transform(dataFile['class'])

print(classLabels)

dataFile['class'] = classLabels

print(dataFile)

minMaxScalar = preprocessing.MinMaxScaler(feature_range=(0,10))

col = dataFile.columns
result = minMaxScalar.fit_transform(dataFile)
minMaxScalarFrame = pd.DataFrame(result, columns=col)

minMaxScalarFrame['class'] = classLabels

print(minMaxScalarFrame)