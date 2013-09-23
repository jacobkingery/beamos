'''
Principles of Engineering, Fall 2013
Lab 1:  DIY LIDAR
Jacob Kingery and Myles Cooper
'''

import serial
import time
import math
import matplotlib.pyplot as plt

def receive():
	#receive data from arduino and plot

	#set up figure
	plt.ion()
	plt.show()
	plt.title('2D Top-Down Representation')  #set title
	plt.xlabel('X (cm)')  #set axes label
	plt.ylabel('Y (cm)')
	plt.axis([-100, 100, -10, 100])  #set axes range
	plt.axes().set_aspect('equal')
	plt.plot(0, 0, 'bo')  #plot origin
	plt.draw()

	colors = ['r.', 'g.', 'm.', 'y.', 'c.']  #different colors for plotting
	clr = 0

	sweep = 0  #keep track of sweep number

	while 1:

		#read data from arduino, split by '@' to get a list of
		#distance and pan angle
		raw_data = ser.readline().strip().split('@')

		dist = float(raw_data[0])  #get calculated distance

		#convert degrees to radians and shift so that the range is 
		#-90 to 90 degrees
		ang = -math.radians(int(raw_data[1]) - 90)

		#convert from polar coordinates to cartesian
		#coordinates and inches to cm
		x = math.sin(ang) * dist * 2.54
		y = math.cos(ang) * dist * 2.54

		#cycle the color and save the figure each sweep
		if  abs(ang) == math.radians(85):
			clr = (clr + 1)*(clr != len(colors) - 1)
			plt.savefig('./Results/test{0}.png'.format(sweep)) 
			sweep+=1

		plt.plot(x, y, colors[clr])  #plot the point
		plt.draw()  #update plot

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
	receive()
	ser.close()