import turtle
import numpy as np
import cv2
import sys

x, y = sys.argv[1].split('x')
x = int(x)
y = int(y)
name = sys.argv[2]

img = np.ones((y, x))

wn = turtle.Screen()
wn.bgcolor('black')
wn.setup(1000,1000)
wn.tracer(0)

ts = {f'{Y}_{X}' : turtle.Turtle() for X in range(x) for Y in range(y)}

def line(start, end, color='white'):
	t = turtle.Turtle()
	t.pencolor('white')
	t.ht()
	t.up()
	t.goto(start[0], start[1])
	t.down()
	t.goto(end[0], end[1])

d = 30
origin = (-450, 450)

for Y in range(y):
	for X in range(x):
		ts[f'{Y}_{X}'].up()
		ts[f'{Y}_{X}'].goto(origin[0]+X*d+d//2, origin[1]-Y*d-d//2)
		ts[f'{Y}_{X}'].shape('square')
		ts[f'{Y}_{X}'].shapesize(1, 1)
		ts[f'{Y}_{X}'].color('white')
		ts[f'{Y}_{X}'].ht()

for Y in range(y+1):
	line((origin[0], origin[1]-Y*d), (origin[0]+x*d, origin[1]-Y*d))

for X in range(x+1):
	line((origin[0]+X*d, origin[1]), (origin[0]+X*d, origin[1]-y*d))

def do(x, y):
	c, r = (x-origin[0])//d, (origin[1]-y)//d
	c = int(c)
	r = int(r)
	ts[f'{r}_{c}'].st()
	img[r, c] = 0

def save():
	global img
	img_ = img.copy()
	img_.reshape(img.shape+(1,))
	cv2.imwrite(f"{name}.png", img_*255)
	string = f"{img_.shape[1]} {img_.shape[0]}\n"
	for y in range(img_.shape[0]):
		for x in range(img_.shape[1]):
			string += str(1-int(img_[y][x])) + ' '
		string += "\n"
	with open(f"{name}.txt", 'w') as f:
		f.write(string)

wn.listen()
wn.onclick(do)
wn.onkeypress(save, 's')

while 1:
	wn.update()



