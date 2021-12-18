import pandas as pd

if __name__ == '__main__':
    data = pd.read_excel(r'Wisconsin-breast-cancer-no-question-marks.xls')
    df = pd.DataFrame(data)
    df = df.drop(['class', 'id'], axis=1)
    normalized = df / 10.0
    print(normalized)
