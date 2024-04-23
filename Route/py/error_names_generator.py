
import re


enum_matcher_re = re.compile(r"enum\s+class\s+Error\s+{([\w\s,]+)}")
errors_header = r'../src/Route/Error.h'

MAX_NAME_LN = 64
longest_name_found = ''

# all names are post-processed before they are written/output-ed
def name_post_process(name: str):
	return '{ ' f'"{name}"' ' }'

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
	

# writing the names after post processing them
with open("error_names.txt", 'w') as f:
	f.write(',\n'.join(map(name_post_process, names)))

print(f"longest error name is '{longest_name_found}' which is {len(longest_name_found)} characters long")
