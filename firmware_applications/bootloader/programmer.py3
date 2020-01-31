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
	
	cmdpth=sys.argv[2]
	
	
	
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
		time.sleep(0.05)
		if (serialPort.in_waiting):
			time.sleep(0.05)
			while (serialPort.in_waiting):
				serialPort.read(1)
			break
		
	
	serialPort.write(bytearray([0x01, 0x00, 0x00, 0xff]))
	
	time.sleep(0.05)
	
	while (serialPort.in_waiting==0):
		time.sleep(0.01)
	
	time.sleep(0.05)
	
	txt=serialPort.read(serialPort.in_waiting)
	
	ref="iMCP2.1 B0\n"
	ref=bytes(ref,'utf-8')
	
	if (txt == ref):
		print("Valid Bootloader found.\n")
	else:
		print("\nFAILED TO FIND A VALID BOOTLOADER")
		exit(-1)
	
	
	
	if cmdpth == "STORE":
		filePath=""
		
		if (len(sys.argv) > 3):
			filePath=sys.argv[3]
			try:
				fileHandler=open(filePath, "rb")
			except:
				print("\nFAILED TO OPEN FILE ", filePath)
				exit(-1)
		else:
			print("\nFAILED TO OPEN FILE ", filePath)
			exit(-1)

		time.sleep(0.5)
	
		byte = fileHandler.read(1)
		fileSize = 0
	
		while byte != b"":
			fileSize = fileSize + 1
			byte = fileHandler.read(1)
			
	
		print("Sending binary...")
		serialPort.write(bytearray([0x02, 0x00, 0x00, 0xff]))
		time.sleep(0.05)
		
		while (serialPort.in_waiting==0):
			time.sleep(0.01)
		time.sleep(0.05)
		txt=serialPort.read(serialPort.in_waiting)
		ref="storeSTART\n"
		ref=bytes(ref,'utf-8')
		
		if (txt != ref):
			print("FAIL")
			exit(-1)
		
		sizeString=bytearray( [ fileSize & 0xff , (fileSize>>8) & 0xff , (fileSize>>16) & 0xff , (fileSize>>24) & 0xff ] )
		
		
		serialPort.write(sizeString)
		
		byte = fileHandler.seek(0)
				
		while (serialPort.in_waiting==0):
			time.sleep(0.01)
		time.sleep(0.05)
		txt=serialPort.read(serialPort.in_waiting)
		
		buffer=bytearray()
		size=0
		
		for i in range(fileSize):
			if (size == 64):
				serialPort.write(buffer)
				time.sleep(0.05)
				buffer=bytearray()
				size=0
				while (serialPort.in_waiting==0):
					time.sleep(0.01)
				time.sleep(0.05)
				txt=serialPort.read(serialPort.in_waiting)
				
			byte = fileHandler.read(1)
			buffer=buffer+bytearray(byte)
			size=size+1
				
		aux=bytes(" ",'utf-8')
		while (size < 64 ):
			size=size+1
			buffer=buffer+bytearray(aux)
		
		serialPort.write(buffer)
		time.sleep(0.05)
		
		while (serialPort.in_waiting==0):
			time.sleep(0.01)
		time.sleep(0.05)
		txt=serialPort.read(serialPort.in_waiting)
		
		ref="store,END \n"
		ref=bytes(ref,'utf-8')
		
		fileHandler.close()
		
		if (txt != ref):
			print("FAIL")
			exit(-1)
		
		print("Done")	
	
	elif cmdpth=="BOOT":
		print("Booting...")
		serialPort.write(bytearray([0x05, 0x00, 0x00, 0xff]))
		time.sleep(0.05)
	
	elif cmdpth=="AUTOBOOTon":
		print("Setting the autoboot feature on...")
		
		serialPort.write(bytearray([0x84, 0x00, 0x00, 0xfa]))
		time.sleep(0.05)
		
		while (serialPort.in_waiting==0):
			time.sleep(0.01)
		time.sleep(0.05)
		txt=serialPort.read(serialPort.in_waiting)
		ref="autoboot1 \n"
		ref=bytes(ref,'utf-8')
		
		if (txt != ref):
			print("FAIL")
			exit(-1)
		
		print("Done")
	
	elif cmdpth=="AUTOBOOToff":
		print("Setting the autoboot feature off...")
		
		serialPort.write(bytearray([0x2C, 0x00, 0x00, 0xfa]))
		time.sleep(0.05)
		
		while (serialPort.in_waiting==0):
			time.sleep(0.01)
		time.sleep(0.05)
		txt=serialPort.read(serialPort.in_waiting)
		ref="autoboot0 \n"
		ref=bytes(ref,'utf-8')
		
		if (txt != ref):
			print("FAIL")
			exit(-1)
			
		print("Done")
	
	serialPort.close()
	
	
else:

	print ("\nWelcome to iMCP Bootloader Programmer Version 0.5")
	print("\n#################")
	print ("\nHere are the serial ports available in your computer,")
	print ("please select the one connected to the iMCP board:\n")
	
	ports = list_ports.comports()
	print([port.device for port in ports])
	
	print("\n#################")
	print("\nHow to use this program:")
	print("The first argument is the serial port.")
	print("The second argument is the function:")
	print("\tTo store a new program, type STORE")
	print("\t\tThe third argument is the path to the bin file to be programmed.")
	print("\t\tExample: python3.8 programmer.py3 COM6 STORE thisfile.bin")
	print("\tTo boot, type BOOT")
	print("\t\tExample: python3.8 programmer.py3 COM6 BOOT")
	print("\tTo enable autoboot at startup time, type AUTOBOOTon")
	print("\t\tExample: python3.8 programmer.py3 COM6 AUTOBOOTon")
	print("\tTo disable autoboot at startup time, type AUTOBOOToff")
	print("\t\tExample: python3.8 programmer.py3 COM6 AUTOBOOToff")
	print("\n\n\tNote: mostly any reset but the external reset pin will put the system in the bootloader mode, even with the autoboot feature on. You may use it to force-start the bootloader through your software, using a software-generate reset (in Arm Keil compiler with the command \"NVIC_SystemReset();\").")
