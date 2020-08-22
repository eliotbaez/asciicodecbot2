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
cfunctions.decodeBin.argtypes = [ctypes.c_wchar_p]
cfunctions.decodeBin.restype = ctypes.c_void_p
# dec
cfunctions.encodeDec.argtypes = [ctypes.c_wchar_p]
cfunctions.encodeDec.restype = ctypes.c_void_p
cfunctions.decodeDec.argtypes = [ctypes.c_wchar_p]
cfunctions.decodeDec.restype = ctypes.c_void_p
# hex
cfunctions.encodeHex.argtypes = [ctypes.c_wchar_p]
cfunctions.encodeHex.restype = ctypes.c_void_p
cfunctions.decodeHex.argtypes = [ctypes.c_wchar_p]
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


# string to base64
def encode_base64(string_in = ""):
    string_out = ""
    length_in = len(string_in) # faster to read a variable than call a function
    i = 0
    group = 0
    empty_bytes = (3 - (length_in % 3)) % 3
    # split byte triplets into groups of three 6-bit characters
    while i < length_in:
        group = 0
        if length_in - i <= 3: # only if on last triplet
            group |= (ord(string_in[i]) << 16) # first byte will always be valid
            if empty_bytes == 1 or empty_bytes == 0: # if only last byte will need padding or none needed
                group |= (ord(string_in[i + 1]) << 8)
            if empty_bytes == 0: # if no padding will be needed
                group |= ord(string_in[i + 2])
        else: # for all other byte triplets
            group |= (ord(string_in[i]) << 16)
            group |= (ord(string_in[i + 1]) << 8)
            group |= ord(string_in[i + 2])

        # now split into four 6-byte characters
        for char in range(3,-1,-1): # range of 3, 2, 1, 0
            c = (group >> (6 * char)) & 63
            # append the character to the output string
            string_out += chr(c)
        i += 3
    # convert string to list to assign individual characters
    list_out = list(string_out)
    # translate output characters into valid text
    i = 0
    while i < len(string_out):
        c = ord(list_out[i])
        if 0 <= c and c <= 25: # capital alphabet
            c += 65
        elif 26 <= c and c <= 51: # lowercase alphabet
            c += 71
        elif 52 <= c and c <= 61: # numbers
            c -= 4
        elif c == 62: # '+' symbol
            c = 43
        elif c == 63: # '/' symbol
            c = 47
        list_out[i] = chr(c)
        i += 1
    # overwrite padding characters
    for i in range(len(list_out) - empty_bytes, len(list_out)):
        list_out[i] = '='
    string_out = "".join(list_out)
    return string_out

# base64 to string
def decode_base64(string_in = ""):
    string_in = string_in.replace('=', '') # remove padding characters
    # check for input validity
    invalid_chars = 0
    for char in string_in:
        if not (char.isalnum() or char == '/' or char == '+'):
            invalid_chars += 1
    if invalid_chars > 0: # there are invalid characters
        return ("Input invalid.")
    elif len(string_in) % 4 == 1: # not enough characters
        return ("Input invalid.")
        
    string_out = ""
    
    list_in = list(string_in)
    # translate text into integers
    i = 0
    while i < len(list_in):
        c = ord(list_in[i])
        if 65 <= c and c <= 90: # capital alphabet
            c -= 65
        elif 97 <= c and c <= 122: # lowercase alphabet
            c -= 71
        elif 48 <= c and c <= 57: # numbers
            c += 4
        elif c == 43: # '+' symbol
            c == 62
        elif c == 47: # '/' symbol
            c = 63
        list_in[i] = c
        i += 1
    #print(list_in)
    # group integers into byte triplets
    i = 0
    group = 0
    empty_bytes = (4 - (len(string_in) % 4)) % 4
    #print(empty_bytes)
    while i < len(string_in):
        group = 0

        if len(string_in) + empty_bytes - i <= 4: # only if on last group
            #print("last group")
            group |= (list_in[i] << 18) # first 2 chars will always be valid
            group |= (list_in[i + 1] << 12)
            if empty_bytes == 1 or empty_bytes == 0: # if only last byte used padding or none used
                group |= (list_in[i + 2] << 6)
            if empty_bytes == 0: # if no padding was needed
                group |= list_in[i + 3]
        else: # for all other character groups
            #print("regular group")
            group |= (list_in[i] << 18)
            group |= (list_in[i + 1] << 12)
            group |= (list_in[i + 2] << 6)
            group |= list_in[i + 3]
        #print(group)
        # now split into 3 bytes
        for char in range(2,-1,-1): # range of 2, 1, 0
            c = (group >> (8 * char)) & 255
            # append the character to the output string
            string_out += chr(c)
        i += 4
    # remove any padding characters
    string_out = string_out[:(len(string_out) - empty_bytes)]
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
    pstring = cfunctions.decodeHex(hexstr)
    pstring = ctypes.cast(pstring, ctypes.c_wchar_p)
    string = pstring.value
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
    pstring = cfunctions.decodeBin(binstr)
    pstring = ctypes.cast(pstring, ctypes.c_wchar_p)
    string = pstring.value
    cfunctions.freewchar(pstring)
    return string
