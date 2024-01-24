import os
import torch
import cv2
import numpy as np
from matplotlib import pyplot as plt

midas = torch.hub.load("intel-isl/MiDaS", "MiDaS_small")
midas.to("cpu")
midas.eval()

transforms = torch.hub.load("intel-isl/MiDaS", "transforms")
transform = transforms.small_transform

cap = cv2.VideoCapture(0)

while cv2.waitKey(1) != ord('q'):
    ret, frame = cap.read()

    image_numpy = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)[50:-50, 50:-50, :]
    image = transform(image_numpy).to('cpu')
    with torch.no_grad():
        pred = midas(image)
        pred = torch.nn.functional.interpolate(
            pred.unsqueeze(1),
            size = image_numpy.shape[:2],
            mode = 'bicubic',
            align_corners = False
        ).squeeze()
        output = pred.cpu().numpy()
        plt.imshow(output, cmap="bone")
        plt.pause(0.001)
    cv2.imshow("original", frame)

