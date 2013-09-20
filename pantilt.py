import serial
import time
import math
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def receive():
	fig = plt.figure()
	ax = fig.add_subplot(111, projection='3d')
	plt.ion()
	plt.show()
	plt.title('3D View')
	ax.set_xlabel('X (cm)')
	ax.set_ylabel('Y (cm)')
	ax.set_zlabel('Z (cm)')
	ax.autoscale(enable=False)
	ax.set_xlim(-100, 100)
	ax.set_ylim(-10, 100)
	ax.set_zlim(-50, 50)
	ax.scatter(0, 0, 0, s=20, c='b')
	plt.draw()
	colors = ['r', 'g', 'm', 'y', 'c']
	clr = 0
	sweep = 0
	trigger = 0
	while 1:

		raw_data = ser.readline().strip().split('@')

		raw_dist = float(raw_data[0])
		w = (raw_dist - 280.0)/120.0
		dist = .62*w**4 - 2.4*w**3 + 3.4*w**2 - 7.3*w + 18

		theta = -math.radians(int(raw_data[1]) - 90)  #convert degrees to radians and shift so that the range is -90 to 90 degrees
		phi = math.radians(int(raw_data[2]) - 90)

		xyDist = math.cos(phi) * dist

		z = math.sin(phi) * dist * 2.54
		x = math.sin(theta) * xyDist * 2.54  #convert from polar coordinates to
		y = math.cos(theta) * xyDist * 2.54  #cartesian coodinates and inches to mm


		if  abs(theta) == math.radians(85):  #cycle the color each sweep
			trigger += 1
			if trigger == 1:
				clr = (clr + 1)*(clr != len(colors) - 1)
				plt.savefig('./Results/test{0}.png'.format(sweep)) 
				sweep += 1
		if  abs(theta) == math.radians(45):
			trigger = 0

		ax.scatter(x, y, z, s=20, c='g', marker='.')  #plot the point
		plt.draw()  #update plot


if __name__ == '__main__':
	ser = serial.Serial('/dev/ttyACM0', 9600)
	time.sleep(1)
	ser.flushInput()
	time.sleep(2)
	print(ser.readline().strip())
	print('Python ready!')
	ser.write("Python ready!")
	receive()
	ser.close()