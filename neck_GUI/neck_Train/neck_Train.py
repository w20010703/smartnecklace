import os
import numpy as np
import utils
import keras
import matplotlib.pyplot as plt
from keras.models import Sequential
from keras.models import load_model
from keras.callbacks import ModelCheckpoint
from keras.utils import np_utils
from sklearn.model_selection import train_test_split

# create CNN model
num_classes = 4
model, epochs_val = utils.createCNNModel_smart_necklace(num_classes)
print("CNN Model Smart Necklace created.")

# load image data 
images, labels = utils.load_data_smart_necklace("D:/temp/0723/filelist4.txt")
# print("Data loaded")

#train_filelist = "D:/Software/ZoneDevelope/ScienceFair/Neck/data_0716/filelist.txt"

#tgen = utils.data_generator_smart_necklace(train_filelist, 5) # Train data generator
#vgen = utils.data_generator_smart_necklace(train_filelist, 5)
print("Data loaded")

X_train, X_test, y_train, y_test = train_test_split(images, labels, test_size=0.2, random_state=0)

#X_train = images
#y_train = labels

#X_test = X_train
#y_test = y_train

# normalize inputs from 0-255 and 0.0-1.0
X_train = np.array(X_train).astype('float32')
X_test = np.array(X_test).astype('float32')
X_train = X_train / 4095.0
X_test = X_test / 4095.0
# one hot encode outputs
y_train = np.array(y_train)
y_test = np.array(y_test)
y_train = np_utils.to_categorical(y_train, num_classes=4)
y_test = np_utils.to_categorical(y_test, num_classes=4)
num_classes = y_test.shape[1]
print("Data normalized and hot encoded.")

X_train = np.expand_dims(X_train, axis=2)
X_test = np.expand_dims(X_test, axis=2)

# fit and run our model
seed = 7
np.random.seed(seed)

train_history = model.fit(X_train, y_train, validation_data=(X_test, y_test), epochs=30, batch_size=16)

# checkpoint
#filepath="c:/temp/weights/weights-improvement-{epoch:02d}-{val_acc:.4f}.h5"
#checkpoint = ModelCheckpoint(filepath, monitor='val_acc', verbose=1, save_best_only=True, mode='max')
#callbacks_list = [checkpoint]

#samples_val = int(130/5) + 1
#samples_val = 130
#validate_val = int(samples_val*0.25) + 1

#train_history = model.fit_generator(tgen, samples_per_epoch=samples_val, epochs=epochs_val, validation_data=vgen, validation_steps=validate_val, callbacks=callbacks_list)
#train_history = model.fit_generator(tgen, samples_per_epoch=samples_val, epochs=epochs_val, validation_data=vgen, validation_steps=validate_val)

model.save("c:/temp/smartnecklace_0726_water_2_cough_epoch30.h5")

utils.show_train_history(train_history, 'acc', 'val_acc')

print("Training done")

