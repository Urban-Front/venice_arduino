#!/usr/bin/env python3
import serial
import time
import random

if __name__ == '__main__':
  # To get these from terminal: $ ls /dev/tty.*
  serialStrings = ['/dev/ttyACM0', '/dev/ttyACM1', '/dev/ttyACM2', '/dev/ttyACM3']
  serials = []
  for serialString in serialStrings:
    serials.append(serial.Serial(serialString, 230400, timeout=1))
  x = 0
  price = 27.12
  for serialPort in serials:
    serialPort.flush()
  while True:
    tosend = f'{x}: {price:.2f}\n'
    j = 0
    for serialPort in serials:
      serialPort.write(tosend.encode('utf-8'))
      line = serialPort.readline().decode('utf-8').rstrip()
      print(f'From {j}: {line}')
      j+=1
    time.sleep(0.0666)
    # Mod x if we get larger than max int size for arduino
    x = {True: 0, False: x + 1}[x >= 32767]
    if not x % 20:
      price = price + random.uniform(-0.1, 0.1)
      price = round(price, 2)
