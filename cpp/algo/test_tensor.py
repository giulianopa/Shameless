#
# Copyright (C) 2019 Giuliano Pasqualotto (github.com/giulianopa)
# This code is licensed under MIT license (see LICENSE.txt for details)
#
import numpy as np
from scipy import ndimage

a = np.array([[[29, 54,  3], [54,  7, 49], [47, 59, 28], [23,  6, 47]],
              [[41, 20,  9], [40, 52, 19], [24, 50, 56], [45, 38,  8]],
              [[30,  3, 15], [48, 60, 58], [24, 30, 52], [29, 25,  0]],
              [[17, 12,  0], [45, 37,  6], [33, 17, 28], [45, 60, 19]],
              [[ 5, 23, 11], [ 0, 10, 49], [ 9, 40, 54], [26, 27, 55]]])

# This kernel obviously does NOT compute the mean.
# It could, replacing 1 with 1/7. But results would be incorrect at the border
# When using constant mode and cval=0.0.
# Also, convolve == correlate, being the kernel symmetric.
k = np.array([[[0, 0, 0], [0, 1, 0], [0, 0, 0]],
              [[0, 1, 0], [1, 1, 1], [0, 1, 0]],
              [[0, 0, 0], [0, 1, 0], [0, 0, 0]]])

print(a.shape)
print(a)
print(ndimage.correlate(a, k, mode='constant', cval=0.0))
