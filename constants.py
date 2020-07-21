# constants.py
# these constants shouldn't have to change during normal bot operation

help_message = '    ASCIICodecBot ver. 2.0 - encode and decode ASCII text using various\n    number systems\n    \n    Usage: u/asciicodecbot <command>[ <codetype>][: <text>]\n    \n    Commands:\n      help,info - display this help information\n      usage     - display simple usage instructions\n      encode    - encode a message into numbers\n      decode    - decode numbers into text\n    \n    Code Types:\n      binary,bin      - bytes of binary, separated by spaces\n      hexadecimal,hex - bytes of hexadeximal, separated by spaces\n      decimal,dec     - decimal numbers separated by spaces\n      rot13           - simple ROT13 encryption\n    NOTE: if no code type is provided, binary is implied.\n    \n    ASCIICodecBot will encode/decode the parent comment/submission of the\n    comment in which it was mentioned. If desired, you can invoke\n    ASCIICodecBot and provide it text in a single comment, by appending your\n    command with a ": " and your text.\n    \n    Usage Examples:\n      u/asciicodecbot encode\n      u/asciicOdecbot decode hex\n      u/ASCIICODECBOt encode binary: your text goes here!\n    \n    Report bugs to u/Nissingmo.'

err_no_command = 'Error: You need to issue a command.\n'

err_code_unknown = 'Error: Codetype "%s" is unknown.\n'

err_command_unknown = 'Error: Command "%s" is unknown.\n'

usage_instructions = '\n    Basic Usage: u/asciicodecbot <command>[ <codetype>][: <text>]\n    Commands: help,info; usage; encode; decode\n    Code types: binary, decimal, hexadecimal, ROT13\n'

# masks and flags
# these leave room for future functionality without needing to add much code to implement it
# all the information about the service requested is stored in a single variable, service_requested
MASK_CMD     = 0x00F
MASK_CODE    = 0x0F0
MASK_MOD     = 0xF00

# commands
CMD_NONE     = 0x000
CMD_HELP     = 0x001
CMD_ENCODE   = 0x002
CMD_DECODE   = 0x003
CMD_USAGE    = 0x004
CMD_THROW    = 0x00E
CMD_UNKNOWN  = 0x00F

# code types
CODE_BIN     = 0x010
CODE_HEX     = 0x020
CODE_DEC     = 0x030
CODE_ROT13   = 0x040
CODE_UNKNOWN = 0x0F0

# modifiers
# this will store miscellaneous information
# as of now, it just stores whether the comment is requesting for the bot to read the comment itself, or the parent comment
MOD_PARENT   = 0x000
MOD_SELF     = 0X100
