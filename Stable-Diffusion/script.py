import torch
import time
import random
from diffusers import StableDiffusionPipeline

im_name = "image" + str(int(random.random()*1000)) + ".jpg"
print("images\\"+im_name);

model_id = "CompVis/stable-diffusion-v1-4"
device = "cpu"

pipe = StableDiffusionPipeline.from_pretrained(model_id, use_auth_token=True)
pipe = pipe.to(device)

image = pipe(input("Enter Prompt> "), guidance_scale=7.5).images[0]

image.save("images\\"+im_name)