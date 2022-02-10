from lib2to3.pgen2.token import N_TOKENS
from tabnanny import verbose
from debugpy import trace_this_thread
import graphviz
import numpy as np
import sklearn.datasets
from sklearn import tree
from sklearn.datasets import load_iris
from sklearn.datasets import load_breast_cancer
import pandas as pd
from sklearn.metrics import accuracy_score, classification_report, precision_score
from sklearn.model_selection import GridSearchCV, KFold, StratifiedKFold, cross_val_score, cross_validate
from sklearn.model_selection import train_test_split
import hashlib
from sklearn.metrics import classification_report, accuracy_score, make_scorer


fold = 0
def classification_report_with_accuracy_score(target_true, target_pred):
    global fold
    fold += 1
    print("***** FOLD {} *****".format(fold))
    print(classification_report(target_true, target_pred))
    return 0

# Mushroom dataset
input_file = "./SimpleMushroomData.csv"

# Read mushroom from csv
df = pd.read_csv(input_file, header=0)

# Split data into features and labels
attr_names = np.array(df.columns.values[1:23])
target_names = np.array(["poison", "edible"])

# Split data into attributes and labels
data = df.values[:, 1:23]
target = df.values[:, 23]

#TODO Do our own breast cancer dataset

# debug with cancer data
# breast = load_breast_cancer()
# data = breast.data
# target = breast.target
# attr_names = breast.feature_names
# target_names = breast.target_names

# Split data into training and testing sets
data_train, data_test, target_train, target_test = train_test_split(data, target, test_size=0.2, stratify=target)

# Create a decision tree classifier
clf = tree.DecisionTreeClassifier()

# Fit the classifier to the training data
clf = clf.fit(data_train, target_train)

# Evaluate the classifier on the test data
print("Traditional Test Data Evaluation Score:", clf.score(data_test, target_test))

# Reset the classifier
# clf = tree.DecisionTreeClassifier()

# n-fold cross validation parameters
n_folds = 10
n_jobs = 1 # thread count
n_times = 10

# Create dictionary of parameters to search
# scoring = {'accuracy': 'accuracy', 'precision': 'precision'}
scoring = {'accuracy': 'accuracy', 'precision': 'precision', 'printClassificationReport': make_scorer(classification_report_with_accuracy_score)}

# Perform n-fold cross validation
print("Performing", str(n_folds) + "-fold cross validation...")
# scoreMap = cross_validate(estimator=tree.DecisionTreeClassifier(), X=data, y=target, cv=n_folds, n_jobs=n_jobs, verbose=True, scoring=scoring)
scoreMap = cross_validate(estimator=tree.DecisionTreeClassifier(), X=data, y=target, cv=n_folds, n_jobs=n_jobs, verbose=10, scoring=scoring, return_estimator=True, return_train_score=True)
clf = scoreMap['estimator'][0]
print()

# TODO LOOK INTO PRECISION DATA
    # accuracy vs precision
    # when creating rules for one class, we can only compute precision
    #   recall: confusion matrix
    #                   predicted class 0  |  predicted class 1  |  num correct out of total in class   |     coverage / recall   TODO: Look into recall formula
    #   class 0 actual          3000(support is only 3000) 1000              3000/(3000 + 1000) = 75%                   100% (no incorrect predictions)
    #                                                               (need to know total num in class 0)
    #                           3000                 0                  3000/(3000 + 0) = 100%                  75%
    #   class 1 actual          3500               500                3500/(3500 + 500) = 75%                   100% (no incorrect predictions)

    #   accuracy:                                                       75%                             (3000 + 3500) / total num cases (3000 + 1000 + 500)
    #                                                                                                      6400 / 8000

    #   f: sum up and divide. Combine accuracy and precision. Not good because no clear meaning. Closer to 1 means better
    # support: given a rule: A(x)=>B(x) (antecident => consequent) num cases where antecident is true
        #      compute all cases where A(x) is true = X: A(x)=True. This is called "support"
    # TODO: look up: macro average: macro average is their term
    #               could be just a summed version with no weights
    # weighted average: Rule1: A=>B (16 cases) (accuracy = .9)
    #                   Rule2: C=>B (160 cases) (accuracy = .7)
    #                   Rule3: D=>B (ie, b: beniegn) (1600 cases) (accuracy = .8)
    #                                                  average accuracy is .7
    #                                                  weighted average accuracy : ().9 * 16 + .7 * 160 + .8 * 1600) / (1600 + 160 + 16)



# Print scores
for i in range(n_folds):
    print("Fold:", str(i + 1),"", "| accuracy:", scoreMap['test_accuracy'][i], "|", "precision:", scoreMap['test_precision'][i])
print()

# Print average and standard deviation of scores
print("Average Accuracy:", scoreMap['test_accuracy'].mean(), "\t|", "Accuracy Standard Deviation:", scoreMap['test_accuracy'].std())
print("Average Precision:", scoreMap['test_precision'].mean(), "\t|", "Precision Standard Deviation:", scoreMap['test_precision'].std())

# Use graphviz to visualize the decision tree
dot_data = tree.export_graphviz(clf, out_file=None,
                                feature_names=attr_names,
                                class_names=target_names,
                                filled=True, rounded=True,
                                special_characters=True)
graph = graphviz.Source(dot_data)

# Write graph to file
graph.render("MushroomDecisionTree")
