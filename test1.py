import warnings
warnings.filterwarnings("ignore", category=DeprecationWarning)

import os
import time

import cv2
import numpy as np
import torch
import spacy
from transformers import CLIPSegProcessor, CLIPSegForImageSegmentation

class test():

    def __init__(self, prompt):

        self.processor = CLIPSegProcessor.from_pretrained("CIDAS/clipseg-rd64-refined")
        self.model = CLIPSegForImageSegmentation.from_pretrained("CIDAS/clipseg-rd64-refined")
        
        self.nlp_model = spacy.load("en_core_web_sm")
        self.doc = self.nlp_model(prompt)

        self.objects = []
        self.adjectives = []

        for token in self.doc:
            if "obj" in token.dep_:
                self.objects.append(token.text)
            if "amod" == token.dep_:
                self.adjectives.append(token.text)

        self.prompt = ''.join(self.adjectives + [" "] + self.objects)
        print(self.adjectives, self.objects)

    def process_frame(self, frame):

        inputs = self.processor(text=self.prompt, images=frame, padding="max_length", return_tensors="pt")
        with torch.no_grad():
            output = self.model(**inputs)

        output = torch.sigmoid(output[0]).numpy()
        cv2.imshow(self.prompt, cv2.resize(output, (frame.shape[1], frame.shape[0])))

    def run(self):

        cap = cv2.VideoCapture(0)

        while cv2.waitKey(1) != ord('q'):

            _, frame = cap.read()
            self.process_frame(frame)            
            cv2.imshow("frame", frame)

if __name__ == "__main__":

    test("find the tall person").run()


