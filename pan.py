import serial
import time
import math
import matplotlib.pyplot as plt

def receive():
	plt.ion()
	plt.show()
	plt.xlabel('X')
	plt.ylabel('Y')
	plt.axis([-100, 100, -100, 100])
	plt.plot(0, 0, 'bo')
	plt.draw()
	colors = ['r.', 'g.', 'm.', 'y.', 'c.']
	clr = 0
	while 1:

		raw_data = ser.readline().strip().split('@')

		raw_dist = float(raw_data[0])
		w = (raw_dist - 280.0)/120.0
		dist = .62*w**4 - 2.4*w**3 + 3.4*w**2 - 7.3*w + 18

		ang = -math.radians(int(raw_data[1]) - 90)  #convert degrees to radians and shift so that the range is -90 to 90 degrees

		x = math.sin(ang) * dist  #convert from polar coordinates
		y = math.cos(ang) * dist  #to cartesian coodinates

		if  abs(ang) == math.radians(85):  #cycle the color each sweep
			clr = (clr + 1)*(clr != len(colors) - 1)

		plt.plot(x, y, colors[clr])  #plot the point
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