#!/usr/bin/env python3
import serial
import time

if __name__ == '__main__':
  serial1 = serial.Serial('/dev/tty.usbmodem14101', 230400, timeout=1)
  # serial2 = serial.Serial('/dev/tty.usbmodem14201', 9600, timeout=1)
  serials = [serial1]
  i = 0
  x = 0
  while i < len(serials):
    serials[i].flush()
    i+=1
  while True:
    tosend = str(x) + '\n';
    j = 0
    while j < len(serials):
      serials[j].write(tosend.encode('utf-8'))
      line = serials[j].readline().decode('utf-8').rstrip()
      print('From {num}: {message}'.format(num=j, message=line))
      j+=1
    time.sleep(0.05)
    x = {True: 0, False: x + 1}[x > 240]
