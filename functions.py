# functions.py
# this file contains all the functions for use in encoding and decoding
# any changes to the encoding/encoding algorithms may take place here without changing the main script

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
            if empty_bytes == 1 : # if only last byte will need padding
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

# ROT13 is the same for encoding and decoding, so only one function
def rot13(string_in = ""):
    string_out = ""
    for c in string_in:
        d = ord(c)
        if 65 <= d and d <= 90:
            d = (d - 52) % 26 + 65
        elif 97 <= d and d <= 122:
            d = (d - 84) % 26 + 97
        else:
            pass # no modification is to be done on non-alphabet characters
        string_out += chr(d)
    return string_out

# ROT5 same as above function, but for digits only
def rot5(string_in = ""):
    string_out = ""
    for c in string_in:
        d = ord(c)
        if 48 <= d and d <= 57:
            d = (d - 43) % 10 + 48
        else:
            pass # no modification is to be done on non-numeric characters
        string_out += chr(d)
    return string_out

# ROT47 similar to the above, but with all ASCII printable characters
def rot47(string_in = ""):
    string_out = ""
    for c in string_in:
        d = ord(c)
        if 33 <= d and d <= 126:
            d = (d + 14) % 94 + 33
        else:
            pass # no modification is to be done on non-printable characters
        string_out += chr(d)
    return string_out

# string to decimal
def encode_dec(string = ""):
    decstr = ""
    for c in string:
        if 0 <= ord(c) and ord(c) < 256:
            decstr += str(ord(c)) + ' '

    return decstr

# decimal to string
def decode_dec(decstr = ""):
    string = ""
    buf = 0
    c = 0

    while c < len(decstr):
        if decstr[c] == ' ':
            if 0 <= buf and buf < 256:
                string += chr(buf)
            buf = 0
            c += 1
            continue
        if decstr[c].isdigit():
            buf *= 10
            buf += int(decstr[c])
            c += 1
    if buf != 0:
        string += chr(buf)
    return string

# string to hex
def encode_hex(string = ""):
    hexstr = ""
    for c in range(0, len(string)):
        i = ord(string[c])
        j = i - i % 16
        j = int(j / 16) # j is now representative of first hex digit
        if j < 10:
            hexstr += chr(j + 48)
        elif j < 16:
            hexstr += chr(j + 55)
        else:
            hexstr += '0'
        
        i -= j * 16 # i is second hex digit
        if i < 10:
            hexstr += chr(i + 48)
        elif i < 16:
            hexstr += chr(i + 55)
        else:
            hexstr += '0'
        
        hexstr += ' '
    return hexstr

# hex to string
def decode_hex(hexstr = ""):
    string = ""
    index = 0
    while len(hexstr) - index >= 2:
        num = int(0)
        
        character = ord(hexstr[index + 0])
        if 48 <= character and character <= 57:
            num += (character - 48) * 16
        elif 65 <= character and character <= 70:
            num += (character - 55) * 16
        
        character = ord(hexstr[index + 1])
        if 48 <= character and character <= 57:
            num += (character - 48) * 1
        elif 65 <= character and character <= 70:
            num += (character - 55) * 1
        
        string += chr(int(num))
        index += 2
        if index >= len(hexstr):
            break
        if hexstr[index] == ' ':
            index += 1
        
    return string

# string to binary
def encode_bin(string = ""):
    binstr = ""
    for c in range(0, len(string)):
        i = ord(string[c])
        for counter in range(8):
            if (i >= 128): # if even
                binstr = binstr + '1'
            else:
                binstr = binstr + '0'
            i = i << 1 # left shift i
            if i > 255:
                i -= 256
        binstr = binstr + ' '
    return binstr

# binary to string
def decode_bin(binstr = ""):
    string = ""
    index = 0
    while index < len(binstr):
        num = int(0)
        for bitno in range(0, 8):
            num += (128 / 2**bitno) * (ord(binstr[index + bitno]) - 48);
        string += chr(int(num))
        index += 8
        if index + 1 >= len(binstr):
            break

        while not (binstr[index] == '1' or binstr[index] == '0'):
            index += 1
        
    return string
