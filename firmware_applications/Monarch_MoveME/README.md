## HT32SX - Monarch Move-ME
<div align="center">
  <img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcSesenrhZDRBpVRdUHpQ5ouT6wUTu0t0zaYtSI5GZqXJjGc2tor4Q&s">
</div>

The HT32SX is a System-in-Package device build for the Internet of Things providing a ready-to-use connectivity solution.
The system provides an ARM Cortex M0+ 32bit, the STM S2-LP low power transceiver and an RF Front-End module
combining all the advantages, integration, energy efficiency and convenience of advanced semiconductor packaging
technology into a single chip with a 50Ω RF TX/RX interface.


This application explain how the SigFox™ network can be accessed using the **HT32SX** , that searches the available SigFox™ network 
region via Monarch feature and publishes the found network region after a motion detection. The motion detection is developed with the help of the ST Microelectronics X-NUCLEO-IKS01A2 board (https://www.st.com/en/ecosystems/x-nucleo-iks01a2.html), using an **accelerometer**. The device operates in two modes: 

* Monarch mode: the device scans for available SigFox™ networks through the Monarch feature and stores the network 
configuration it finds for later use. 
* POC mode, the device publishes found SigFox™ network configuration after detecting a valid motion.

The user may press USER BUTTON (blue button) to change the device mode (Monarch mode or POC mode) as many times as desired. 
Press RESET BUTTON (purple button) to revert the device to initial condition.
To detect the SigFox™ network configuration via Monarch feature, press the RESET button (purple button) on the evaluation board and 
press the USER BOTTON (blue button) once to switch to Monarch mode. After detecting a valid SigFox™ network Monarch Beacon (https://build.sigfox.com/monarch), the device will show the SigFox™ network region found as a message on the terminal.

Press the USER button (blue button) again to switch to POC mode. In this mode, the device posts a message to the SigFox™ network after a 
motion detection, which is done by an accelerometer built into the shield mounted on top of the device. The device will publish a message 
on the SigFox™ network containing the found region after a valid motion detection. The USER LED (white LED) indicates the current state 
of the device (OFF: IDLE or ON: BUSY) and will remain ON while the device sends the SigFox™ message indicating that the device is in the 
BUSY state. The user can send a new SigFox™ message after the USER LED (white LED) turn OFF, indicating the IDLE state. While the device 
s in BUSY state (USER LED ON), the user may not change the operating mode.

 
Enjoy! =)
If you need help, please send a message to suppot_iot@htmicron.com.br 

---
