import os
import numpy as np
import keras
import random
import matplotlib.pyplot as plt
from keras.models import Sequential
from keras.models import load_model
from keras import regularizers
from keras.layers import Dense,Dropout,Flatten,Conv2D,MaxPooling2D,Reshape,Conv1D,MaxPooling1D,GlobalAveragePooling1D
from keras.layers import Embedding
from keras.layers import LSTM
from keras.constraints import maxnorm
from keras.optimizers import SGD
from keras.utils import np_utils

def show_train_history(train_history, train, validation):
    plt.plot(train_history.history[train])
    plt.plot(train_history.history[validation])
    plt.title("Train History")
    plt.ylabel(train)
    plt.xlabel('Epoch')
    plt.show()

def createCNNModel_smart_necklace(num_classes):
    # Create the model
    model = Sequential()
    model.add(Conv1D(filters=128,
                     kernel_size=16,
                     padding='same',
                     input_shape=(17500, 1),
                     activation='relu'))
    model.add(MaxPooling1D(strides=8))
    model.add(Conv1D(filters=256,
                     kernel_size=16,
                     padding='same',
                     activation='relu'))
    model.add(MaxPooling1D(strides=8))

    model.add(Reshape((-1,)))
    model.add(Dropout(0.2, input_shape=(1,70144)))
    model.add(Dense(1024, kernel_regularizer=regularizers.l2(0.01),
                activation='relu'))
    model.add(Dropout(0.1, input_shape=(1,1024)))
    model.add(Dense(1024, kernel_regularizer=regularizers.l2(0.01),
                activation='relu'))
    model.add(Dense(128, activation='relu'))
    model.add(Dense(num_classes, activation='softmax'))

    # Compile model
    epochs = 30  # >>> should be 25+
    lrate = 0.001
    #decay = lrate/epochs
    sgd = SGD(lr=lrate, momentum=0.9, decay=0.0000001, nesterov=False)
    model.compile(loss='categorical_crossentropy', optimizer=sgd, metrics=['accuracy'])
    #model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
    print(model.summary())
    return model, epochs

def createCNNModel_smart_necklace2(num_classes):
    # Create the model
    model = Sequential()
    model.add(Conv1D(filters=100,
                     kernel_size=1,
                     padding='same',
                     input_shape=(17500, 1),
                     activation='relu'))
    model.add(Conv1D(filters=100,
                     kernel_size=1,
                     padding='same',
                     activation='relu'))
    model.add(MaxPooling1D(strides=8))
    model.add(Conv1D(filters=100,
                     kernel_size=1,
                     padding='same',
                     activation='relu'))
    model.add(Conv1D(filters=100,
                     kernel_size=1,
                     padding='same',
                     activation='relu'))
    #model.add(GlobalAveragePooling1D())
    model.add(MaxPooling1D(strides=8))
    model.add(Dropout(0.5))
    model.add(Reshape((-1,)))
    model.add(Dense(1024, activation='relu'))
    model.add(Dense(1024, activation='relu'))
    model.add(Dense(128, activation='relu'))
    model.add(Dense(num_classes, activation='softmax'))
    
    # Compile model
    epochs = 20  # >>> should be 25+
    model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
    print(model.summary())
    return model, epochs

def data_generator_smart_necklace(filelist_name, batch_size):
    fp = open(filelist_name)
    image_filename_list = fp.readlines()
    total_image_count = len(image_filename_list)
    image_random_index = random.sample(range(total_image_count),total_image_count)

    batchCount = 0
    p = 0
    while True:
        images = []
        labels = []
        for i in range(batch_size):            
            fn = image_filename_list[image_random_index[p]]
            fn = fn[:-1]
            p += 1
            if p >= total_image_count: p = 0
            class0_flag = "class0" in fn
            class1_flag = "class1" in fn
            class2_flag = "class2" in fn
            class3_flag = "class3" in fn
            if class0_flag: 
                category = 0
            elif class1_flag:
                category = 1
            elif class2_flag:
                category = 2
            elif class3_flag:
                category = 3
            else:
                category = 0

            fp_data = open(fn)
            lines = fp_data.readlines()
            xd = []
            for x in lines:
                xv = float(x[:-1])
                xd.append(xv)
            images.append(xd)
            labels.append(category)
            batchCount += 1
            
            if batchCount >= batch_size:
                batchCount = 0
                X_data = images
                y_data = labels
                X_data = np.array(X_data).astype('float32')
                #X_data = X_data * 255.0
                # one hot encode outputs
                y_data = np.array(y_data)
                y_data = np_utils.to_categorical(y_data, num_classes=3)
                X_data = np.expand_dims(X_data, axis=2)
                yield X_data,y_data

def load_data_smart_necklace(filelist_name):
    fp = open(filelist_name)
    image_filename_list = fp.readlines()
    total_image_count = len(image_filename_list)
    image_random_index = random.sample(range(total_image_count),total_image_count)

    images = []
    labels = []
    for i in range(total_image_count):            
        fn = image_filename_list[image_random_index[i]]
        fn = fn[:-1]
        fp_data = open(fn)
        lines = fp_data.readlines()
        xd = []
        for x in lines:
            xv = float(x[:-1])
            xd.append(xv)

        class0_flag = "class0" in fn
        class1_flag = "class1" in fn
        class2_flag = "class2" in fn
        class3_flag = "class3" in fn
        if class0_flag: 
            category = 0
        elif class1_flag:
            category = 1
        elif class2_flag:
            category = 2
        elif class3_flag:
            category = 3
        else:
            category = 0

        images.append(xd)
        labels.append(category)

    return images, labels