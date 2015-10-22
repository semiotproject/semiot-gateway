#import urllib.request
#for num in range(0,500):
#	urllib.request.urlopen("http://hiroakis.com:8888/api?id=1&value="+str(num)).read()

from websocket import create_connection
ws = create_connection("ws://localhost:8888/ws")
print("Sending 'Hello, World'...")
ws.send("Hello, World")
print("Sent")
print("Reeiving...")
result = ws.recv()
print("Received '%s'" % result)
ws.close()
