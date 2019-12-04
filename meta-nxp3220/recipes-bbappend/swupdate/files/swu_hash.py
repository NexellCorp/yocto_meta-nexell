#!/usr/bin/env python3

import sys

def swupdate_get_sha256(filename):
	import hashlib

	m = hashlib.sha256()

	with open(filename, 'rb') as f:
		while True:
			data = f.read(1024)
			if not data:
				break
			m.update(data)
	return m.hexdigest()


def main():
	file = sys.argv[1]
	return swupdate_get_sha256(file)


if __name__ == "__main__":
	try:
		ret = main()
	except Exception:
		ret = 1
		import traceback
		traceback.print_exc()
	sys.exit(ret)
