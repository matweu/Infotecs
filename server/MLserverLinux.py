import sys
import numpy as np
import datetime
from sklearn.neighbors import KNeighborsClassifier
 
if len(sys.argv) < 2:
    print("Usage: python my_script.py <data>")
    sys.exit(1)
 
data_str = sys.argv[1]
data = np.array(list(map(int, data_str.split(','))))
 
print("Received data:", data)
 
# Исходные данные
X = [[1, 1, 1, 1, 1, 1, 1, 1, 1],
[0, 0, 0, 1, 1, 1, 1, 1, 1],
[1, 0, 0, 1, 1, 1, 0, 0, 1],
[1, 1, 1, 1, 0, 0, 0, 0, 0],
[1, 0, 1, 0, 1, 1, 1, 0, 1],
[0, 0, 0, 0, 0, 0, 0, 0, 0],
[1, 0, 1, 0, 0, 0, 0, 0, 0],
[1, 0, 0, 1, 1, 1, 0, 0, 1],
[1, 0, 0, 0, 0, 0, 0, 1, 0],
[1, 0, 1, 0, 0, 0, 0, 1, 0],
[1, 0, 0, 1, 1, 0, 0, 0, 1],
[0, 1, 0, 0, 0, 1, 0, 0, 0]]
 
# Названия признаков
feature_names = ['libreoffice', 'gnumeric', 'firefox','gnome-terminal-server', 'gnome-system-monitor', 'nm-connection-editor', 'gnome-control-center', 'seahorse', 'mainwindow.py']
 
# Метки классов
y = [1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0]
 
# Обучение модели
clf = KNeighborsClassifier(n_neighbors=3)
clf.fit(X, y)
 
# Предсказание метки класса
prediction = clf.predict(np.array(data).reshape(1,-1))
 
# Запись результата в файл
if prediction[0] == 1:
    NOWTIME=datetime.datetime.now()
    formate_NOWTIME = NOWTIME.strftime("%Y-%m-%d %H:%M:%S")
    with open('output.txt', 'a') as f:
        #Предупреждение о возможной атаке на Клиент
        f.write(f'Critical values detected,Anomaly time: {formate_NOWTIME} \n')
        for i in range(len(feature_names)):
            if data[i] == 1:
                f.write(feature_names[i] + ': ' + str(data[i]) + '\n')