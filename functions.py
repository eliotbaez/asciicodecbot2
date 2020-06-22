# TODO: reformat sto* functions, use python's built-in functions instead if possible
# NOTE: parse_ functions are deprecated and thus should not be used. They are only here for compatibility reasons with asciicodecbot v0.1-1.0

def rot13(string_in = ""):
    string_out = ""
    for c in string_in:
        d = ord(c)
        if 65 <= d and d <= 90:
            d = (d - 52) % 26 + 65
        elif 97 <= d and d <= 122:
            d = (d - 84) % 26 + 97
        else:
            pass # no modification is to be done on non-alphanumeric characters
        string_out += chr(d)
    return string_out

def stod(string = ""):
    decstr = ""
    for c in string:
        if 0 <= ord(c) and ord(c) < 256:
            decstr += str(ord(c)) + ' '

    return decstr

def dtos(decstr = ""):
    str = ""
    buf = 0
    c = 0

    while c < len(decstr):
        if decstr[c] == ' ':
            if 0 <= buf and buf < 256:
                str += chr(buf)
            buf = 0
            c += 1
            continue
        if decstr[c].isdigit():
            buf *= 10
            buf += int(decstr[c])
            c += 1
    if buf != 0:
        str += chr(buf)
    return str

def stox(str = "", start_ind = 0):
    hexstr = ""
    if start_ind < 0:
        return ""
    for c in range(start_ind, len(str)):
        i = ord(str[c])
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

def xtos(hexstr = "", start_ind = 0):
    str = ""
    if start_ind < 0:
        return ""
    index = start_ind
    while len(hexstr) - index >= 2:
        num = int(0)
        #for bitno in range(0, 2):
        
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
        
        str += chr(int(num))
        index += 2
        if index >= len(hexstr):
            break
        if hexstr[index] == ' ':
            index += 1
        
    return str

def parse_hex(hexstr = "", start_ind = 0):
    index = start_ind
    noData = True
    #start_ind = 0
    character = ''
    #num = 1
    while index < len(hexstr):
        #print("index=", index)
        if not noData:
            break
            
        character = ord(hexstr[index])
        if not ((48 <= character and character <= 57) or (65 <= character and character <= 70)): # if not a hex digit
            #print("no x")
            index += 1
        else: # if yes hex digit
            #print("yes x")
            if len(hexstr) - index >= 2: # if remaining chars can form a byte
                #print("enough chars for byte")
                for charno in range(0,2):
                    character = ord(hexstr[index + charno])
                    if not ((48 <= character and character <= 57) or (65 <= character and character <= 70)): #if not hex digit
                        #print("not a hex")
                        index += charno + 1# increment index by bit number
                        break
                    if charno == 1:
                        #code
                        
                        if noData:
                            start_ind = index
                        noData = False
                        
            else: #only if remaining chars cannot form byte
                #print("not enough for byte")
                index = len(hexstr)-1
                break
            index += 2
    if noData:
        return -1
    else: 
        return start_ind
    str = ""
    if start_ind < 0:
        return ""
    index = start_ind
    while index < len(binstr):
        num = int(0)
        for bitno in range(0, 8):
            num += (128 / 2**bitno) * (ord(binstr[index + bitno]) - 48);
        str += chr(int(num))
        index += 8
        if index >= len(binstr):
            break
        if binstr[index] == ' ':
            index += 1
        
    return str

def stob(str = "", start_ind = 0):
    binstr = ""
    if start_ind < 0:
        return ""
    for c in range(start_ind, len(str)):
        i = ord(str[c])
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
    
def btos(binstr = "", start_ind = 0):
    str = ""
    if start_ind < 0:
        return ""
    index = start_ind
    while index < len(binstr):
        num = int(0)
        for bitno in range(0, 8):
            num += (128 / 2**bitno) * (ord(binstr[index + bitno]) - 48);
        str += chr(int(num))
        index += 8
        if index + 1 >= len(binstr):
            break

        while not (binstr[index] == '1' or binstr[index] == '0'):
            index += 1
        
    return str
        
def parse_bin(binstr = ""):
    index = 0
    noData = True
    start_ind = 0
    #num = 1
    while index < len(binstr):
        #print("index=", index)
        if not noData:
            break
        if not (binstr[index + 0] == '0' or binstr[index + 0] == '1'): # if neither 1 nor 0
            index += 1
        else: # if yes 1 or 0
            if len(binstr) - index >= 8: # if remaining chars can form a byte
                #print("enough chars for byte")
                for charno in range(0,8):
                    if not (binstr[index + charno] == '0' or binstr[index + charno] == '1'): #if neither 0 nor 1, for each char
                        #print("not a bit")
                        index += charno + 1# shift index right by bit number
                        break
                    if charno == 7:
                        #code
                        
                        if noData:
                            start_ind = index
                        noData = False
                        
            else: #only if remaining chars cannot form byte
                #print("not enough for byte")
                index = len(binstr)-1
                break
    if noData:
        return -1
    else: 
        return start_ind
