## HT32SX - Monarch Move-ME

<div align="center">
  <img src="eval_accel.png" width="273" height="427" >
</div>

This application explain how the SigFox™ network can be accessed using the **HT32SX** , that searches the available SigFox™ network 
region via Monarch feature and publishes the found network region after a motion detection. The motion detection is developed with the help of the ST Microelectronics [X-NUCLEO-IKS01A2 board](https://www.st.com/en/ecosystems/x-nucleo-iks01a2.html), using an **accelerometer**. 

This application was shown on Sigfox CONNECT 2019 to show our product capability and allow users to experience it. 

## Device modes 

The application implements two main device modes: 

* **Monarch mode**: Device will search and show via terminal found SigFox network beacon;

* **Move-me mode**: Device will send a SigFox message directly to region RC4 (check further sections to change it) containing found region in Monarch mode after enough motion is applied to the device.

## Requisites

Necessary itens to run this example application:

* HT Micron's iMCP evaluation board;
* STMicroelectronic's X-NUCLEO-IKS01A2 expansion board;
* A 826 - 928 MHz male SMA RF antennae; 
* A mini USB cable;
* [Keil uVision IDE](https://www.keil.com/download/product/).
* (Optional) A PC with terminal to check outputs from device;


## Terminal configuration

Note that every device operation and status is outputed via serial interface. User may use a terminal with following configurations to check system prints:

* Baud rate: 115200 bps;
* Data bits: 8 bits;
* Stop bits: 1 bit;
* Parity: none;
* Flow control: none.

Recomended terminal application is [Termite](https://www.compuphase.com/software_termite.htm), with "Received text" option checked as "monospaced".

## Basic hardware explanation

In order to use this application, please mount the X-NUCLEO-IKS01A2 expansion board on top of the HT32SX_DK evaluation board, respecting the header's connections.

The device have three main user hardware inputs/outputs:

* **USER BUTTON** - Blue botton located a the top of the HT32SX_DK evaluation board, user can toggle device's operation mode by pressing it;

* **RESET BUTTON** - Purple botton located a the top of the HT32SX_DK evaluation board, user can reset the device by pressing it;

* **USER LED** - White led located at the middle of the HT32SX_DK evaluation board, indicates SigFox message send's status.

## Operating the application

After programming the HT32SX_DK evaluation board, user must press **RESET BUTTON**  to reset device's mode to **Move-me mode**.

When device is in **Move-me mode**, it uses the **USER LED** to indicate its substatus:

* **USER LED** IS **ON**: Device is busy sending a SigFox message;
* **USER LED** IS **OFF**: Device is in idle state waiting enough motion input to send a SigFox message;

After enough motion is applied to device, it will send a SigFox message to region RC4 (check further sections to change it). Note that while the device is sending the SigFox message it won't be able to change the operation mode.

when device is in **Move-me mode**, while the **USER LED** is **OFF**, used can press **USER BUTTON** in order to change its operation mode to **Monarch mode**.

In **Monarch Mode**, the device will automatically search for SigFox network beacons and print via serial the detected region. After the region detection, the device will wait until user toggle its operation mode by pressing **USER BUTTON**.

Note that in this application the Monarch feature is used only to detect network's region configuration, and will **NOT** use this information in **Move-me mode**. Please check our [Monarch_Scan](https://github.com/htmicron/ht32sx/tree/master/firmware_applications/Monarch_Scan/Sigfox) example application for a more complete Monarch feature exploit (including device automatic region rconfiguration).

## Changing region configuration

This application sends a SigFox message to the region configured by user in compile time. User may change the desired region by changing "RCZ_REGION" variable in "main.h" header.

Enjoy! =)
If you need help, please send a message to suppot_iot@htmicron.com.br 
