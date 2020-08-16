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

# base64 to string
def decode_base64(string_in = ""):
    string_out = ""
    
    string_in = string_in.replace('=', '') # remove padding characters
    list_in = list(string_in)
    # translate text into integers
    i = 0
    while i < len(list_in):
        c = ord(list_in[i])
        if 65 <= c and c <= 90: # capital alphabet
            c -= 65
        if 97 <= c and c <= 122: # lowercase alphabet
            c -= 71
        if 48 <= c and c <= 57: # numbers
            c += 4
        elif c == 43: # '+' symbol
            c == 62
        elif c == 47: # '/' symbol
            c = 63
        list_in[i] = chr(c)
        i += 1
    # group integers into byte triplets
    i = 0
    group = 0
    empty_bytes = (4 - (len(string_in) % 4)) % 4
    print(empty_bytes)
    while i < len(string_in):
        group = 0

        if len(string_in) + empty_bytes - i <= 4: # only if on last group
            print("last group")
            group |= (ord(list_in[i]) << 18) # first 2 chars will always be valid
            group |= (ord(list_in[i + 1]) << 12)
            if empty_bytes == 1 or empty_bytes == 0: # if only last byte used padding or none used
                group |= (ord(list_in[i + 2]) << 6)
            if empty_bytes == 0: # if no padding was needed
                group |= ord(list_in[i + 3])
        else: # for all other character groups
            group |= (ord(list_in[i]) << 18)
            group |= (ord(list_in[i + 1]) << 12)
            group |= (ord(list_in[i + 2]) << 6)
            group |= ord(list_in[i + 3])

        # now split into 3 bytes
        for char in range(2,-1,-1): # range of 2, 1, 0
            c = (group >> (8 * char)) & 255
            # append the character to the output string
            string_out += chr(c)
        i += 4
    # remove any padding characters
    #string_out = string_out[:(len(string_out) - empty_bytes)]
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
