# asciicodecbot2
A Reddit bot for text conversion using the ASCII standard, utilizing [PRAW](https://github.com/praw-dev/praw). ASCIICodecBot takes an input of text and converts it into a numerical representation of said text, and vice versa.

## Hosting the bot
Hosting the bot is simple and takes 1 argument. ASCIICodecBot responds to specially-formatted comments that mention its username. The bot is called from the command line, and runs indefinitely until a Keyboard Interrupt is sent. All other exceptions will be caught and the bot will continue waiting for invocations.

Usage:
 
    $ python3 asciicodecbot [n]

where `n` is the delay in seconds between subsequent checks of the Reddit inbox. Smaller numbers yield less latency. If `n` is omitted, the delay defaults to 30 seconds.

## Summoning the bot

ASCIICodecBot will only respond to specially-formatted comments as per the code. The following information about invocation can be obtained by commenting `u/asciicodecbot help` on any post where ASCIICodecBot has the ability to reply.

    ASCIICodecBot ver. 2.0 - encode and decode ASCII text using various number systems
    
    Usage: u/asciicodecbot <command>[ <codetype>][: <text>]
    
    Commands:
      help,info - display this help information
      usage     - [NOT YET IMPLEMENTED] display simple usage instructions
      encode    - encode a message into numbers
      decode    - decode numbers into text
    
    Code Types:
      binary,bin      - bytes of binary, separated by spaces
      hexadecimal,hex - bytes of hexadeximal, separated by spaces
      decimal,dec     - decimal numbers separated by spaces
      rot13           - simple ROT13 encryption
    NOTE: if no code type is provided, binary is implied.
    
    ASCIICodecBot will encode/decode the parent comment/submission of the comment in which 
    it was mentioned. If desired, you can invoke ASCIICodecBot and provide it text in a 
    single comment, by appending your command with a ": " and your text.
    
    Usage Examples:
        u/asciicodecbot encode
        u/asciicOdecbot decode hex
        u/ASCIICODECBOt encode binary: your text goes here!
    
    Report bugs to u/Nissingmo.
