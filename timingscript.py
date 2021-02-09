# timingscript.py
# a script to test the execution time of all the available functions for optimization

from time import time
import functions

# import a sample string that already exists
from constants import help_message

start = time()
string = functions.encode_bin(help_message)
end = time()
print ("encodeBin()   : %lf us" % ((end - start) * 1000000))

start = time()
string2 = functions.decode_bin(string)
end = time()
print ("decodeBin()   : %lf us" % ((end - start) * 1000000))

start = time()
string = functions.encode_dec(help_message)
end = time()
print ("encodeDec()   : %lf us" % ((end - start) * 1000000))

start = time()
string2 = functions.decode_dec(string)
end = time()
print ("decodeDec()   : %lf us" % ((end - start) * 1000000))

start = time()
string = functions.encode_hex(help_message)
end = time()
print ("encodeHex()   : %lf us" % ((end - start) * 1000000))

start = time()
string2 = functions.decode_hex(string)
end = time()
print ("decodeHex()   : %lf us" % ((end - start) * 1000000))

start = time()
string = functions.encode_base64(help_message)
end = time()
print ("encodeBase64(): %lf us" % ((end - start) * 1000000))

start = time()
string2 = functions.decode_base64(string)
end = time()
print ("decodeBase64(): %lf us" % ((end - start) * 1000000))

start = time()
string2 = functions.rot5(string)
end = time()
print ("rot5()        : %lf us" % ((end - start) * 1000000))

start = time()
string2 = functions.rot13(string)
end = time()
print ("rot13()       : %lf us" % ((end - start) * 1000000))

start = time()
string2 = functions.rot47(string)
end = time()
print ("rot47()       : %lf us" % ((end - start) * 1000000))
