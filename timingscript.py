# timingscript.py

from time import time
import functions
from constants import help_message

help_message = help_message * 3
print (len(help_message))

print ("USING PYTHON FUNCTIONS\n")

start = time()
string = functions.encode_bin(help_message)
end = time()
print ("encodeBin(): %lf ms" % ((end - start) * 1000))

start = time()
string2 = functions.decode_bin(string)
end = time()
print ("decodeBin(): %lf ms" % ((end - start) * 1000))

start = time()
string = functions.encode_dec(help_message)
end = time()
print ("encodeDec(): %lf ms" % ((end - start) * 1000))

start = time()
string2 = functions.decode_dec(string)
end = time()
print ("decodeDec(): %lf ms" % ((end - start) * 1000))

start = time()
string = functions.encode_hex(help_message)
end = time()
print ("encodeHex(): %lf ms" % ((end - start) * 1000))

start = time()
string2 = functions.decode_hex(string)
end = time()
print ("decodeHex(): %lf ms" % ((end - start) * 1000))
