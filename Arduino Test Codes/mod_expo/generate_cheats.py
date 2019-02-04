#!/usr/bin/env python3

from math import factorial
import matplotlib.pyplot as pp
import numpy as np
import scipy.io
import sys

def otsu(gray):
	pixel_number = len(gray)
	mean_weigth = 1.0/pixel_number
	his, bins = np.histogram(gray, np.array(range(min(gray), max(gray))))
	final_thresh = -1
	final_value = -1
	for t in bins[1:-1]:
		Wb = np.sum(his[:t]) * mean_weigth
		Wf = np.sum(his[t:]) * mean_weigth
		mub = np.mean(his[:t])
		muf = np.mean(his[t:])
		value = Wb * Wf * (mub - muf) ** 2
		print("Wb", Wb, "Wf", Wf)
		print("t", t, "value", value)
		if value > final_value:
			final_thresh = t
			final_value = value
	final_img = gray.copy()
	print(final_thresh)
	return final_thresh

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
	h = {
		'1466015503703t.mat' : [-0.839, 50, 400, 3237, 24629],
		'1501199875790165t.mat' : [-0.839, 50, 400, 2317, 25472],
		'22906492249t.mat' : [-0.839, 50, 400, 5731, 22089],
		'23456248059221t.mat' : [-0.839, 50, 400, 4550, 25089],
		'366503875927t.mat' : [-0.839, 50, 500, 3237, 25347],
		'375299968947541t.mat' : [-0.839, 50, 400, 568, 27555],
		'5864062014807t.mat' : [-0.839, 50, 400, 3236, 26021],
		'93824992236885t.mat' : [-0.839, 50, 500, 1181, 27214],
		'91625968981t.mat' : [-0.838, 50, 500, 3237, 23437],
		'4436988421939.mat' : [-0.839, 50, 500, 1531, 26891],
		'10097080277531.mat' : [-0.839, 50, 500, 1356, 26881],
		'107763315118965647.mat' : [-0.839, 50, 400, 1312, 25880],
		'18702745650660789.mat' : [-0.839, 50, 400, 2362, 26386],
		'2324818243004987.mat' : [-0.839, 50, 400, 131, 27835],
		'24967491312354467.mat' : [-0.839, 50, 400, 568, 27740],
		'26499063470653493.mat' : [-0.839, 50, 400, 2012, 26866],
		'34277.mat' : [-0.839, 50, 1200, 156, 24810],
		'384945969131490773.mat' : [-0.839, 50, 400, 1093, 27489],
		'422890635578730455.mat' : [-0.839, 50, 400, 1443, 27324],
		'51280870137213061.mat' : [-0.839, 50, 400, 2012, 26188],
		'55468967563257125.mat' : [-0.839, 50, 400, 2012, 26906],
		'70994704783375.mat' : [-0.839, 50, 500, 218, 27853],
		'850022758091.mat' : [-0.839, 50, 500, 43, 27946],
		'8948760622563097.mat' : [-0.839, 50, 500, 1312, 26621],
		'992493553292811149.mat' : [-0.839, 50, 400, 1531, 27181],
		'65599.mat' : [-0.811, 50, 1200, 157, 24755]
	}
	# for i in h.keys():
	# 	print(type(i))
	# raise SystemExit
	for file in h.keys():
		file = sys.argv[1]
		traces = scipy.io.loadmat(file)
		t = traces['Trace_1'][:, 0]
		b = traces['Trace_1'][:, 1]
		s = traces['Trace_2'][:, 1]
		p = traces['Trace_3'][:, 1]
		f = savitzky_golay(p, 35, 10)
		j1 = 0
		for i in range(50, len(t) - 51):
			m = np.median(f[i : i + 50])
			if m < -0.9:
				j1 = i
				# print(i, m)
		j2 = 0
		for i in range(len(t) - 51, 50, -1):
			m = np.median(f[i : i + 50])
			# print(m)
			if m < 0:
				j2 = i
				# print(i, m)
		last = j1 + np.random.randint(50)
		first = j2 - np.random.randint(50)
		peak = np.amax(f) - 0.03
		limit = 50
		threshold = 500
		# peak, limit, threshold, first, last = scipy.io.loadmat('dpa.mat')[file][0]
		start = first
		signatures = []
		for i in range(int(first), int(last + 1)):
			if f[i] > peak:
				if i - start < limit:
					continue
				signatures.append(f[int(start) : i])
				start = i
		if i - start > limit:
			signatures.append(f[start : i])
		bits = np.array([len(i) for i in signatures])
		# bits[bits <= threshold] = 0
		# bits[bits > threshold] = 1
		bits = bits[:: -1]
		# print(signatures)
		threshold = otsu(bits)
		h[file] = [peak, limit, threshold, first, last]
		print(h[file])
	scipy.io.savemat('dpa2.mat', h)
