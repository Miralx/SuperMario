import os

path = r"C:\Users\PC\Documents\AGK\template\Mario\target_img"
images = [] # 先存储所有的图像的名称
file_name = os.listdir(path)
for f in file_name:
    images.append(f)
images.sort()

# mario 图片需要交换一下位置
tmp = []
tmp.append(images[0])
for i in range(2, len(images)):
    tmp.append(images[i])
tmp.append(images[1])
images = tmp
images.sort()

import numpy as np
from PIL import Image

img_array = ''
img = ''
for i, v in enumerate(images):
    if i == 0:
        img = Image.open(path+'/'+v)  # 打开图片
        img_array = np.array(img)  # 转化为np array对象
    if i > 0:
        img_array2 = np.array(Image.open(path+'/'+v))
        img_array = np.concatenate((img_array, img_array2), axis=1)  # 横向拼接
        # img_array = np.concatenate((img_array, img_array2), axis=0)  # 纵向拼接
        img = Image.fromarray(img_array)

# 保存图片
img.save(r"C:\Users\PC\Documents\AGK\template\Mario\test.png")