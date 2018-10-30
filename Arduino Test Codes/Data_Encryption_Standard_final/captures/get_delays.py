#!/usr/bin/env python

import scipy.io
import matplotlib.pyplot as pp
import numpy as np
import sys

# obtain the data of permutation of 0xAAAAAAAAAAAAAAAA
bits_AA = scipy.io.loadmat('0-F_2018-10-30.mat')

'''
	above dictionary variable contains two-dimensional arrays
	whose first array is the array of time instants
	(this is the same for all three	because they were all captured from a single screenshot)
	and second array is the array of the respective voltages
'''
timeA = bits_AA['Trace_1'][:, 0]
cycleA = bits_AA['Trace_1'][:, 1]
powerA = bits_AA['Trace_2'][:, 1]
flipsA = bits_AA['Trace_3'][:, 1]

'''
	first, some details on what this script is doing
	in Data Encryption Standard, there is a step called PC1
	in which the 64-bit encryption key is permuted and truncated to 56 bits
	to generate these 56 bits algorithmically, we first initialize a uint64_t to 0
	then we add 1 shifted by whatever the number of bits required to this number
	e.g. 41st bit of the key becomes 3rd bit in the permuted key
	if this 49th bit was 1, we add 0x8000000000000000 shifted by 2 to the permuted key
	if this 49th bit was 0, we do nothing, because the permuted key was initialized to 0
	there are 56 such operations
	the power consumed is more if 1 is encountered (because it shifts and adds)
	now, to determine, from a random power consumption graph, all of the 64 bits,
	we need a dictionary of power consumption
	i.e. how much power is consumed when 0 is encountered in each position
	and how much when 1 is encountered
	here, we toggle 'flips' each time a bit is operated on
	and the 56-bit permuted key generation begins when 'cycle' is high
'''

'''
	study the 'cycle' graph to get ones and zeros
	'delaysA[0]' stores the length of time (samples) required for permuting 0
	'delaysA[1]' stores the length of time (samples) required for permuting 1
'''
delaysA = np.zeros([2, 56])
start = 0
old = 0
counter = 0

for i in range(len(timeA)):

	# 56-bit generation begins only when 'cycle' is high
	if start == 0 and cycleA[i] > -2 and flipsA[i] > -2:
		start = 1

	# permuted key generation has started
	if start == 1:

		# stop if the cycle marker has fallen
		if cycleA[i] < -4:
			break

		'''
			if 'flips' suffers a large change, it means
			that the next bit is now being permuted
		'''
		if abs(old - flipsA[i]) > 2:
			counter += 1

			'''
				if 'counter' reaches 56, we have all the 56 bits we need
				next cycle of encryption will begin now, which is not needed
			'''
			if counter > 55:
				break

		'''
			we are operating on 0xAAAAAAAAAAAAAAAA
			so we know that the first bit is 1
			and there are alternating 0s and 1s
		'''
		delaysA[(counter + 1) % 2, counter] += 1
		old = flipsA[i]

# convert number of samples to number of seconds
delaysA_time = delaysA * (timeA[1] - timeA[0])

# same thing, but for 0x5555555555555555

bits_55 = scipy.io.loadmat('pattern_0x5.mat')

time5 = bits_55['Trace_1'][:, 0]
cycle5 = bits_55['Trace_1'][:, 1]
power5 = bits_55['Trace_2'][:, 1]
flips5 = bits_55['Trace_3'][:, 1]

delays5 = np.zeros([2, 56])
start = 0
old = 0
counter = 0

for i in range(len(time5)):
	if start == 0 and cycle5[i] > -2 and flips5[i] > -2:
		start = 1
	if start == 1:
		if cycle5[i] < -4:
			break
		if abs(old - flips5[i]) > 2:
			counter += 1
			if counter > 55:
				break
		delays5[counter % 2, counter] += 1
		old = flips5[i]

delays5_time = delays5 * (time5[1] - time5[0])

'''
	the time taken by each operation is simply the sum of these
	the answer will be in number of samples, but we need the time
	hence, the scaling factor
'''
# bit_samples = delays5 + delaysA
all_times = delaysA_time + delays5_time

# now try to get the data from a sample
bits_sample = scipy.io.loadmat('sample_1.mat')

time_sample = bits_sample['Trace_1'][:, 0]
power_sample = bits_sample['Trace_2'][:, 1]
cycle_sample = bits_sample['Trace_1'][:, 1]
flips_sample = bits_sample['Trace_3'][:, 1]

delays_sample = np.zeros(56)
start = 0
stop = 0
old = 0
counter = 0

for i in range(len(time_sample)):
	if start == 0 and cycle_sample[i] > -2 and flips_sample[i] > -2:
		start = 1
	if start == 1:
		if cycle_sample[i] < -4:
			break
		if abs(old - flips_sample[i]) > 2:
			counter += 1
		delays_sample[counter] += 1
		old = flips_sample[i]

permuted_subkey = 0
sample_time = delays_sample * (time_sample[1] - time_sample[0])
for i in range(56):
	permuted_subkey <<= 1
	if abs(sample_time[i] - all_times[0][i]) > abs(sample_time[i] - all_times[1][i]):
		permuted_subkey += 1
	else:
		permuted_subkey += 0

print bin(permuted_subkey)

# this takes care of plotting the graphs
pp.figure().canvas.set_window_title('Learning DES PC1 (1)')
pp.plot(timeA, powerA* float(sys.argv[1]) + float(sys.argv[2]), '#0F0F0F', label = 'power trace', linewidth = 0.8)
pp.plot(timeA, cycleA, 'r-', label = 'encryption cycle marker', linewidth = 0.8)
pp.plot(timeA, flipsA, 'b-', label = 'bit operation marker', linewidth = 0.8)
pp.xlabel('time / s')
pp.ylabel('voltage / V')
pp.title(r'0x00FF00FF00FF00FF $\rightarrow$ 0xAAAAAAAAAAAAAAAA')
pp.legend()
pp.grid(True)
pp.show()

pp.figure().canvas.set_window_title('Learning DES PC1 (2)')
pp.plot(time5, power5, 'r-', label = 'power trace', linewidth = 0.8)
pp.plot(time5, cycle5, 'g-', label = 'encryption cycle marker', linewidth = 0.8)
pp.plot(time5, flips5, 'b-', label = 'bit operation marker', linewidth = 0.8)
pp.xlabel('time / s')
pp.ylabel('voltage / V')
pp.title(r'0xFF00FF00FF00FF00 $\rightarrow$ 0x5555555555555555')
pp.legend()
pp.grid(True)
# pp.show()

pp.figure().canvas.set_window_title('Testing DES PC1')
pp.plot(time_sample, power_sample, 'r-', label = 'power trace', linewidth = 0.8)
pp.plot(time_sample, cycle_sample, 'g-', label = 'encryption cycle marker', linewidth = 0.8)
pp.plot(time_sample, flips_sample, 'b-', label = 'bit operation marker', linewidth = 0.8)
pp.xlabel('time / s')
pp.ylabel('voltage / V')
pp.title(r'0x0123456789ABCDEF $\rightarrow$ 0xF0CCAA0AACCF00')
pp.legend()
pp.grid(True)
#pp.show()
