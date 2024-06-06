import discord
from discord.ext import commands
from discord.utils import get
import random
import asyncio
import requests
import json
from dotenv import load_dotenv
import os

load_dotenv()

intents = discord.Intents.default()
intents.message_content = True
intents.members = True
bot = commands.Bot(command_prefix='!',intents=intents)

flag = os.getenv("FLAG")

guild_id = int(os.getenv("guild_id"))

channel_id = int(os.getenv("channel_id"))

request_id = int(os.getenv("request_id"))

role_id = int(os.getenv("role_id"))

join_message = "Hey ! Seems that you are not in my Discord server :'(\nHere is an invitation for you :)\n%s"

help_message = """
Hey you, here is what you can do here:
- You can `!toss` a coin
- You can `!roll` a dice
- You can ask for a `!cat` or for a `!fox`
- You can use the `!send_secret <username>` to send my secret to you or your friend, if you have the right to ;)
In case of any problem, use the `!send_msg <message>` to send a message to my private request channel on my server. I will check it as soon as possible!"""

def get_cat():
    url = "https://api.thecatapi.com/v1/images/search"
    r = requests.get(url)
    json_r = json.loads(r.text)
    return json_r[0]["url"]

def get_fox():
    url = "https://randomfox.ca/floof/?ref=apilist.fun"
    r = requests.get(url)
    json_r = json.loads(r.text)
    return json_r["image"]
    
@bot.event
async def on_member_join(member):
    if member.guild == bot.get_guild(guild_id):
        channel = bot.get_channel(channel_id)
        guild = member.guild
        role = member.name
        autorize_role = await guild.create_role(name=role)
        overwrites = {
            guild.default_role: discord.PermissionOverwrite(read_messages=False),
            guild.me: discord.PermissionOverwrite(read_messages=True),
            autorize_role: discord.PermissionOverwrite(read_messages=True)
        }
        await guild.create_text_channel(member.name, overwrites=overwrites)
        await member.add_roles(autorize_role)
        await channel.send(f"Hey {member.mention}! Welcome to my server, take a chair and make yourself comfortable 😎")

@bot.event
async def on_message(message):

    guild = bot.get_guild(guild_id)
    if not(message.guild):
        if message.author != bot.user:
            if guild.get_member(message.author.id) is None:
                channel = bot.get_channel(channel_id)
                invite = await channel.create_invite(max_uses=1,unique=True)
                async with message.channel.typing():
                    await asyncio.sleep(5)
                await message.author.send(join_message % invite)
            else:
                async with message.channel.typing():
                    await asyncio.sleep(2)
                await message.author.send("Hey, let's chat on my server :)")
    elif message.guild == guild:
        if message.content.startswith("!"):
            cmd = message.content.split(" ")
            match cmd[0]:
                case "!help":
                    async with message.channel.typing():
                        await asyncio.sleep(5)
                    await message.channel.send(help_message)
                case "!roll":
                    channel = bot.get_channel(channel_id)
                    async with channel.typing():
                        await asyncio.sleep(1)
                    await channel.send(f"🎲 - {message.author.mention} rolls a dice: **{random.choice(['⚀','⚁','⚂','⚃','⚄','⚅'])}**")
                case "!toss":
                    channel = bot.get_channel(channel_id)
                    async with channel.typing():
                        await asyncio.sleep(1)
                    if random.random() < 0.5:
                        await channel.send(f"🪙 - {message.author.mention} tosses a coin: **Head!**")
                    else:
                        await channel.send(f"🪙 - {message.author.mention} tosses a coin: **Tail!**")
                case "!send_secret":
                    role = message.guild.get_role(role_id)
                    if role in message.author.roles:
                        username = " ".join(cmd[1:])
                        member = guild.get_member_named(username)
                        if member:
                            async with member.typing():
                                await asyncio.sleep(5)
                            await member.send("Hey, someone qualified wants to send you my secret, there you go ;)\n" + flag)
                            channel = bot.get_channel(channel_id)
                            async with channel.typing():
                                await asyncio.sleep(2)
                            await channel.send(f"{username} got my secret, congratz!")
                    else:
                        async with message.channel.typing():
                            await asyncio.sleep(3)
                        await message.channel.send("Sorry, you are not allowed to get my secret :'(")
                case "!send_msg":
                    content = ' '.join(cmd[1:])
                    request_channel = bot.get_channel(request_id)
                    await request_channel.send(content)
                    async with message.channel.typing():
                        await asyncio.sleep(1)
                    await message.channel.send("Message received, thanks!")
                case "!cat":
                    channel = bot.get_channel(channel_id)
                    async with channel.typing():
                        await asyncio.sleep(2)
                    await channel.send(f"🐱 - {message.author.mention} wants to send a cat!")
                    await channel.send(get_cat())
                case "!fox":
                    channel = bot.get_channel(channel_id)
                    async with channel.typing():
                        await asyncio.sleep(2)
                    await channel.send(f"🦊 - {message.author.mention} wants to send a fox!")
                    await channel.send(get_fox())
        else:
            if message.author != bot.user:
                async with message.channel.typing():
                    await asyncio.sleep(2)
                await message.channel.send("Hey :)\nCan I `!help` you ?")


bot.run(os.getenv("DISCORD_BOT_KEY"))
