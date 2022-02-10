#!/usr/bin/env python
# coding: utf-8

# In[1]:


import pandas as pd
import numpy as np
import matplotlib.pylab as plt
from matplotlib.pylab import rcParams
from datetime import datetime
rcParams['figure.figsize'] = 15, 6


# In[2]:


import xlrd
import numpy as np
#import cv2
exfile = xlrd.open_workbook('data.xls')
data = exfile.sheet_by_name('ionosphere')
Array=[]
#BLUE=[255,0,0]
for row in range(351):
    List=[]
    for column in range(35):
        List.append(data.cell(row,column).value)
    for column in range(34,36):
        List.append(data.cell(row,column).value)
    Array.append(List)
    


# In[3]:


import math
w= 50
def getXy(i1a,i1b,i1c,i2a,i2b,i2c,i3a,i3b,i3c,i4a,i4b,i4c,i5a,i5b,i5c,i6a,i6b,i6c,i7a,i7b,i7c,i8a,i8b,i8c,i9a,i9b,i9c,
          i10a,i10b,i10c,i11a,i11b,i11c,i12a,i12b,i12c,i13a,i13b,i13c,i14a,i14b,i14c,i15a,i15b,i15c,i16a,i16b,i16c,
          i17a,i17b,i17c):
 imglist=[]
 cnt =0
    
 for i in range(len(Array)):
    img=np.ones(shape=(10,10,3))
    r=int(Array[i][33])-1
    c=int(Array[i][34])-1
    if img[r][c][0]!=1:
        shift=1
        while(1):
           if c-shift>=0 and img[r][c-shift][0]==1:
               c-=shift
               break
           elif r-shift>=0 and img[r-shift][c][0]==1:
                r-=shift
                break
           elif c+shift<10 and img[r][c+shift][0]==1:
                c-=shift
                break
           elif r+shift<10 and img[r+shift][c][0]==1:
                r+=shift
                break
           shift+=1
    if img[r][c][0]!=1:
        cnt+=1
    img[r][c][0]=i1a
    img[r][c][1]=i1b
    img[r][c][2]=i1c
    
    r=int(Array[i][31])-1
    c=int(Array[i][32])-1
    if img[r][c][0]!=1:
        shift=1
        while(1):
           if c-shift>=0 and img[r][c-shift][0]==1:
             c-=shift
             break
           elif r-shift>=0 and img[r-shift][c][0]==1:
                r-=shift
                break
           elif c+shift<10 and img[r][c+shift][0]==1:
                c-=shift
                break
           elif r+shift<10 and img[r+shift][c][0]==1:
                r+=shift
                break
           shift+=1
    if img[r][c][0]!=1:
        cnt+=1
    img[r][c][0]=i2a
    img[r][c][1]=i2b
    img[r][c][2]=i2c

    r=int(Array[i][29])-1
    c=int(Array[i][30])-1
    if img[r][c][0]!=1:
         shift=1
         while(1):
           if c-shift>=0 and img[r][c-shift][0]==1:
                c-=shift
                break
           elif r-shift>=0 and img[r-shift][c][0]==1:
                r-=shift
                break
           elif c+shift<10 and img[r][c+shift][0]==1:
                c-=shift
                break
           elif r+shift<10 and img[r+shift][c][0]==1:
                r+=shift
                break
           shift+=1
    if img[r][c][0]!=1:
        cnt+=1
    img[r][c][0]=i3a
    img[r][c][1]=i3b
    img[r][c][2]=i3c

    r=int(Array[i][27])-1
    c=int(Array[i][28])-1
    if img[r][c][0]!=1:
            shift=1
            while(1):
               if c-shift>=0 and img[r][c-shift][0]==1:
                c-=shift
                break
               elif r-shift>=0 and img[r-shift][c][0]==1:
                r-=shift
                break
               elif c+shift<10 and img[r][c+shift][0]==1:
                c-=shift
                break
               elif r+shift<10 and img[r+shift][c][0]==1:
                r+=shift
                break
               shift+=1
    if img[r][c][0]!=1:
         cnt+=1
    img[r][c][0]=i4a
    img[r][c][1]=i4b
    img[r][c][2]=i4c

    r=int(Array[i][25])-1
    c=int(Array[i][26])-1
    if img[r][c][0]!=1:
        shift=1
        while(1):
           if c-shift>=0 and img[r][c-shift][0]==1:
            c-=shift
            break
           elif r-shift>=0 and img[r-shift][c][0]==1:
            r-=shift
            break
           elif c+shift<10 and img[r][c+shift][0]==1:
            c-=shift
            break
           elif r+shift<10 and img[r+shift][c][0]==1:
            r+=shift
            break
           shift+=1
    if img[r][c][0]!=1:
        cnt+=1
    img[r][c][0]=i5a
    img[r][c][1]=i5b
    img[r][c][2]=i5c

    r=int(Array[i][23])-1
    c=int(Array[i][24])-1
    if img[r][c][0]!=1:
        shift=1
        while(1):
          if c-shift>=0 and img[r][c-shift][0]==1:
            c-=shift
            break
          elif r-shift>=0 and img[r-shift][c][0]==1:
            r-=shift
            break
          elif c+shift<10 and img[r][c+shift][0]==1:
            c-=shift
            break
          elif r+shift<10 and img[r+shift][c][0]==1:
            r+=shift
            break
          shift+=1
    if img[r][c][0]!=1:
        cnt+=1
    img[r][c][0]=i6a
    img[r][c][1]=i6b
    img[r][c][2]=i6c

    r=int(Array[i][21])-1
    c=int(Array[i][22])-1
    if img[r][c][0]!=1:
        shift=1
        while(1):
          if c-shift>=0 and img[r][c-shift][0]==1:
            c-=shift
            break
          elif r-shift>=0 and img[r-shift][c][0]==1:
            r-=shift
            break
          elif c+shift<10 and img[r][c+shift][0]==1:
            c-=shift
            break
          elif r+shift<10 and img[r+shift][c][0]==1:
            r+=shift
            break
          shift+=1
    if img[r][c][0]!=1:
       cnt+=1
    img[r][c][0]=i7a
    img[r][c][1]=i7b
    img[r][c][2]=i7c

    r=int(Array[i][19])-1
    c=int(Array[i][20])-1
    if img[r][c][0]!=1:
        shift=1
        while(1):
          if c-shift>=0 and img[r][c-shift][0]==1:
            c-=shift
            break
          elif r-shift>=0 and img[r-shift][c][0]==1:
            r-=shift
            break
          elif c+shift<10 and img[r][c+shift][0]==1:
            c-=shift
            break
          elif r+shift<10 and img[r+shift][c][0]==1:
            r+=shift
            break
          shift+=1
    if img[r][c][0]!=1:
       cnt+=1
    img[r][c][0]=i8a
    img[r][c][1]=i8b
    img[r][c][2]=i8c

    r=int(Array[i][17])-1
    c=int(Array[i][18])-1
    if img[r][c][0]!=1:
        shift=1
        while(1):
          if c-shift>=0 and img[r][c-shift][0]==1:
            c-=shift
            break
          elif r-shift>=0 and img[r-shift][c][0]==1:
            r-=shift
            break
          elif c+shift<10 and img[r][c+shift][0]==1:
            c-=shift
            break
          elif r+shift<10 and img[r+shift][c][0]==1:
            r+=shift
            break
          shift+=1
    if img[r][c][0]!=1:
       cnt+=1
    img[r][c][0]=i9a
    img[r][c][1]=i9b
    img[r][c][2]=i9c  

    r=int(Array[i][15])-1
    c=int(Array[i][16])-1
    if img[r][c][0]!=1:
        shift=1
        while(1):
           if c-shift>=0 and img[r][c-shift][0]==1:
            c-=shift
            break
           elif r-shift>=0 and img[r-shift][c][0]==1:
                r-=shift
                break
           elif c+shift<10 and img[r][c+shift][0]==1:
            c-=shift
            break
           elif r+shift<10 and img[r+shift][c][0]==1:
            r+=shift
            break
           shift+=1
    if img[r][c][0]!=1:
       cnt+=1   
    img[r][c][0]=i10a
    img[r][c][1]=i10b
    img[r][c][2]=i10c

    r=int(Array[i][13])-1
    c=int(Array[i][14])-1
    if img[r][c][0]!=1:
        shift=1
        while(1):
          if c-shift>=0 and img[r][c-shift][0]==1:
                c-=shift
                break
          elif r-shift>=0 and img[r-shift][c][0]==1:
                r-=shift
                break
          elif c+shift<10 and img[r][c+shift][0]==1:
            c-=shift
            break
          elif r+shift<10 and img[r+shift][c][0]==1:
            r+=shift
            break
          shift+=1
    if img[r][c][0]!=1:
       cnt+=1    
    img[r][c][0]=i11a
    img[r][c][1]=i11b
    img[r][c][2]=i11c

    r=int(Array[i][11])-1
    c=int(Array[i][12])-1
    if img[r][c][0]!=1:
        shift=1
        while(1):
          if c-shift>=0 and img[r][c-shift][0]==1:
            c-=shift
            break
          elif r-shift>=0 and img[r-shift][c][0]==1:
            r-=shift
            break
          elif c+shift<10 and img[r][c+shift][0]==1:
            c-=shift
            break
          elif r+shift<10 and img[r+shift][c][0]==1:
            r+=shift
            break
          shift+=1
    if img[r][c][0]!=1:
       cnt+=1
    img[r][c][0]=i12a
    img[r][c][1]=i12b
    img[r][c][2]=i12c

    r=int(Array[i][9])-1
    c=int(Array[i][10])-1
    if img[r][c][0]!=1:
        shift=1
        while(1):
          if c-shift>=0 and img[r][c-shift][0]==1:
                c-=shift
                break
          elif r-shift>=0 and img[r-shift][c][0]==1:
                r-=shift
                break
          elif c+shift<10 and img[r][c+shift][0]==1:
                c-=shift
                break
          elif r+shift<10 and img[r+shift][c][0]==1:
                r+=shift
                break
          shift+=1
    if img[r][c][0]!=1:
       cnt+=1
    img[r][c][0]=i13a
    img[r][c][1]=i13b
    img[r][c][2]=i13c

    r=int(Array[i][7])-1
    c=int(Array[i][8])-1
    if img[r][c][0]!=1:
        shift=1
        while(1):
          if c-shift>=0 and img[r][c-shift][0]==1:
                c-=shift
                break
          elif r-shift>=0 and img[r-shift][c][0]==1:
                r-=shift
                break
          elif c+shift<10 and img[r][c+shift][0]==1:
                c-=shift
                break
          elif r+shift<10 and img[r+shift][c][0]==1:
                r+=shift
                break
          shift+=1
    if img[r][c][0]!=1:
       cnt+=1
    img[r][c][0]=i14a
    img[r][c][1]=i14b
    img[r][c][2]=i14c

    r=int(Array[i][5])-1
    c=int(Array[i][6])-1
    if img[r][c][0]!=1:
        shift=1
        while(1):
          if c-shift>=0 and img[r][c-shift][0]==1:
                c-=shift
                break
          elif r-shift>=0 and img[r-shift][c][0]==1:
                r-=shift
                break
          elif c+shift<10 and img[r][c+shift][0]==1:
                c-=shift
                break
          elif r+shift<10 and img[r+shift][c][0]==1:
                r+=shift
                break
          shift+=1
    if img[r][c][0]!=1:
       cnt+=1
    img[r][c][0]=i15a
    img[r][c][1]=i15b
    img[r][c][2]=i15c 

    r=int(Array[i][3])-1
    c=int(Array[i][4])-1
    if img[r][c][0]!=1:
        shift=1
        while(1):
           if c-shift>=0 and img[r][c-shift][0]==1:
                c-=shift
                break
           elif r-shift>=0 and img[r-shift][c][0]==1:
                r-=shift
                break
           elif c+shift<10 and img[r][c+shift][0]==1:
                c-=shift
                break
           elif r+shift<10 and img[r+shift][c][0]==1:
                r+=shift
                break
           shift+=1
    if img[r][c][0]!=1:
       cnt+=1
    img[r][c][0]=i16a
    img[r][c][1]=i16b
    img[r][c][2]=i16c

    r=int(Array[i][1])-1
    c=int(Array[i][2])-1
    if img[r][c][0]!=1:
        shift=1
        while(1):
          if c-shift>=0 and img[r][c-shift][0]==1:
                c-=shift
                break
          elif r-shift>=0 and img[r-shift][c][0]==1:
                r-=shift
                break
          elif c+shift<10 and img[r][c+shift][0]==1:
                c-=shift
                break
          elif r+shift<10 and img[r+shift][c][0]==1:
                r+=shift
                break
          shift+=1
    if img[r][c][0]!=1:
       cnt+=1
    img[r][c][0]=i17a
    img[r][c][1]=i17b
    img[r][c][2]=i17c

    #img=img*255
    img2=np.ones(shape=(w,w,3))
    scale=w/10
    for row in range(w):
       for col in range(w):
        for d in range(3):
          img2[row][col][d]=img[int(row/scale)][int(col/scale)][d]


    imglist.append((img2,Array[i][0],Array[i][36]))

 X=[]
 y=[]
 for i in imglist:
     X.append(i[0])
     y.append(i[2])

 for i in range(len(y)):
     if y[i]=='g':
        y[i]=1
     elif y[i]=='b':
        y[i]=0
 X=np.asarray(X)
 y=np.asarray(y)
    
 print('Length of X:', len(X))
 print('Length of y:', len(y))
 return X,y
    


# In[4]:


X,y=getXy(0.99,0.35,0.26,0.98,0.99,0.26,0.33,0.99,0.26,0.27,0.98,0.87,0.27,0.44,0.98,0.71,0.27,0.98,0.98,0.27,0.84,0.92,0.47,0.33,0.88,0.91,0.34,0.35,0.91,0.34,0.35,0.87,0.91,0.35,0.39,0.90,0.80,0.35,0.90,0.86,0.48,0.39,0.77,0.86,0.39,0.39,0.86,0.45,0.61,0.39,0.86)


# In[5]:


X.shape


# In[6]:


y.shape


# In[7]:


import random
import gc 
import cv2
import numpy as np
import pandas as pd

import matplotlib.pyplot as plt
from matplotlib import ticker
import seaborn as sns
get_ipython().run_line_magic('matplotlib', 'inline')

plt.figure(figsize=(20,10))
columns = 5
for i in range(columns):
    print(Array[i])
    plt.subplot(5 / columns + 1, columns, i + 1)
    plt.imshow(X[i])


# In[8]:


sns.countplot(y)
plt.title('Labels g and b')


# In[9]:


from sklearn.model_selection import KFold
from tensorflow.keras import layers
from tensorflow.keras import models
#from tensorflow.keras.applications import InceptionResNetV2
model = models.Sequential()
model.add(layers.Conv2D(64, kernel_size=(2,2), input_shape=(2*w,2*w,3),strides=(2,2),activation='relu'))
model.add(layers.Conv2D(64, kernel_size=(2,2), strides=(2,2),activation='relu'))
model.add(layers.MaxPooling2D(pool_size=(2, 2)))
model.add(layers.Dropout(0.4))
model.add(layers.Conv2D(128, kernel_size=(2,2),strides=(2,2),activation='relu'))
model.add(layers.Conv2D(128, kernel_size=(2,2),strides=(2,2),activation='relu'))
model.add(layers.MaxPooling2D(pool_size=(2, 2)))
model.add(layers.Dropout(0.4))
model.add(layers.Flatten())
model.add(layers.Dense(256,activation='relu'))
model.add(layers.Dropout(0.4))
model.add(layers.Dense(1,activation='sigmoid'))
model.summary()

from tensorflow.keras import optimizers
model.compile(loss='binary_crossentropy', optimizer=optimizers.RMSprop(lr=1.5e-4), metrics=['acc'])


# In[ ]:


from tensorflow.keras.preprocessing.image import ImageDataGenerator
from tensorflow.keras.preprocessing.image import img_to_array, load_img
train_datagen = ImageDataGenerator(rescale=1./1.)
val_datagen = ImageDataGenerator(rescale=1./1.)
batch_size = 32
import random

weights=[]

from sklearn.model_selection import StratifiedKFold
kf = StratifiedKFold(n_splits=10,random_state=None)
Wsave = model.get_weights()
list_of_hlist=[]
list_of_val_acc=[]
list_of_intensity=[]
#list_of_mean_val_acc=[]

for iters in range(5):
  historylist=[]
  val_accuracies=[]
  #test_accuracies=[]
  #prev=0
  l=[random.uniform(0,1) for i in range(51)]
  X,y=getXy(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14],l[15],l[16],l[17],l[18],l[19],l[20],l[21],l[22],l[23],l[24],l[25],l[26],l[27],l[28],l[29],l[30],l[31],l[32],l[33],l[34],l[35],l[36],l[37],l[38],l[39],l[40],l[41],l[42],l[43],l[44],l[45],l[46],l[47],l[48],l[49],l[50])
  for train_index, val_index in kf.split(X,y):
      #print((test_index).shape)
      model.set_weights(Wsave)
      X_train_tmp,X_val_tmp=X[train_index],X[val_index]
      y_train,y_val=y[train_index],y[val_index]
      idxg=[]
      idxb=[]
      for i in range(y_train.shape[0]):
        #print(y_train[i],y_train[i]==1)
        if y_train[i]=='g' or y_train[i]==1:
          #print("g")
          idxg.append(i)
        else:
          idxb.append(i)
      print(len(idxg),len(idxb))
      X_g=X_train_tmp[idxg]
      print("iter=",iters)
      X_b=X_train_tmp[idxb]
      #print("bmean done")
      g_mean=X_g.mean(axis=0)
      b_mean=X_b.mean(axis=0)
      X_train=[]
      X_val=[]

      for imgidx in range(X_train_tmp.shape[0]):
        g_mean_cop=np.ones(shape=(w,w,3))
        b_mean_cop=np.ones(shape=(w,w,3))
        g_mean_cop[:][:][:]=g_mean[:][:][:]
        b_mean_cop[:][:][:]=b_mean[:][:][:]
        t=np.where(X_train_tmp[imgidx]!=1)
        for i in range(t[0].shape[0]):
          g_mean_cop[t[0][i]][t[1][i]][0]=(X_train_tmp[imgidx][t[0][i]][t[1][i]][0]+X_train_tmp[imgidx][t[0][i]][t[1][i]][1]+X_train_tmp[imgidx][t[0][i]][t[1][i]][2])/3
          g_mean_cop[t[0][i]][t[1][i]][1]=(X_train_tmp[imgidx][t[0][i]][t[1][i]][0]+X_train_tmp[imgidx][t[0][i]][t[1][i]][1]+X_train_tmp[imgidx][t[0][i]][t[1][i]][2])/3
          g_mean_cop[t[0][i]][t[1][i]][2]=(X_train_tmp[imgidx][t[0][i]][t[1][i]][0]+X_train_tmp[imgidx][t[0][i]][t[1][i]][1]+X_train_tmp[imgidx][t[0][i]][t[1][i]][2])/3
          b_mean_cop[t[0][i]][t[1][i]][0]=(X_train_tmp[imgidx][t[0][i]][t[1][i]][0]+X_train_tmp[imgidx][t[0][i]][t[1][i]][1]+X_train_tmp[imgidx][t[0][i]][t[1][i]][2])/3
          b_mean_cop[t[0][i]][t[1][i]][1]=(X_train_tmp[imgidx][t[0][i]][t[1][i]][0]+X_train_tmp[imgidx][t[0][i]][t[1][i]][1]+X_train_tmp[imgidx][t[0][i]][t[1][i]][2])/3
          b_mean_cop[t[0][i]][t[1][i]][2]=(X_train_tmp[imgidx][t[0][i]][t[1][i]][0]+X_train_tmp[imgidx][t[0][i]][t[1][i]][1]+X_train_tmp[imgidx][t[0][i]][t[1][i]][2])/3
          
        st=np.hstack((g_mean_cop,b_mean_cop))
        st=np.vstack((np.ones(shape=(int(w/2),2*w,3)),st))
        st=np.vstack((st,np.ones(shape=(int(w/2),2*w,3))))
        X_train.append(st);


      for imgidx in range(X_val_tmp.shape[0]):
        g_mean_cop=np.ones(shape=(w,w,3))
        b_mean_cop=np.ones(shape=(w,w,3))
        g_mean_cop[:][:][:]=g_mean[:][:][:]
        b_mean_cop[:][:][:]=b_mean[:][:][:]
        t=np.where(X_val_tmp[imgidx]!=1)
        for i in range(t[0].shape[0]):
#           M_mean_cop[t[0][i]][t[1][i]][0]+=X_val_tmp[imgidx][t[0][i]][t[1][i]][0]
#           M_mean_cop[t[0][i]][t[1][i]][1]+=X_val_tmp[imgidx][t[0][i]][t[1][i]][1]
#           M_mean_cop[t[0][i]][t[1][i]][2]+=X_val_tmp[imgidx][t[0][i]][t[1][i]][2]
#           B_mean_cop[t[0][i]][t[1][i]][0]+=X_val_tmp[imgidx][t[0][i]][t[1][i]][0]
#           B_mean_cop[t[0][i]][t[1][i]][1]+=X_val_tmp[imgidx][t[0][i]][t[1][i]][1]
#           B_mean_cop[t[0][i]][t[1][i]][2]+=X_val_tmp[imgidx][t[0][i]][t[1][i]][2]
          
          g_mean_cop[t[0][i]][t[1][i]][0]=(X_val_tmp[imgidx][t[0][i]][t[1][i]][0]+X_val_tmp[imgidx][t[0][i]][t[1][i]][1]+X_val_tmp[imgidx][t[0][i]][t[1][i]][2])/3
          g_mean_cop[t[0][i]][t[1][i]][1]=(X_val_tmp[imgidx][t[0][i]][t[1][i]][0]+X_val_tmp[imgidx][t[0][i]][t[1][i]][1]+X_val_tmp[imgidx][t[0][i]][t[1][i]][2])/3
          g_mean_cop[t[0][i]][t[1][i]][2]=(X_val_tmp[imgidx][t[0][i]][t[1][i]][0]+X_val_tmp[imgidx][t[0][i]][t[1][i]][1]+X_val_tmp[imgidx][t[0][i]][t[1][i]][2])/3
          b_mean_cop[t[0][i]][t[1][i]][0]=(X_val_tmp[imgidx][t[0][i]][t[1][i]][0]+X_val_tmp[imgidx][t[0][i]][t[1][i]][1]+X_val_tmp[imgidx][t[0][i]][t[1][i]][2])/3
          b_mean_cop[t[0][i]][t[1][i]][1]=(X_val_tmp[imgidx][t[0][i]][t[1][i]][0]+X_val_tmp[imgidx][t[0][i]][t[1][i]][1]+X_val_tmp[imgidx][t[0][i]][t[1][i]][2])/3
          b_mean_cop[t[0][i]][t[1][i]][2]=(X_val_tmp[imgidx][t[0][i]][t[1][i]][0]+X_val_tmp[imgidx][t[0][i]][t[1][i]][1]+X_val_tmp[imgidx][t[0][i]][t[1][i]][2])/3
          
        st=np.hstack((g_mean_cop,b_mean_cop))
        st=np.vstack((np.ones(shape=(int(w/2),2*w,3)),st))
        st=np.vstack((st,np.ones(shape=(int(w/2),2*w,3))))
        X_val.append(st);
      X_train=np.asarray(X_train)
      X_val=np.asarray(X_val)
  #     print(np.array_equal(prev,train_index))
  #     prev=[]
  #     for i in train_index:
  #       prev.append(i)
  #     print("noob",np.array_equal(prev,train_index))
      print("----------------------"+str(train_index[25])+"-----------------------------------------")
      plt.figure()
      plt.imshow(X_train_tmp[6])
      plt.show()
      plt.figure()
      plt.imshow(g_mean)
      plt.show()
      plt.figure()
      plt.imshow(b_mean)
      plt.show()
      plt.figure()
      plt.imshow(X_train[6])
      plt.show()
      
      ntrain=X_train.shape[0]
      nval=X_val.shape[0]
      train_generator = train_datagen.flow(X_train, y_train,batch_size=batch_size)
      val_generator = val_datagen.flow(X_val, y_val, batch_size=batch_size)
      historylist.append(model.fit_generator(train_generator,
                                steps_per_epoch=ntrain // batch_size,
                                epochs=90,
                                validation_data=val_generator,
                                validation_steps=nval // batch_size))
      val_score = model.evaluate(X_val,y_val,verbose=0)
      val_accuracies.append(val_score[1]*100)
      #test_score = model.evaluate(X_test,y_test,verbose=0)
      #test_accuracies.append(test_score[1]*100)
      weights.append(model.get_weights())
  print(np.asarray(val_accuracies).mean())
  print(val_accuracies)
  print(l)
  print("\n")
  print("###################################################################################################################################################")
  list_of_hlist.append(historylist)
  list_of_val_acc.append(val_accuracies)
  list_of_intensity.append(l)


# In[ ]:


import matplotlib.pyplot as plt
iternum=1
for historyl in list_of_hlist:
  for history in historyl:
      #get the details form the history object
      acc = history.history['acc']
      val_acc = history.history['val_acc']
      loss = history.history['loss']
      val_loss = history.history['val_loss']

      epochs = range(1, len(acc) + 1)

      #Train and validation accuracy
      plt.plot(epochs, acc, 'b', label='Training accurarcy')
      plt.plot(epochs, val_acc, 'r', label='Validation accurarcy')
      plt.title('Training and Validation accurarcy')
      plt.legend()

      plt.figure()
      #Train and validation loss
      plt.plot(epochs, loss, 'b', label='Training loss')
      plt.plot(epochs, val_loss, 'r', label='Validation loss')
      plt.title('Training and Validation loss'+str(iternum))
      plt.legend()

      plt.show()
  iternum+=1


# In[ ]:


mean_accies=[]
for val_accies in list_of_val_acc:
  print(val_accies)
  print("Mean Validation Accuracy::",np.asarray(val_accies).mean())
  print("\n")
  mean_accies.append(np.asarray(val_accies).mean())


# In[ ]:


for ac,intensities in zip(mean_accies,list_of_intensity):
  print(intensities,ac)


# In[ ]:


print(mean_accies)


# In[ ]:




