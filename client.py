import socket
import pickle
import json
import time
import threading
import sys

server_addr = ("127.0.0.1", 3000)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
	sock.connect(server_addr)
except ConnectionRefusedError as e:
	print('server not running')
	sys.exit(0)

over = False

def send_data():
	global over, sock
	while not over:

		try:
			sock.send("data".encode('utf-8'))
			time.sleep(1)
		except BrokenPipeError as e:
			over = True
			sock.close()

		except KeyboardInterrupt as e:
			over = True
			sock.close()

		except Exception as e:
			print(type(e), e)

threading.Thread(target=send_data).start()

while not over:
	try:
		data = sock.recv(1024)
		if (data):
			print(json.loads(data.decode('utf-8'))['data'])

	except KeyboardInterrupt as e:
		over = True
		sock.close()

	except Exception as e:
		over = True
		print(e)

sock.close()


