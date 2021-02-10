# functions.py
# this file contains all the functions for use in encoding and decoding
# any changes to the encoding/encoding algorithms may take place here without changing the main script

# The functions defined in functions.py will act as wrappers for the C
# functions as they become available. This will help to declutter the
# main script.

import ctypes

cfunctions = ctypes.cdll.LoadLibrary("./cfunctions.so")

# Please note that all of these functions actually return wchar_t *
# they must be cast to the proper type before being used, but I am
# keeping the return types set to void *, so that the pointer can
# successfully be passed to freewchar() afterward.
cfunctions.freewchar.argtypes = [ctypes.c_void_p]
cfunctions.freewchar.restype = ctypes.c_void_p
# bin
cfunctions.encodeBin.argtypes = [ctypes.c_wchar_p]
cfunctions.encodeBin.restype = ctypes.c_void_p
cfunctions.decodeBin.argtypes = [ctypes.c_char_p]
cfunctions.decodeBin.restype = ctypes.c_void_p
# dec
cfunctions.encodeDec.argtypes = [ctypes.c_wchar_p]
cfunctions.encodeDec.restype = ctypes.c_void_p
cfunctions.decodeDec.argtypes = [ctypes.c_wchar_p]
cfunctions.decodeDec.restype = ctypes.c_void_p
# hex
cfunctions.encodeHex.argtypes = [ctypes.c_wchar_p]
cfunctions.encodeHex.restype = ctypes.c_void_p
cfunctions.decodeHex.argtypes = [ctypes.c_char_p]
cfunctions.decodeHex.restype = ctypes.c_void_p
# rot5
cfunctions.rot5.argtypes = [ctypes.c_wchar_p]
cfunctions.rot5.restype = ctypes.c_void_p
# rot13
cfunctions.rot13.argtypes = [ctypes.c_wchar_p]
cfunctions.rot13.restype = ctypes.c_void_p
# rot47
cfunctions.rot47.argtypes = [ctypes.c_wchar_p]
cfunctions.rot47.restype = ctypes.c_void_p
# base64
cfunctions.encodeBase64.argtypes = [ctypes.c_wchar_p]
cfunctions.encodeBase64.restype = ctypes.c_void_p
cfunctions.decodeBase64.argtypes = [ctypes.c_wchar_p]
cfunctions.decodeBase64.restype = ctypes.c_void_p

# string to base64
def encode_base64(string_in = ""):
    pstring_out = cfunctions.encodeBase64(string_in)
    pstring_out = ctypes.cast(pstring_out, ctypes.c_wchar_p)
    string_out = pstring_out.value
    cfunctions.freewchar(pstring_out)
    return string_out

# base64 to string
def decode_base64(string_in = ""):
    pstring_out = cfunctions.decodeBase64(string_in)
    pstring_out = ctypes.cast(pstring_out, ctypes.c_wchar_p)
    string_out = pstring_out.value
    cfunctions.freewchar(pstring_out)
    return string_out

# ROT13 is the same for encoding and decoding, so only one function
def rot13(string_in = ""):
    pstring_out = cfunctions.rot13(string_in)
    pstring_out = ctypes.cast(pstring_out, ctypes.c_wchar_p)
    string_out = pstring_out.value
    cfunctions.freewchar(pstring_out)
    return string_out

# ROT5 same as above function, but for digits only
def rot5(string_in = ""):
    pstring_out = cfunctions.rot5(string_in)
    pstring_out = ctypes.cast(pstring_out, ctypes.c_wchar_p)
    string_out = pstring_out.value
    cfunctions.freewchar(pstring_out)
    return string_out

# ROT47 similar to the above, but with all ASCII printable characters
def rot47(string_in = ""):
    pstring_out = cfunctions.rot47(string_in)
    pstring_out = ctypes.cast(pstring_out, ctypes.c_wchar_p)
    string_out = pstring_out.value
    cfunctions.freewchar(pstring_out)
    return string_out
    
# string to decimal
def encode_dec(string = ""):
    pdecstr = cfunctions.encodeDec(string)
    pdecstr = ctypes.cast(pdecstr, ctypes.c_wchar_p)
    decstr = pdecstr.value
    cfunctions.freewchar(pdecstr)
    return decstr

# decimal to string
def decode_dec(decstr = ""):
    pstring = cfunctions.decodeDec(decstr)
    pstring = ctypes.cast(pstring, ctypes.c_wchar_p)
    string = pstring.value
    cfunctions.freewchar(pstring)
    return string

# string to hex
def encode_hex(string = ""):
    phexstr = cfunctions.encodeHex(string)
    phexstr = ctypes.cast(phexstr, ctypes.c_wchar_p)
    hexstr = phexstr.value
    cfunctions.freewchar(phexstr)
    return hexstr

# hex to string
def decode_hex(hexstr = ""):
    pstring = cfunctions.decodeHex(bytes(hexstr, "utf-8"))
    pstring = ctypes.cast(pstring, ctypes.c_char_p)
    string = str(pstring.value, "utf-8")
    cfunctions.freewchar(pstring)
    return string

# string to binary
def encode_bin(string = ""):
    pbinstr = cfunctions.encodeBin(string)
    pbinstr = ctypes.cast(pbinstr, ctypes.c_wchar_p)
    binstr = pbinstr.value
    cfunctions.freewchar(pbinstr)
    return binstr

# binary to string
def decode_bin(binstr = ""):
    pstring = cfunctions.decodeBin(bytes(binstr, "utf-8"))
    pstring = ctypes.cast(pstring, ctypes.c_char_p)
    string = str(pstring.value, "utf-8")
    cfunctions.freewchar(pstring)
    return string
