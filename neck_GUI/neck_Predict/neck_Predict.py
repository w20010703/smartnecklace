
import os
import cv2
import numpy as np
import utils
import keras
import time
from keras.models import load_model

model = load_model("c:/temp/smartnecklace_0726_water_2_cough_epoch30.h5")

f = "D:/Temp/0723/capture_data.txt"
fp_data = open(f)
lines = fp_data.readlines()

xd = []
images = []
p = 0
for x in lines:
    xstr = x.strip()
    xstr_list = xstr.split(':')
    xv = float(xstr_list[1])
    xd.append(xv)

file = open("D:/Temp/0723/result.txt", 'w')

for i in range(0,len(xd)-17500+1,3500):
    xd1 = []
    images = []
    xd1 = xd[i:i+17500]
    images.append(xd1)
    X_data = images
    # normalize inputs from 0-255 and 0.0-1.0
    X_data = np.array(X_data).astype('float32')
    X_data = X_data / 4095.0
    X_data = np.expand_dims(X_data, axis=2)
    predictions = model.predict(X_data)
    # print("result=", i, f, predictions)
    buf = "%6d %f %f %f %f\n" % (i, predictions[0][0], predictions[0][1], predictions[0][2], predictions[0][3])
    file.writelines(buf)
    print(i, predictions[0][0], predictions[0][1], predictions[0][2], predictions[0][3])

file.close()


