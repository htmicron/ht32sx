#!/bin/python3.8

# Copyright 2020 HT Micron Semicondutores S.A.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
#
# THIS IS A PRELIMINARY PIECE OF SOFTWARE, USE AT YOUR OWN DISCRETION,
# THE FINAL VERSION MAY VARY DRASTICALLY
#
#

import sys
import time
import serial
from serial.tools import list_ports

if len(sys.argv) > 2 :

	serialPortName=sys.argv[1]
	
	#cmdpth=sys.argv[2]
	
	serialPort = 0
	
	try:
		serialPort = serial.Serial(port=serialPortName, baudrate=115200, timeout=0)
		
	except:
		print("\nFAILED TO OPEN PORT ", serialPortName)
		exit(-1)
	
	while (serialPort.in_waiting):
		serialPort.read(1)
		
	#workaround to sync
	while (True):
		serialPort.write(bytearray([0x00]))
		time.sleep(0.01)
		if (serialPort.in_waiting):
			time.sleep(0.01)
			while (serialPort.in_waiting):
				serialPort.read(1)
			break
		
	serialPort.write(bytearray([0x01, 0x00, 0x00, 0xff]))
	
	time.sleep(0.01)
	
	while (serialPort.in_waiting==0):
		time.sleep(0.01)
	
	time.sleep(0.01)
	
	txt=serialPort.read(serialPort.in_waiting)
	ref="iMCP C0\n"
	ref=bytes(ref,'utf-8')
		
	if (txt == ref):
		print("Valid Bootloader found.\n")
	else:
		print("\nFAILED TO FIND A VALID BOOTLOADER")
		exit(-1)
	
	filePath=sys.argv[2]
	try:
		fileHandler=open(filePath, "rb")
	except:
		print("\nFAILED TO OPEN FILE ", filePath)
		exit(-1)


	time.sleep(0.1)

	byte = fileHandler.read(1)
	fileSize = 0

	while byte != b"":
		fileSize = fileSize + 1
		byte = fileHandler.read(1)

	serialPort.write(bytearray([0xf1, 0x00, 0x00, 0xcf]))
	time.sleep(0.01)
	
	while (serialPort.in_waiting==0):
		time.sleep(0.01)
	time.sleep(0.01)
	txt=serialPort.read(serialPort.in_waiting)
	
	time.sleep(0.01)
	
	serialPort.write(bytearray([0x02, 0x00, 0x00, 0xa7]))
	
	time.sleep(0.01)
	
	while (serialPort.in_waiting==0):
		time.sleep(0.01)
	time.sleep(0.01)
	txt=serialPort.read(serialPort.in_waiting)

	time.sleep(0.01)
	
	serialPort.write(bytearray([0xB1, 0x00, 0x00, 0x14]))
	time.sleep(0.01)
	
	while (serialPort.in_waiting==0):
		time.sleep(0.01)
	time.sleep(0.01)
	txt=serialPort.read(serialPort.in_waiting)
	time.sleep(0.01)
	
	print("Sending the firmware...")
	serialPort.write(bytearray([0x02, 0x00, 0x00, 0xff]))
	time.sleep(0.01)
	
	while (serialPort.in_waiting==0):
		time.sleep(0.01)
	time.sleep(0.01)
	txt=serialPort.read(serialPort.in_waiting)
	ref="STORE  \n"
	ref=bytes(ref,'utf-8')
	
	if (txt != ref):
		print("FAIL")
		exit(-1)
	
	sizeString=bytearray( [ fileSize & 0xff , (fileSize>>8) & 0xff , (fileSize>>16) & 0xff , (fileSize>>24) & 0xff ] )
	
	
	serialPort.write(sizeString)
	
	byte = fileHandler.seek(0)
			
	while (serialPort.in_waiting==0):
		time.sleep(0.01)
	time.sleep(0.01)
	txt=serialPort.read(serialPort.in_waiting)
	
	buffer=bytearray()
	size=0
	
	for i in range(fileSize):
		if (size == 64):
			serialPort.write(buffer)
			time.sleep(0.01)
			buffer=bytearray()
			size=0
			while (serialPort.in_waiting==0):
				time.sleep(0.01)
			time.sleep(0.01)
			txt=serialPort.read(serialPort.in_waiting)
			
		byte = fileHandler.read(1)
		buffer=buffer+bytearray(byte)
		size=size+1
			
	aux=bytes(" ",'utf-8')
	while (size < 64 ):
		size=size+1
		buffer=buffer+bytearray(aux)
	
	serialPort.write(buffer)
	time.sleep(0.01)
	
	while (serialPort.in_waiting==0):
		time.sleep(0.01)
	time.sleep(0.01)
	txt=serialPort.read(serialPort.in_waiting)
	
	ref="STOend \n"
	ref=bytes(ref,'utf-8')
	
	fileHandler.close()
	
	if (txt != ref):
		print("FAIL")
		exit(-1)
	
	time.sleep(0.01)
		
	serialPort.write(bytearray([0x04, 0x00, 0x00, 0xcf]))
	time.sleep(0.01)
	
	while (serialPort.in_waiting==0):
		time.sleep(0.01)
	time.sleep(0.01)
	txt=serialPort.read(serialPort.in_waiting)
	ref="LOCKon \n"
	ref=bytes(ref,'utf-8')
	if (txt != ref):
		print("FAIL")
		exit(-1)
		
	time.sleep(0.01)
		
	print("Booting...")
	serialPort.write(bytearray([0x05, 0x00, 0x00, 0xff]))
	time.sleep(0.01)
	
	print("Done")	
	
	serialPort.close()
	
	
else:

	print ("\nWelcome to iMCP Bootloader Easy Programmer Version 1.0")
	print ("\nThis script is only fully compatible with bootloader version B0")
	print("\n#################")
	print ("\nHere are the serial ports available in your computer,")
	print ("please select the one connected to the iMCP board:\n")
	
	ports = list_ports.comports()
	print([port.device for port in ports])
	
	print("\n#################")
	print("\nHow to use this program:")
	print("\tThe first argument is the serial port.")
	print("\tThe second argument is the path to the bin file to be programmed.")
	print("\t\tExample: python3.8 programmer.py3 COM6 thisfile.bin")
	