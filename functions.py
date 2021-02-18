# functions.py

# The functions defined here act as wrappers for the functions defined
# in functions.c. They are accessed through the shared library 
# libasciicodec. All function wrappers are define here because they
# would otherwise clutter the main script, asciicodecbot2.py

import ctypes

# loading the shared library using the absolute path set in the
# Makefile during compile time
cfunctions = ctypes.cdll.LoadLibrary("/usr/lib/libasciicodec.so")

# Please note that all of these functions actually return char *;
# they must be cast to the proper type before being used, but I am
# keeping the return types set to void *, so that the pointer can
# successfully be passed to freechar() afterward.
cfunctions.freechar.argtypes = [ctypes.c_char_p]
cfunctions.freechar.restype = None
# bin
cfunctions.encodeBin.argtypes = [ctypes.c_char_p]
cfunctions.encodeBin.restype = ctypes.c_void_p
cfunctions.decodeBin.argtypes = [ctypes.c_char_p]
cfunctions.decodeBin.restype = ctypes.c_void_p
# dec
cfunctions.encodeDec.argtypes = [ctypes.c_char_p]
cfunctions.encodeDec.restype = ctypes.c_void_p
cfunctions.decodeDec.argtypes = [ctypes.c_char_p]
cfunctions.decodeDec.restype = ctypes.c_void_p
# hex
cfunctions.encodeHex.argtypes = [ctypes.c_char_p]
cfunctions.encodeHex.restype = ctypes.c_void_p
cfunctions.decodeHex.argtypes = [ctypes.c_char_p]
cfunctions.decodeHex.restype = ctypes.c_void_p
# rot5
cfunctions.rot5.argtypes = [ctypes.c_char_p]
cfunctions.rot5.restype = ctypes.c_void_p
# rot13
cfunctions.rot13.argtypes = [ctypes.c_char_p]
cfunctions.rot13.restype = ctypes.c_void_p
# rot47
cfunctions.rot47.argtypes = [ctypes.c_char_p]
cfunctions.rot47.restype = ctypes.c_void_p
# base64
cfunctions.encodeBase64.argtypes = [ctypes.c_char_p]
cfunctions.encodeBase64.restype = ctypes.c_void_p
cfunctions.decodeBase64.argtypes = [ctypes.c_char_p]
cfunctions.decodeBase64.restype = ctypes.c_void_p

# string to base64
def encode_base64(string_in = ""):
    pstring_out = cfunctions.encodeBase64(bytes(string_in, "utf-8"))
    pstring_out = ctypes.cast(pstring_out, ctypes.c_char_p)
    # decode UTF-8 and trim to max comment length
    string_out = str(pstring_out.value, "utf-8")[:10000]
    cfunctions.freechar(pstring_out)
    return string_out

# base64 to string
def decode_base64(string_in = ""):
    pstring_out = cfunctions.decodeBase64(bytes(string_in, "utf-8"))
    pstring_out = ctypes.cast(pstring_out, ctypes.c_char_p)
    # decode UTF-8 and trim to max comment length
    string_out = str(pstring_out.value, "utf-8", "replace")[:10000]
    cfunctions.freechar(pstring_out)
    return string_out

# ROT13 is the same for encoding and decoding, so only one function
def rot13(string_in = ""):
    pstring_out = cfunctions.rot13(bytes(string_in, "utf-8"))
    pstring_out = ctypes.cast(pstring_out, ctypes.c_char_p)
    # decode UTF-8 and trim to max comment length
    string_out = str(pstring_out.value, "utf-8")[:10000]
    cfunctions.freechar(pstring_out)
    return string_out

# ROT5 same as above function, but for digits only
def rot5(string_in = ""):
    pstring_out = cfunctions.rot5(bytes(string_in, "utf-8"))
    pstring_out = ctypes.cast(pstring_out, ctypes.c_char_p)
    # decode UTF-8 and trim to max comment length
    string_out = str(pstring_out.value, "utf-8")[:10000]
    cfunctions.freechar(pstring_out)
    return string_out

# ROT47 similar to the above, but with all ASCII printable characters
def rot47(string_in = ""):
    pstring_out = cfunctions.rot47(bytes(string_in, "utf-8"))
    pstring_out = ctypes.cast(pstring_out, ctypes.c_char_p)
    # decode UTF-8 and trim to max comment length
    string_out = str(pstring_out.value, "utf-8")[:10000]
    cfunctions.freechar(pstring_out)
    return string_out
    
# string to decimal
def encode_dec(string = ""):
    pdecstr = cfunctions.encodeDec(bytes(string, "utf-8"))
    pdecstr = ctypes.cast(pdecstr, ctypes.c_char_p)
    # decode UTF-8 and trim to max comment length
    decstr = str(pdecstr.value, "utf-8")[:10000]
    cfunctions.freechar(pdecstr)
    return decstr

# decimal to string
def decode_dec(decstr = ""):
    pstring = cfunctions.decodeDec(bytes(decstr, "utf-8"))
    pstring = ctypes.cast(pstring, ctypes.c_char_p)
    # decode UTF-8 and trim to max comment length
    string = str(pstring.value, "utf-8", "replace")[:10000]
    cfunctions.freechar(pstring)
    return string

# string to hex
def encode_hex(string = ""):
    phexstr = cfunctions.encodeHex(bytes(string, "utf-8"))
    phexstr = ctypes.cast(phexstr, ctypes.c_char_p)
    # decode UTF-8 and trim to max comment length
    hexstr = str(phexstr.value, "utf-8")[:10000]
    cfunctions.freechar(phexstr)
    return hexstr

# hex to string
def decode_hex(hexstr = ""):
    pstring = cfunctions.decodeHex(bytes(hexstr, "utf-8"))
    pstring = ctypes.cast(pstring, ctypes.c_char_p)
    # decode UTF-8 and trim to max comment length
    string = str(pstring.value, "utf-8", "replace")[:10000]
    cfunctions.freechar(pstring)
    return string

# string to binary
def encode_bin(string = ""):
    pbinstr = cfunctions.encodeBin(bytes(string, "utf-8"))
    pbinstr = ctypes.cast(pbinstr, ctypes.c_char_p)
    # decode UTF-8 and trim to max comment length
    binstr = str(pbinstr.value, "utf-8")[:10000]
    cfunctions.freechar(pbinstr)
    return binstr

# binary to string
def decode_bin(binstr = ""):
    pstring = cfunctions.decodeBin(bytes(binstr, "utf-8"))
    pstring = ctypes.cast(pstring, ctypes.c_char_p)
    # decode UTF-8 and trim to max comment length
    string = str(pstring.value, "utf-8", "replace")[:10000]
    cfunctions.freechar(pstring)
    return string
