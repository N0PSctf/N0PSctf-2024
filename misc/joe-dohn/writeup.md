## Joe Dohn

### Description

Is it me or there is a strange guy on the discord server ?

**Author: algorab**
### Solution

Once we have reached the bot, it sends us and invitation to its Discord server.

On it, we only can interact with the bot through a dedicated and personal channel. The help message of the bot gives us these commands:
```
Hey you, here is what you can do here:

- You can !toss a coin
- You can !roll a dice
- You can ask for a !cat or for a !fox
- You can use the !send_secret <username> to send my secret to you or your friend, if you have the right to ;)

In case of any problem, use the !send_msg <message> to send a message to my private request channel on my server. I will check it as soon as possible!
```

When requesting for the flag directly, the bot answers that we don't have sufficient permissions. The other commands produce a somehow funny output in the general channel.

We can guess that the only account on the server allowed to send the flag is Joe Dohn himself, as he is the only one with the `Admin` role.

So we have to find a way to make him execute the command `!send_secret <our_discord_account>`. 

By reading the description of the `!send_msg` command, we can deduce that our message will be sent to a channel on the server that we cannot see. Which means that if the bot is not programmed to ignore message coming from himself, and if message is not preprocessed before being sent, we could except this behaviour to happen:
```
> input: !send_msg !send_secret <username>
> bot writes in the requests channel: !send_secret <username>
> bot sees that command !send_secret has been used by an admin, and therefore executes it.
```

By doing so, we receive the flag.
### Flag

`N0PS{d15c0Rd_c0mM4nD_1nJ3c710n}`