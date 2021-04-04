#!/usr/bin/env python3
import serial
import time
import random

if __name__ == '__main__':
  # To get these from terminal: $ ls /dev/tty.*
  serialStrings = ['/dev/tty.usbmodem14101', '/dev/tty.usbmodem14201']
  serials = []
  for serialString in serialStrings:
    serials.append(serial.Serial(serialString, 230400, timeout=1))
  # serial2 = serial.Serial('/dev/tty.usbmodem14201', 9600, timeout=1)
  x = 0
  price = 27.12
  for serialPort in serials:
    serialPort.flush()
  while True:
    tosend = str(x) + ':' + str(price) + '\n';
    j = 0
    for serialPort in serials:
      serialPort.write(tosend.encode('utf-8'))
      line = serialPort.readline().decode('utf-8').rstrip()
      print('From {num}: {message}'.format(num=j, message=line))
      j+=1
    time.sleep(0.05)
    # Mod x if we get larger than 240
    # This no longer works, bc we are looping counter in arduino
    # around pixelsInMessage + 24 + w...
    x = {True: 0, False: x + 1}[x >= 60 * 5 + 120]
    # x = x + 1
    if not x % 20:
      price = price + random.uniform(-0.1, 0.1)
      price = round(price, 2)
