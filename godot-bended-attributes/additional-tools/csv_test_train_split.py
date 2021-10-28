from sklearn.model_selection import train_test_split
import pandas as pd
import os
import sys

def main(input_file, delim, test_ratio):
    df = pd.read_csv(input_file, delim)

    train_df, test_df = train_test_split(df, test_size=float(test_ratio))
    file_name_stub, file_extension = os.path.splitext(input_file)

    train_df = train_df.copy()
    test_df = test_df.copy()

    train_df["train_test"] = 0
    test_df["train_test"] = 1

    train_file = file_name_stub + "_train" + file_extension
    test_file = file_name_stub + "_test" + file_extension

    train_df.to_csv(train_file, delim)
    test_df.to_csv(test_file, delim)

    print("Training data saved to: " + train_file)
    print("Testing data saved to: " + test_file)

if __name__ == '__main__':
    if len(sys.argv) < 4:
        print("Error: Missing command line arguments.")
        print("Usage: python " + os.path.basename(__file__) + " [csv file path] [file delimeter] [test ratio]")
        print("Example: python " + os.path.basename(__file__) + " dataset.csv \",\" 0.2")
        print("Note: \"`t\" can be used in place of a tab character in powershell.")
    else:
        main(sys.argv[1], sys.argv[2], sys.argv[3])