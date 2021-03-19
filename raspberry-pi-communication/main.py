#!/usr/bin/env python3
import serial
import time

x = 0

if __name__ == '__main__':
  ser1 = serial.Serial('/dev/tty.usbmodem14101', 9600, timeout=1)
  ser2 = serial.Serial('/dev/tty.usbmodem14201', 9600, timeout=1)
  ser1.flush()
  ser2.flush()
  while True:
    x = x + 1
    tosend = str(x) + '\n';
    ser1.write(tosend.encode('utf-8'))
    ser2.write(tosend.encode('utf-8'))
    line1 = ser1.readline().decode('utf-8').rstrip()
    print('from one: ' + line1)
    line2 = ser2.readline().decode('utf-8').rstrip()
    print('from two: ', line2)
    time.sleep(0.1)
