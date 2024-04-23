
import enum
from pathlib import Path
import re

__folder__ = Path(__file__).parent

enum_matcher_re = re.compile(r"enum\s+class\s+Error\s*:\s*\w*\s+{([\w\s,]+)}")
errors_header = __folder__.parent.joinpath(Path(r'src/Route/Error.h'))

MAX_NAME_LN = 64

# all names are post-processed before they are written/output-ed
def name_post_process_n(name: str):
	return f'"{name}"'

def name_post_process_w(name: str):
	return f'L"{name}"'

def name_post_process_cn(name: str):
	return '{ ' f'"{name}"' ' }'

def name_post_process_cw(name: str):
	return '{ ' f'L"{name}"' ' }'

class ProcessMode(enum.IntEnum):
	Normal = 0
	Wide = 1
	CompoundNormal = 2
	CompoundWide = 3

	def get_post_process(self):
		match self:
			case ProcessMode.Normal:
				return name_post_process_n
			case ProcessMode.Wide:
				return name_post_process_w
			case ProcessMode.CompoundNormal:
				return name_post_process_cn
			case ProcessMode.CompoundWide:
				return name_post_process_cw
		return name_post_process_n

	def get_name(self):
		match self:
			case ProcessMode.Normal:
				return "normal"
			case ProcessMode.Wide:
				return "wide"
			case ProcessMode.CompoundNormal:
				return "compound normal"
			case ProcessMode.CompoundWide:
				return "compound wide"
		return ProcessMode.Normal.get_name()
	
def print_process_modes_help():
	for i in range(4):
		print(f"type {i} for the {ProcessMode(i).get_name()} mode")

def get_process_mode() -> ProcessMode:
	print_process_modes_help()

	while True:
		val = input("mode: ")
		try:
			val = int(val)
		except ValueError:
			print(f"unknown type: '{val}'")
			continue
		
		if val < 0 or val > ProcessMode.CompoundWide:
			print(f"out of range mode: {val}")
			continue
		return val
	return 0


def main():
	longest_name_found = ''
	process_mode = ProcessMode(get_process_mode())

	# reading the header
	with open(errors_header, 'r') as f:
		text = f.read()

	enum_match = enum_matcher_re.search(text)

	# checking if the regex match is valid
	if enum_match is None:
		print(f"Couldn't find the Error enum in '{errors_header}'")
		quit()

	enum_body = enum_match[1]
	names = []

	# parsing names and discarding empty lines
	for i in enum_body.splitlines():
		i = i.strip().rstrip(',')
		if not i:
			continue
		if len(i) >= MAX_NAME_LN:
			print(f"error name '{i}' has a length = {len(i)}; max name length = {MAX_NAME_LN}")
		if len(longest_name_found) < len(i):
			longest_name_found = i
		names.append(i)
		
	name_post_process = process_mode.get_post_process()
	# writing the names after post processing them
	with open(__folder__.joinpath("error_names.txt"), 'w') as f:
		f.write(',\n'.join(map(name_post_process, names)))

	print(f"longest error name is '{longest_name_found}' which is {len(longest_name_found)} characters long")

if __name__ == '__main__':
	main()
