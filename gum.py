import numpy as np
import cv2

image = cv2.imread("yo.png")

roi = (400, 150, 1000, 700)

cv2.namedWindow('Trackbar')
cv2.resizeWindow('Trackbar', 700, 300)

cv2.createTrackbar('h', 'Trackbar', 0, 179, lambda x : x)
cv2.createTrackbar('H', 'Trackbar', 0, 179, lambda x : x)
cv2.createTrackbar('s', 'Trackbar', 0, 255, lambda x : x)
cv2.createTrackbar('S', 'Trackbar', 0, 255, lambda x : x)
cv2.createTrackbar('v', 'Trackbar', 0, 255, lambda x : x)
cv2.createTrackbar('V', 'Trackbar', 0, 255, lambda x : x)

def getHSVrange(image):
	for r in range(10):
		print("test")
		
	hsv = cv2.cvtColor(image, cv2.COLOR_RGB2HSV)
	#cv2.imshow('hsv', hsv)
	h = cv2.getTrackbarPos('h', "Trackbar")
	H = cv2.getTrackbarPos('H', "Trackbar")
	s = cv2.getTrackbarPos('s', "Trackbar")
	S = cv2.getTrackbarPos('S', "Trackbar")
	v = cv2.getTrackbarPos('v', "Trackbar")
	V = cv2.getTrackbarPos('V', "Trackbar")

	print(h, s, v, '\t', H, S, V)
	lower, upper = np.array([h, s, v]), np.array([H, S, V])
	mask = cv2.inRange(hsv, lower, upper)

	temp = cv2.resize(mask, (mask.shape[1]//10, mask.shape[0]//10))
	cv2.imshow('masked', temp)
	return lower, upper

things = [
	lambda x : x[roi[1]:roi[1]+roi[3], roi[0]:roi[0]+roi[2]],
	lambda x : cv2.GaussianBlur(x, (7, 7), 0),
	lambda x : cv2.dilate(x, kernel=np.ones((5, 5), dtype=np.uint8), iterations=3)
]

def process(frame, things):
	x = frame.copy()
	for f in things:
		x = f(x)
	return x

try:
	while cv2.waitKey(1) != ord('q'):
		lower, upper = getHSVrange(image)

except KeyboardInterrupt:
	print('\n'*3)

exit()



