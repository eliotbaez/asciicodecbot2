#!/usr/bin/python3

import sys
import praw
import pdb
import re
import os
import datetime
import time
import traceback

import functions # function definitions in separate file to declutter main script
import constants # same as above but for constants
import private # this info should not be publicly available

reddit = praw.Reddit(
        client_id=private.client_id, 
        client_secret=private.client_secret, 
        user_agent=private.user_agent, 
        username=private.username, 
        password=private.password)

if not os.path.isfile("posts_replied_to.txt"):
    posts_replied_to = []
else:
    with open("posts_replied_to.txt", "r") as f:
        posts_replied_to = f.read()

        posts_replied_to = posts_replied_to.split("\n")
        posts_replied_to = list(filter(None, posts_replied_to))
        
if not os.path.isfile("posts_seen.txt"):
    posts_seen = []
else:
    with open("posts_seen.txt", "r") as f:
        posts_seen = f.read()
        posts_seen = posts_seen.split("\n")
        posts_seen = list(filter(None, posts_seen))

cache = ""

if len(sys.argv) > 1:
    time_interval = int(sys.argv[1])
else:
    time_interval = 30

starting_timestamp = int(time.time())
iterations = 0

if time_interval < 1:
    time_interval = 30

# main loop
while True:
    reply_qty = 0
    iterations += 1
    
    try:
        while time.time() % time_interval > 0.5:
            sys.stdout.write("\r%02d seconds until refresh..." % (time_interval - time.time() % time_interval))
            sys.stdout.flush()
            time.sleep(0.5)
    except:
        print("\n\n-- %d REPLIES SENT --\n" % reply_qty)
        raise
        
    print("\niteration %d\n" % iterations)
    print("****In mentions:****\n")
    
    try:
        for comment in reddit.inbox.unread(mark_read = True, limit = None):
            if (comment.id not in posts_replied_to) and (comment.id not in posts_seen):
                author = comment.author
                submission = comment.submission
                post_is_deleted = False

                if comment.body == "[deleted]" or comment.body == "[removed]":
                    print ("[post not accessible]")
                    post_is_deleted = True
                
                replySent = False
                
                if not post_is_deleted:
                    print("in r/%s" % comment.subreddit)
                    print("by u/%s" % author.name)
                    print("comment body:\n\n%s\n" % (comment.body))
                    
                
                
                    try:
                        if "asciicodecbot" not in author.name: # avoid replying to self
                            
                            match = re.search("(?s)u/asciicodecbot (\w+)(?:(?: ?(?:(?:(\w*)(?: ?(\w*))?)?))?)(?:(: )?)(.*)", comment.body, re.IGNORECASE)  
                            if match:
                                command = match.group(1).lower()
                                code = match.group(2).lower()
                                modifier = match.group(3).lower()
                                target_indicator = match.group(4) # this group is either ": " or blank
                                self_source = match.group(5) # this is everything else 
                            else:
                                # no match was found, but the username was still mentioned
                                command = code = modifier = target_indicator = self_source = ""

                            # time to figure out what the requested service is
                            service_requested = 0x000
                            
                            # command parse
                            if command == "":
                                service_requested |= constants.CMD_NONE
                            elif command == "help" or command == "info":
                                service_requested |= constants.CMD_HELP
                            elif command == "encode":
                                service_requested |= constants.CMD_ENCODE
                            elif command == "decode":
                                service_requested |= constants.CMD_DECODE
                            elif command == "usage":
                                service_requested |= constants.CMD_USAGE
                            else:
                                service_requested |= constants.CMD_UNKNOWN

                            # code parse
                            if code == "" or code == "bin" or code == "binary":
                                service_requested |= constants.CODE_BIN
                            elif code == "hex" or code == "hexadecimal":
                                service_requested |= constants.CODE_HEX
                            elif code == "dec" or code == "decimal":
                                service_requested |= constants.CODE_DEC
                            elif code == "rot13":
                                service_requested |= constants.CODE_ROT13
                            else:
                                service_requested |= constants.CODE_UNKNOWN
                            
                            # modifiers parse
                            # note that 'modifier' has no effect as of now, but will be left in for future additions 
                            if target_indicator == ": ":
                                service_requested |= constants.MOD_SELF
                            else: # field is blank
                                service_requested |= constants.MOD_PARENT

                            if comment.id not in cache: # we are now clear to reply
                                # first set the text which will act as the target
                                if service_requested & constants.MASK_MOD == constants.MOD_SELF:
                                    target = self_source
                                else: # only other option is for the target to be the parent comment
                                    if comment.parent_id.find("t3_", 0) == 0:
                                        target = submission.selftext
                                    else:
                                        target = comment.parent().body
                                if service_requested & constants.MASK_CMD == constants.CMD_NONE:
                                    comment.reply(constants.err_no_command + constants.usage_instructions)
                                    cache += comment.id
                                    posts_replied_to.append(comment.id)
                                    replySent = True
                                    print("reply sent: error message, missing command")
                                elif service_requested & constants.MASK_CMD == constants.CMD_HELP:
                                    comment.reply(constants.help_message)
                                    cache += comment.id
                                    posts_replied_to.append(comment.id)
                                    replySent = True
                                    print("reply sent: help message.")
                                elif service_requested & constants.MASK_CMD == constants.CMD_USAGE:
                                    comment.reply(constants.usage_instructions)
                                    cache += comment.id
                                    posts_replied_to.append(comment.id)
                                    replySent = True
                                    print("reply sent: basic usage")
                                elif service_requested & constants.MASK_CMD == constants.CMD_ENCODE:
                                    if service_requested & constants.MASK_CODE == constants.CODE_BIN:
                                        comment.reply(functions.stob(target))
                                        print("reply sent: binary.")
                                    if service_requested & constants.MASK_CODE == constants.CODE_HEX:
                                        comment.reply(functions.stox(target))
                                        print("reply sent: hexadeximal.")
                                    if service_requested & constants.MASK_CODE == constants.CODE_DEC:
                                        comment.reply(functions.stod(target))
                                        print("reply sent: decimal.")
                                    if service_requested & constants.MASK_CODE == constants.CODE_ROT13:
                                        comment.reply(functions.rot13(target))
                                        print("reply sent: ROT13.")
                                    if service_requested & constants.MASK_CODE == constants.CODE_UNKNOWN:
                                        comment.reply((constants.err_code_unknown % code) + constants.usage_instructions)
                                        print("reply sent: error message, code unknown.")
                                    cache += comment.id
                                    posts_replied_to.append(comment.id)
                                    replySent = True
                                elif service_requested & constants.MASK_CMD == constants.CMD_DECODE:
                                    if service_requested & constants.MASK_CODE == constants.CODE_BIN:
                                        comment.reply(functions.btos(target))
                                        print("reply sent: text.")
                                    if service_requested & constants.MASK_CODE == constants.CODE_HEX:
                                        comment.reply(functions.xtos(target))
                                        print("reply sent: text.")
                                    if service_requested & constants.MASK_CODE == constants.CODE_DEC:
                                        comment.reply(functions.dtos(target))
                                        print("reply sent: text.")
                                    if service_requested & constants.MASK_CODE == constants.CODE_ROT13:
                                        comment.reply(functions.rot13(target))
                                        print("reply sent: ROT13.")
                                    if service_requested & constants.MASK_CODE == constants.CODE_UNKNOWN:
                                        comment.reply((constants.err_code_unknown % code) + constants.usage_instructions)
                                        print("reply sent: error message, code unknown.")
                                    cache += comment.id
                                    posts_replied_to.append(comment.id)
                                    replySent = True
                                elif service_requested & constants.MASK_CMD == constants.CMD_UNKNOWN:
                                    comment.reply((constants.err_command_unknown % command) + constants.usage_instructions)
                                    print("reply sent: error message, command unknown.")
                                    cache += comment.id
                                    posts_replied_to.append(comment.id)
                                    replySent = True
                                # done
                                
                            if replySent:
                                reply_qty += 1
                            else:
                                print("no reply sent: no valid service requested\n\n")
                        else:
                            print("no reply sent: comment posted by self\n\n")
                    except KeyboardInterrupt:
                        print(reply_qty, "replies sent\n\n")
                        #raise
                    except: # any other exception
                        # time to write to a log for debug
                        print("\a") # auditory notification for bot host
                        print("No reply sent: exception occurred:\n", sys.exc_info()[0])
                        print("Check ./.log/asciicodecbot.log for details.")
                        with open("./.log/asciicodecbot.log", "at") as log:
                            log.write("[%s]\n" % datetime.datetime.fromtimestamp(time.time()))
                            log.write("comment id: %s\n" % comment.id)
                            log.write("body:\n%s\n" % comment.body)
                            log.write(traceback.format_exc())
                            log.write("End of entry\n\n")
                else:
                    print("no reply sent: post is deleted\n\n")
            posts_seen.append(comment.id)
            comment.mark_read()
    except:
        with open("posts_replied_to.txt", "w") as f:
            for post_id in posts_replied_to:
                f.write(post_id + "\n")
        with open("posts_seen.txt", "w") as f:
            for post_id in posts_seen:
                f.write(post_id + "\n")
        raise                           

    with open("posts_replied_to.txt", "w") as f:
        for post_id in posts_replied_to:
            f.write(post_id + "\n")
    with open("posts_seen.txt", "w") as f:
        for post_id in posts_seen:
            f.write(post_id + "\n")
    print("%d replies sent.\n****   Done    ****\n\n\n" % reply_qty)
    #break #only break for manual operation
