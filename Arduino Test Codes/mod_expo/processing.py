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

def get_signatures(file, traces):
	t = traces['Trace_1'][:, 0]
	b = traces['Trace_1'][:, 1]
	s = traces['Trace_2'][:, 1]
	p = traces['Trace_3'][:, 1]
	f = savitzky_golay(p, 35, 10)
	# peak = maximum value - 0.22
	# limit = 50
	# threshold = Otsu's Thresholding on sequence lengths
	# first = point where moving median drops
	# last = point where moving median rises
	peak, limit, threshold, first, last = scipy.io.loadmat('dpa.mat')[file][0]
	# peak, limit, threshold, first, last = [-0.838, 50, 500, 3237, 23437]
	start = first
	signatures = []
	# for i in range(len(t)):
	# 	if i not in range(int(first), int(last + 1)):
	# 	# if b[i] < -3:
	# 		# print(i)
	# 		continue
	# 	if f[i] > peak:
	# 		if i - start < limit:
	# 			continue
	# 		signatures.append(f[start : i])
	# 		start = i
	# 		# print(i)
	# deriv = np.diff(b)
	# print(deriv[deriv > 1])
	i_prev = 0
	for i in range(len(t)):
		if b[i] < -3:
			if i - i_prev > 1000:
				print(i_prev + 1, i - 1)
			i_prev = i
			# print(i)
			continue
	for i in range(int(first), int(last + 1)):
		if f[i] > peak:
			if i - start < limit:
				continue
			signatures.append(f[int(start) : i])
			start = i
	if i - start > limit:
		signatures.append(f[start : i])
	bits = np.array([len(i) for i in signatures])
	bits[bits <= threshold] = 0
	bits[bits > threshold] = 1
	bits = bits[:: -1]
	d = ''
	for i in bits:
		d += str(i)
	print('0b{}'.format(d))
	print(int(d, 2))
	print(str(int(d, 2)) == file[: -5] or str(int(d, 2)) == file[: -4])
	# print(np.amax(f))

if __name__ == '__main__':
	try:
		file = sys.argv[1]
		traces = scipy.io.loadmat(file)
	except IndexError:
		raise SystemExit('Enter file name as argument.') # use 4436988421939.mat for report
	signatures = get_signatures(file, traces)
	time = traces['Trace_1'][:, 0]
	big_marker = traces['Trace_1'][:, 1]
	small_marker = traces['Trace_2'][:, 1]
	power = traces['Trace_3'][:, 1]
	filtered = savitzky_golay(power, 35, 10)
	start = 0
	threshold = 500
	signatures = []
	for i in range(len(time)):
		if big_marker[i] < -3:
			continue
		if filtered[i] > -0.83:
			if i - start < 50:
				continue
			signatures.append(i - start)
			# if i - start < threshold:
			# 	signatures.append('0')
			# else:
			# 	signatures.append('1')
			start = i
	# print(signatures)
	# signatures = signatures[1 :]
	# signatures = np.array(signatures)
	# print(signatures)
	# signatures[signatures < threshold] = 0
	# signatures[signatures >= threshold] = 1
	# print(signatures)
	# print('0b1' + ''.join(signatures[:: -1]))
	pp.figure().canvas.set_window_title('RSA on Arduino')
	pp.title('RSA Power Trace')
	pp.plot(power, 'b-', label = 'power trace', linewidth = 0.8)
	# pp.plot(filtered, 'r-', label = 'filtered power trace', linewidth = 0.8)
	pp.plot(small_marker, 'g-', label = 'cycle marker', linewidth = 0.8)
	pp.plot(big_marker, 'y-', label = 'operation marker', linewidth = 0.8)
	pp.xlabel('time / s')
	pp.ylabel('voltage / V')
	pp.legend()
	pp.grid(True, linewidth = 0.4)
	# pp.show()
