#!/usr/bin/env python3

from math import factorial
import matplotlib.pyplot as pp
import numpy as np
import scipy.io
import sys

def savitzky_golay(y, window_size, order, deriv = 0, rate = 1):
	try:
		window_size = np.abs(np.int(window_size))
		order = np.abs(np.int(order))
	except ValueError:
		raise ValueError('window_size and order have to be of type int')
	if window_size % 2 != 1 or window_size < 1:
		raise TypeError('window_size size must be a positive odd number')
	if window_size < order + 2:
		raise TypeError('window_size is too small for the polynomials order')
	order_range = range(order + 1)
	half_window = (window_size -1) // 2
	b = np.mat([[k ** i for i in order_range] for k in range(-half_window, half_window + 1)])
	m = np.linalg.pinv(b).A[deriv] * rate ** deriv * factorial(deriv)
	firstvals = y[0] - np.abs(y[1 : half_window + 1][:: -1] - y[0])
	lastvals = y[-1] + np.abs(y[-half_window - 1 : -1][:: -1] - y[-1])
	y = np.concatenate((firstvals, y, lastvals))
	return np.convolve(m[:: -1], y, mode = 'valid')

if __name__ == '__main__':
	file = sys.argv[1]
	traces = scipy.io.loadmat(file)
	t = traces['Trace_1'][:, 0]
	b = traces['Trace_1'][:, 1]
	s = traces['Trace_2'][:, 1]
	p = traces['Trace_3'][:, 1]
	f = savitzky_golay(p, 35, 10)
	for i in range(50, len(t) - 51):
		m = np.median(f[i : i + 50])
		if m < -0.9:
			j1 = i
			# print(i, m)
	for i in range(len(t) - 51, 50):
		m = np.median(f[i : i + 50])
		if m < -0.9:
			j2 = i
			# print(i, m)
	last = j1 + 12
	first = j2 - 12
	peak = np.amax(f) - 0.22
	limit = 50
