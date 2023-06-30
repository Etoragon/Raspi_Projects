import bluetooth
import struct
import cv2
import numpy as np

# Set the Bluetooth address of your ESP32
esp32_address_cam1 = 'CC:DB:A7:1D:D3:EE'
esp32_address_cam2 = '0C:B8:15:75:81:72'
esp32_address_cam3 = 'CC:DB:A7:1E:E8:D2'
esp32_address_cam4 = 'CC:DB:A7:1E:E3:2A'

addresses = [esp32_address_cam1,esp32_address_cam2, esp32_address_cam3, esp32_address_cam4]
sockList = []
for address in addresses:
	try:
		sockList.append(bluetooth.BluetoothSocket(bluetooth.RFCOMM))
		sockList[-1].connect((address, 1))
	except:
		sockList.pop()
		pass
#sock1 = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
#sock1.connect((esp32_address_cam1, 1))

#sock2 = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
#sock2.connect((esp32_address_cam2, 1))

#sock3 = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
#sock3.connect((esp32_address_cam3, 1))

#sock4 = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
#sock4.connect((esp32_address_cam4, 1))

#sockList = [sock1, sock2, sock3, sock4]
print("here1")
print("Num of connected cams: " + str(len(sockList)))
while True:
	image_list = []
	for sock in sockList:
		sock.send(b'\x01')  # 0 is the correct request flag

		size_data = sock.recv(4)
		img_size = struct.unpack('I', size_data)[0]
		print(img_size)

		image_data = bytearray()
		bytes_received = 0

		while bytes_received < img_size:
			chunk = sock.recv(img_size)
			if not chunk:
				break
			image_data.extend(chunk)
			bytes_received += len(chunk)

		if bytes_received != img_size:
			continue

		# Decode the image data directly
		image = cv2.imdecode(np.frombuffer(image_data, dtype=np.uint8), cv2.IMREAD_COLOR)
		image_list.append(image)
	display_frame = cv2.hconcat(image_list)
	cv2.imshow('image', display_frame)

	if cv2.waitKey(1) & 0xFF == ord('q'):
		break

cv2.destroyAllWindows()
