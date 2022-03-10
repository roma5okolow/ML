import KNN
from matplotlib import pyplot as plt

group, labels = KNN.createDataSet()

fig, axes = plt.figure()
plt.scatter(group, labels = labels)
plt.show()