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

learn_A = scipy.io.loadmat('input_00ff00ff00ff00ff.mat')
times_A = learn_A['Trace_1'][:, 0]
cycle_A = learn_A['Trace_2'][:, 1]
power_A = learn_A['Trace_1'][:, 1]
flips_A = learn_A['Trace_3'][:, 1]

biggest_flips_A = np.amax(flips_A)
average_flips_A = np.average(flips_A)
tiniest_flips_A = np.amin(flips_A)
for count in range(len(flips_A)):
	if flips_A[count] < average_flips_A:
		flips_A[count] = tiniest_flips_A
	else:
		flips_A[count] = biggest_flips_A

biggest_cycle_A = np.amax(cycle_A)
average_cycle_A = np.average(cycle_A)
tiniest_cycle_A = np.amin(cycle_A)
for count in range(len(cycle_A)):
	if cycle_A[count] < -3:
		cycle_A[count] = tiniest_cycle_A
	else:
		cycle_A[count] = biggest_cycle_A

hash_A = []
bit_signature = []
start = 0

for count in range(len(times_A)):
	if start == 0 and cycle_A[count] == biggest_cycle_A and flips_A[count] == biggest_flips_A:
		start = 1
	if start == 1:
		if cycle_A[count] == tiniest_cycle_A:
			hash_A.append(bit_signature)
			break
		bit_signature.append(power_A[count])
		if flips_A[count] != flips_A[count + 1]:
			hash_A.append(bit_signature)
			bit_signature = []


learn_5 = scipy.io.loadmat('input_ff00ff00ff00ff00.mat')
times_5 = learn_5['Trace_1'][:, 0]
cycle_5 = learn_5['Trace_2'][:, 1]
power_5 = learn_5['Trace_1'][:, 1]
flips_5 = learn_5['Trace_3'][:, 1]

biggest_flips_5 = np.amax(flips_5)
average_flips_5 = np.average(flips_5)
tiniest_flips_5 = np.amin(flips_5)
for count in range(len(flips_5)):
	if flips_5[count] < average_flips_5:
		flips_5[count] = tiniest_flips_5
	else:
		flips_5[count] = biggest_flips_5

biggest_cycle_5 = np.amax(cycle_5)
average_cycle_5 = np.average(cycle_5)
tiniest_cycle_5 = np.amin(cycle_5)
for count in range(len(cycle_5)):
	if cycle_5[count] < -3:
		cycle_5[count] = tiniest_cycle_5
	else:
		cycle_5[count] = biggest_cycle_5

hash_5 = []
bit_signature = []
start = 0

for count in range(len(times_5)):
	if start == 0 and cycle_5[count] == biggest_cycle_5 and flips_5[count] == biggest_flips_5:
		start = 1
	if start == 1:
		if cycle_5[count] == tiniest_cycle_5:
			hash_5.append(bit_signature)
			break
		bit_signature.append(power_5[count])
		if flips_5[count] != flips_5[count + 1]:
			hash_5.append(bit_signature)
			bit_signature = []

trace_0 = []
trace_1 = []
for count in range(56):
	if count % 2 == 0:
		trace_0.append(hash_5[count])
		trace_1.append(hash_A[count])
	else:
		trace_0.append(hash_A[count])
		trace_1.append(hash_5[count])


tests_R = scipy.io.loadmat('input_ff00ff00ff00ff00.mat')
times_R = tests_R['Trace_1'][:, 0]
cycle_R = tests_R['Trace_2'][:, 1]
power_R = tests_R['Trace_1'][:, 1]
flips_R = tests_R['Trace_3'][:, 1]

'''
correlator_0 = np.array(trace_0[0])
correlator_1 = np.array(trace_1[0])
length_0 = len(correlator_0)
length_1 = len(correlator_1)
result_0 = []
result_1 = []
for count in range(250):
	correlated_0 = np.array(power_R[count : count + length_0])
	result_0.append(np.linalg.norm(correlated_0 - correlator_0))
	correlated_1 = np.array(power_R[count : count + length_1])
	result_1.append(np.linalg.norm(correlated_1 - correlator_1))
	print count, result_0[count], result_1[count]
print 'locating first 0', result_0.index(min(result_0)), min(result_0)
print 'locating first 1', result_1.index(min(result_1)), min(result_1)
'''

powa = savitzky_golay(power_R * 5, 17, 5) + 6

'''
biggest_power_R = np.amax(power_R)
tiniest_power_R = np.amin(power_R)
threshold = tiniest_power_R + 0.45 * (biggest_power_R - tiniest_power_R)
value = 0
old = 0
for count in range(len(power_R)):
	if power_R[count] < threshold:
		if count - old > 10:
			value = 1 - value
			old = count
	power_R[count] = value
'''
biggest_powa = np.amax(powa)
tiniest_powa = np.amin(powa)
threshold = tiniest_powa + 0.45 * (biggest_powa - tiniest_powa)
value = 0
old = 0
powb = powa.copy()
for count in range(len(powa)):
	if powa[count] < threshold:
		if count - old > 10:
			value = 1 - value
			old = count
	powa[count] = value


pp.figure().canvas.set_window_title('Learning DES PC1 (1)')
pp.plot(times_A, power_A * float(sys.argv[1]) + float(sys.argv[2]), 'k-', label = 'power trace', linewidth = 0.8)
pp.plot(times_A, cycle_A, 'r-', label = 'encryption cycle marker', linewidth = 0.8)
pp.plot(times_A, flips_A, 'b-', label = 'bit operation marker', linewidth = 0.8)
pp.xlabel('time / s')
pp.ylabel('voltage / V')
pp.title(r'0x00FF00FF00FF00FF $\rightarrow$ 0xAAAAAAAAAAAAAAAA')
pp.legend()
pp.grid(True)
# pp.show()

pp.figure().canvas.set_window_title('Learning DES PC1 (2)')
pp.plot(times_5, power_5 * float(sys.argv[1]) + float(sys.argv[2]), 'k-', label = 'power trace', linewidth = 0.8)
pp.plot(times_5, cycle_5, 'r-', label = 'encryption cycle marker', linewidth = 0.8)
pp.plot(times_5, flips_5, 'b-', label = 'bit operation marker', linewidth = 0.8)
pp.xlabel('time / s')
pp.ylabel('voltage / V')
pp.title(r'0xFF00FF00FF00FF00 $\rightarrow$ 0x5555555555555555')
pp.legend()
pp.grid(True)
# pp.show()

pp.figure().canvas.set_window_title('Testing DES PC1')
pp.plot(times_R, 3 * power_R * float(sys.argv[1]) + float(sys.argv[2]) + 5.5, 'r-', label = 'power trace', linewidth = 0.8)
pp.plot(times_R, powb * float(sys.argv[1]) + float(sys.argv[2]) + 0.2, 'b-', label = 'processed power trace', linewidth = 0.8)
pp.plot(times_R, powa * float(sys.argv[1]) + float(sys.argv[2]), 'g-', label = 'binarized processed power trace', linewidth = 0.8)
pp.plot(times_R, cycle_R, 'r-', label = 'encryption cycle marker', linewidth = 0.8)
pp.plot(times_R, flips_R, 'b-', label = 'bit operation marker', linewidth = 0.8)
pp.xlabel('time / s')
pp.ylabel('voltage / V')
# pp.title(r'0x0123456789ABCDEF $\rightarrow$ 0xF0CCAA0AACCF00')
pp.legend()
# pp.yticks([])
pp.grid(True)
# pp.ylim([-3.2, 1])
# pp.xlim([])
pp.show()

file = scipy.io.loadmat('a1b zoom out 2018-10-30.mat')
t = file['Trace_1'][:, 0]
p = file['Trace_1'][:, 1]
pp.figure().canvas.set_window_title('DES Power Signature')
pp.plot(t, p, 'k-', label = 'power trace', linewidth = 0.8)
pp.xlabel('time / s')
pp.ylabel('voltage / V')
pp.title('DES Power Signature')
pp.legend()
pp.xlim([0.343, 0.352])
pp.ylim([-1.2, -0.1])
pp.grid(True)
pp.show()
