
from enum import IntEnum
import json
import re
from attr import dataclass

_PRE_STATEMENT_RE = re.compile(r"\s*#\s*(\w+)\s*(.*)?")

def tokenize_preprocessor(src: str):
	tokens = {}
	for index, line in enumerate(src.split('\n')):
		find = _PRE_STATEMENT_RE.search(line)
		if find:
			tokens[f"{index}:{find[1]}"] = find[2]
	return tokens

class token_type(IntEnum):
	DEFINE = 0
	IFDEF = 1
	IFNDEF = 2
	IF = 3
	ELIF = 4
	ELSE = 5
	UNDEFINE = 6

@dataclass
class token:
	type: token_type
	line: int
	value: str

def get_tokens(src: str):
	for i, v in tokenize_preprocessor(src).items():
		l, t = i.split(':')
		n = 0
		match t:
			case 'define':
				n = token_type.DEFINE
			case 'ifdef':
				n = token_type.IFDEF
			case 'ifndef':
				n = token_type.IFNDEF
			case 'if':
				n = token_type.IF
			case 'elif':
				n = token_type.ELIF
			case 'else':
				n = token_type.ELSE
			case 'undef':
				n = token_type.UNDEFINE
				
		yield token(n, int(l), v)
