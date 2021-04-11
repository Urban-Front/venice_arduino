#!/usr/bin/env python3
import serial
import time
import random
import logging

if __name__ == '__main__':
  # logging
  root_logger = logging.getLogger()
  root_logger.setLevel(logging.DEBUG) # or whatever
  handler = logging.FileHandler('logfile.log', 'w', 'utf-8') # or whatever
  formatter = logging.Formatter('%(asctime)s\n%(name)s\n%(levelname)s\n%(message)s\n')
  handler.setFormatter(formatter)
  root_logger.addHandler(handler)
  # To get these from terminal: $ ls /dev/tty.*
  serialStrings = ['/dev/tty.usbmodem14101']
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
      if 'Data' in line:
        returnedNumber = int(line.split('Data: ')[1].split(':')[0])
        if returnedNumber != x:
          root_logger.error(f'Incorrect line received: {returnedNumber}, instead of {x}')
      else:
        root_logger.error(f'Did not receive any data at step {x}')
      print(f'From {j}: {line}')
      j+=1
    time.sleep(0.06)
    # Mod x if we get larger than 420
    x = {True: 0, False: x + 1}[x >= 60 * 5 + 120]
    if not x % 20:
      price = price + random.uniform(-0.1, 0.1)
      price = round(price, 2)
