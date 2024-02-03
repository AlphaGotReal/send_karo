import os
import time

import discord
from discord.ext import commands
import json

import torch
from models import DiviModel, TrainDivi

divi = DiviModel(768)
divi.load_state_dict(torch.load(f"{os.sys.argv[1]}.pth"))

token = json.load(open("config.json", 'r'))['token']
intents = discord.Intents.default()
intents.messages = True
intents.message_content = True

bot = commands.Bot(command_prefix=">", intents=intents)

@bot.event
async def on_ready():
    print(f"bot is ready")

@bot.command(name="run")
async def on_message(ctx, *messages):
    sentence = ""
    for m in messages:
        sentence += " " + m
    tensors = divi(sentence)
    velocities = torch.normal(mean=tensors[0], std=tensors[1])[0]
    v, w = velocities[0], velocities[1]
    await ctx.send(f"linear: {v}\nangular: {w}")

@bot.command(name="init")
async def reset(ctx, model_version):
    global divi 

    divi = DiviModel(768)
    divi.load_state_dict(torch.load(f"{model_version}.pth"))

    await ctx.send("model is initialised")

bot.run(token)

