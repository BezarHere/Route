
import pyperclip
from preprocessors import get_tokens, token_type

def capitalize(ident: str):
	return '_'.join(i.capitalize() for i in ident.split('_'))

enum_src = ''
with open('glew.h', 'r') as f:
	for i in get_tokens(f.read()):
		if i.type == token_type.DEFINE:
			ss = i.value.split(' ')
			if len(ss) != 2:
				continue
			
			n, v = ss

			# only accept hex constants
			if not v.startswith('0x') and not v.startswith('0X'):
				continue
			v = v[2:]

			value = None
			try:
				value = int(v, base=16)
			except ValueError:
				...
			if value is None:
				continue
			print(n, value)
			enum_src += f'{capitalize(n[3:])} = {hex(value)},\n'

pyperclip.copy(enum_src)
