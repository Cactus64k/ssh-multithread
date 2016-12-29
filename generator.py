#!/usr/bin/env python3
import io

def main():
	passwd	= "123"
	user	= "root"
	f		= open("test1.cfg", "w", encoding="UTF-8")
	for r in range(1, 8):
		f.write("{}:{}@sv{}\n".format(user, passwd, r*100+1))
		for i in range(2, 26):
			f.write("{}:{}@smoloper{}\n".format(user, passwd, r*100+i))
		f.write("{}:{}@sv{}\n".format(user, passwd, r*100+26))
		f.write("\n\n")
	f.close()


if __name__ == "__main__":
	main()
