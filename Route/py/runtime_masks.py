from itertools import batched
from random import randint


def get_mask():
	return randint(0, 1<<32)

values_count = 256
values = [get_mask() for i in range(values_count)]
per_line_values = ',\n'.join(','.join(map(lambda c: hex(c) + 'U', i)) for i in batched(values, 8))
print(per_line_values)
