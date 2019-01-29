#!/usr/bin/env python

import scipy.io
import matplotlib.pyplot as pp
import numpy as np
import sys
from math import factorial

def savitzky_golay(y, window_size, order, deriv=0, rate=1):
    try:
        window_size = np.abs(np.int(window_size))
        order = np.abs(np.int(order))
    except ValueError, msg:
        raise ValueError("window_size and order have to be of type int")
    if window_size % 2 != 1 or window_size < 1:
        raise TypeError("window_size size must be a positive odd number")
    if window_size < order + 2:
        raise TypeError("window_size is too small for the polynomials order")
    order_range = range(order+1)
    half_window = (window_size -1) // 2
    # precompute coefficients
    b = np.mat([[k**i for i in order_range] for k in range(-half_window, half_window+1)])
    m = np.linalg.pinv(b).A[deriv] * rate**deriv * factorial(deriv)
    # pad the signal at the extremes with
    # values taken from the signal itself
    firstvals = y[0] - np.abs( y[1:half_window+1][::-1] - y[0] )
    lastvals = y[-1] + np.abs(y[-half_window-1:-1][::-1] - y[-1])
    y = np.concatenate((firstvals, y, lastvals))
    return np.convolve( m[::-1], y, mode='valid')

A = scipy.io.loadmat('4436988421939.mat')
#A = scipy.io.loadmat('34277.mat')
print(A)

power = A['Trace_3'][:, 1]
print(len(power))

spikes = [1.9 for i in range(len(power))]

powa = savitzky_golay(power * 5, 35, 10) + 6

x = 0
count = 0

i=0

while (i < len(powa)):

	if(powa[i] > 1.85):
		spikes[i] = 2.3
		i = i + 100
	
	i = i + 1


pp.figure().canvas.set_window_title('Power for RSA')
#pp.plot(times_A, power_A * float(sys.argv[1]) + float(sys.argv[2]), 'k-', label = 'power trace', linewidth = 0.8)
pp.plot(   powa,  'r-', label = 'filtered power trace', linewidth = 0.8)
pp.plot(spikes, 'b-', label = 'loop marker', linewidth = 0.8)
pp.xlabel('time / s')
pp.ylabel('voltage / V')
pp.legend()
pp.grid(True)
pp.show()

