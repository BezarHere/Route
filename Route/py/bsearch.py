

from random import randint
from typing import Iterable

VERBOSE = False

def bsearch(value, iter: Iterable) -> int:
	length = len(iter)
	start = 0
	win_ln = length

	iterations = 0
	while win_ln > 0 and iterations < length:
		iterations += 1
		index = start + (win_ln >> 1)
		# print(start, win_ln, index, (win_ln >> 1))
		if iter[index] != value:
			
			# window length only contains this value
			# if it's not correct, then it can't be in the iterable
			if win_ln <= 1:
				break

			if iter[index] > value:
				win_ln = index - start
			else:
				win_ln -= index - start
				start = index
		
		if iter[index] == value:
			if VERBOSE:
				print(iterations)
			return index
	print(iterations, start, "win_ln=", win_ln)
	return -1

def search(value, iter: Iterable) -> int:
	iterations = 0
	for i, v in enumerate(iter):
		iterations += 1
		if v == value:
			if VERBOSE:
				print(iterations)
			return i
	if VERBOSE:
		print(iterations)
	return -1

got_mismatch = False
mismatches = []
def test(val, iter):
	print("binary: ", end='')
	i1 = bsearch(val, iter)
	if i1 == -1:
		print(f"{val} not found")
	else:
		print(f"found {val} at {i1} = {iter[i1]}")
	print("linear: ", end='')
	i2 = search(val, iter)
	if i2 == -1:
		print(f"{val} not found")
	else:
		print(f"found {val} at {i2} = {iter[i2]}")
	if i1 != i2:
		global got_mismatch, mismatches
		got_mismatch = True
		mismatches.append(f"{val=}, binary={i1}, linear={i2}")
		print(f"!!! mismatch index found: binary={i1}, linear={i2} !!!")
		input("press enter to continue...")
	print()

def rand():
	return randint(-100, 1000)

x_len = randint(10000, 100000)
x = tuple(set(rand() for _ in range(1000)))
x = sorted(x)
print(x)

for _ in range(x_len):
	test(rand(), x)

print(f"\nDone processing a search on {x_len} sorted elements")

if got_mismatch:
	print("found mis-match, it's broken")
	print(mismatches)
else:
	print("no mis-matches, try more tests")
