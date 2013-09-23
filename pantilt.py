'''
Principles of Engineering, Fall 2013
Lab 1:  DIY LIDAR
Jacob Kingery and Myles Cooper
'''

import serial
import time
import math
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def receive():
	#receive data from arduino and plot

	#set up figure
	fig = plt.figure()
	ax = fig.add_subplot(111, projection='3d')
	plt.title('3D View')  #set title
	ax.set_xlabel('X (cm)')  #set axes labels
	ax.set_ylabel('Y (cm)')
	ax.set_zlabel('Z (cm)')
	ax.autoscale(enable=False)
	ax.set_xlim(-100, 100)  #set axes ranges
	ax.set_ylim(-10, 100)
	ax.set_zlim(-50, 50)
	ax.scatter(0, 0, 0, s=30, c='b')  #plot origin

	trigger = -1  #trigger to stop after one sweep

	#initialize lists for coordinates
	x = []
	y = []
	z = []

	while trigger != 1:

		#read data from arduino, split by '@' to get a list of
		#distance, pan angle, and tilt angle
		raw_data = ser.readline().strip().split('@')  

		dist = float(raw_data[0])  #get calculated distance

		#convert degrees to radians and shift so that the range is 
		#-90 to 90 degrees
		theta = -math.radians(int(raw_data[1]) - 90)
		phi = math.radians(int(raw_data[2]) - 90)

		xyDist = math.cos(phi) * dist  #find distance in xy plane

		#convert from spherical coordinates to cartesian
		#coordinates and inches to cm
		z.append(math.sin(phi) * dist * 2.54)
		x.append(math.sin(theta) * xyDist * 2.54)
		y.append(math.cos(theta) * xyDist * 2.54)

		#check for when it has completed a full sweep
		if  abs(theta)==math.radians(70) and trigger==0:
			trigger += 1
		if abs(theta) == 0:
			trigger = 0

	print('Done receiving')

	#close the serial connection and open a new one so that the arduino resets
	ser.close()
	ser2 = serial.Serial('/dev/ttyACM0', 9600)
	ser2.close()
	#plot data
	ax.scatter(x, y, z, s=20, c='r', marker='o')
	plt.show()


def establishContact():
	#flush buffer and wait for ready signal from arduino, 
	#then send own ready signal
	time.sleep(.5)
	ser.flushInput()
	time.sleep(1)
	print(ser.readline().strip())
	print('Python ready!')
	ser.write("Python ready!")	


if __name__ == '__main__':
	ser = serial.Serial('/dev/ttyACM0', 9600)  #open serial connection
	establishContact()
	print('Contact established, receiving data...')
	receive()