# Frequently Asked Questions about HT32SX 

<div align="center">
  <img src="https://github.com/htmicron/ht32sx/blob/images/iMCP_reto.jpg">
</div>

## Questions list:

1. [**How do I register a new user at SigFox Backend?**](#regbackend) <br/>
2. [**How do I create a new group at Sigfox Backend?**](#newgroup) <br/>
3. [**How do I create a new Device Type at Sigfox Backend?**](#newdevicetype) <br/>
4. [**How do I register a new device at SigFox Backend?**](#regdevice) <br/>
5. [**How or where can I check if my device is sending the messages correctly?**](#msgcorrec) <br/>
6. [**How should I set up my new device to operate in my current region?**](#configrc) <br/>
7. [**How many messagens can I send daily?**](#dailymsg) <br/>
8. [**What is the Keil uVision setup to use debugger tools?**](#debugpurpose) <br/>
9. [**How to open the examples using STM32CubeIDE?**](#cudeide1) <br/>
10. [**Error: Invalid message sequence from Device #XXXXX: expected X, actual X**](#eventmsg) <br/>
11. [**What softwares do I need to install to run the GitHub Examples?**](#setup) <br/>
12. [**I'm sending messages using one of GitHub examples but I can't see it at backend. What can I do?**](#senderror) <br/>
13. [**Is there any PCB design example?**](#pcbexamples) <br/>
14. [**Why my device is stuck in TX?**](#stucktx) <br/>
15. [**How can I flash a new firmware to my HT32SX?**](#flashcode) <br/>
16. [**I need to use EEPROM in my application. From which address may I start writing?**](#eepromwrite) <br/>
17. [**How can I increase HT32SX output power?**](#outputpower) <br/>
18. [**I received new Sigfox credentials. How should I flash it to my device?**](#credbin) <br/>


## Answered Questions:

<a name="regbackend"></a>
### 1. How do I register a new user on the SigFox Backend?

<hr>

> • **If you do not have an account and have just received an Evaluation Board:**

> 1. Go to [**SigFox Buy**](https://buy.sigfox.com/activate) and select your country. <br/>
> 2. If you do not know the device ID and PAC, open a serial terminal and run one of our [**GitHub**](https://github.com/htmicron/ht32sx) examples. Your ID and PAC number should be shown like the image below:

<div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/1sW1ahpsqRUkARdzv-rliSx3C6Zm96upo?alt=media&key=AIzaSyA3IM3FTM-B4ibyKf1dUPBdbmKgAN9LX-s">
</div>

> 3. Fill all spaces and an account shall be created with your device registered on it.

<hr>

> • **On the other hand, if you already have an account:** <br/> 

> 1. Go to [**SigFox Backend**](https://backend.sigfox.com/), access your account and click _**User**_:

<div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/1C9ppohn34-PME5Q0Li54-l6j_L2UPp52?alt=media&key=AIzaSyA3IM3FTM-B4ibyKf1dUPBdbmKgAN9LX-s">
</div>

> 2. Click _**New**_, fill all spaces, select the group which this new user will participate and select the permissions. The following permissions are recommended for admin access:

<div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/1koGiXAiHDy8k0T2mRYA1Ce60rI99NUrU?alt=media&key=AIzaSyA3IM3FTM-B4ibyKf1dUPBdbmKgAN9LX-s">
</div>

<hr>

<br/>

<a name="newgroup"></a>
### 2. How do I create a new group at Sigfox Backend?

<hr>

> 1. Go to [**SigFox Backend**](https://backend.sigfox.com/), access your account and click on _**Group**_:

<div align="center">
  <img src="https://github.com/htmicron/ht32sx/blob/faq/.github/images/group.png">
</div>

> 2. Click on _**New**_ and select a parent group. Example:

<div align="center">
  <img src="https://github.com/htmicron/ht32sx/blob/faq/.github/images/parentgroup.png">
</div>

> 3. Fill all spaces, select your timezone and a new group shall be created.

<hr>

<br/>

<a name="newdevicetype"></a>
### 3. How do I create a new Device Type at Sigfox Backend?

<hr>

> 1. Open [**SigFox Backend**](https://backend.sigfox.com/) and access your account.
> 2. Go to _**Device Type**_ -> _**New**_ and select the group which this new type will be associated.
> 3. Fill _**Name**_ and _**Description**_ fields. 
> 4. Then click on the white space of _**Contracts**_ field and select one of your Sigfox Contracts. Example:

<div align="center">
  <img src="https://github.com/htmicron/ht32sx/blob/faq/.github/images/contract.png">
</div>

> 4. If you do not see any contract after clicking on the white field, check if the associated group still has available tokens or contact your Sigfox provider to ask for a new contract.

<hr>

<br/>

<a name="regdevice"></a>
### 4. How do I register a new device at SigFox Backend?
<hr>

> If you already have an account, check the following steps below, otherwise have a look at [**Account Registration**](#regbackend) topic.<br/> 
> Every device must be associeted with a group and a device type. So before you start, check if your environment is ready, or go to [**Question 2**](#newgroup) and [**Question 3**](#newdevicetype). You may have as many registered devices in your group or device type as your contract allow.

> 1. Open the [**SigFox Backend**](https://backend.sigfox.com/) website. <br/>
> 2. Click _**Device**_:

<div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/1-u-sz7Adi7hbe048oZzAcpSq2Da_A5fk?alt=media&key=AIzaSyBazifvhYWroBBnEocbFpLG0mOjfUfelE4">
</div>

> 3. Then click _**New**_ to register a new device:

<div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/1p1YuczQEaY9rl7sMq3Bg4l-FvM9OVs5w?alt=media&key=AIzaSyBazifvhYWroBBnEocbFpLG0mOjfUfelE4">
</div>

> 4. Select the group which your device will be part of:

<div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/1ZaWI1HwtjCZa_cMn_b2HwYKeQ6LvDJ69?alt=media&key=AIzaSyD0vizVRI4gcudGwFjpVAbRKDtjUcs-KDk">
</div>

> 5. Fill the underlined fields with your device ID & PAC and choose a name for this new device:

<div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/1T6f-IWvKywE9MNkR91_4jx9xSE6CpOZ3?alt=media&key=AIzaSyBazifvhYWroBBnEocbFpLG0mOjfUfelE4">
</div>

> 6. Finally, click _**Ok**_ and your device will be registered.

<hr>
<br/>

<a name="msgcorrec"></a>
### 5. How or where can I check if my device is sending the messages correctly?
<hr>

> If you do not have an account on [**SigFox Backend**](https://backend.sigfox.com/) or haven't registered your device yet, check the [**Account Registration**](#regbackend) or [**Device Registration**](#regdevice) topics. 
> If It's already done, check the following steps: <br/> 

> 1. Open [**SigFox Backend**](https://backend.sigfox.com/) website, where all your messages will be shown. <br/>
> 2. Click _**Device**_:

<div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/1-u-sz7Adi7hbe048oZzAcpSq2Da_A5fk?alt=media&key=AIzaSyBazifvhYWroBBnEocbFpLG0mOjfUfelE4">
</div>

> 3. Search for your device ID and click on it:

<div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/15KFeZQS8hMDV8PTi6fLRsBA-udrvvj5a?alt=media&key=AIzaSyBazifvhYWroBBnEocbFpLG0mOjfUfelE4">
</div>

> 6. Then click _**Messages**_ and all messages received from the respective device shall be shown:

<div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/1KWfBygD_fq58IR_YOKTo_jCsC3yM6T65?alt=media&key=AIzaSyBazifvhYWroBBnEocbFpLG0mOjfUfelE4">
</div>

<hr>
<br/>

<a name="configrc"></a>
### 6. How should I set up my new device to operate in my current region?
<hr>

> 1. Open the [**Generic_Push_Button V2.2**](https://github.com/htmicron/ht32sx/tree/master_2/Generic_Push_Button) example, if your are using the V2.2 hardware, or [**Generic_Push_Button V2.1**](https://github.com/htmicron/ht32sx/tree/master/applications/Generic_Push_Button), if you are using an [**Evaluation Board**](https://github.com/htmicron/ht32sx/blob/documents/2.1/Evaluation%20Board%20User%20Guide%20v0.1.pdf). <br/>
> 2. Open the _**main.c**_ file and search for the _**main**_ function. <br/>
> 3. Change _**configRegion**_ parameter and select the region where you are currently working on:

<div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/1ZtC7WG6ENod6rMT0C7E5UGqXAQTPLlW4?alt=media&key=AIzaSyBazifvhYWroBBnEocbFpLG0mOjfUfelE4">
</div>

<hr>
<br/>

<a name="dailymsg"></a>
### 7. How many messagens can I send daily?
<hr>

> Therefore, a device can send a maximum of 6 messages per hour (36/6), which means a total of 144 messages per day (24 * 6). Sigfox keeps 4 messages for protocol use, which therefore allows for 140 messages per day for your device.

> It is recommended to use the [**SigFox Dongle**](https://build.sigfox.com/sdr-dongle) for debbuging purposes (when user needs to send too many messages daily) and the Backend only to confirm your tests.

> More information can be found [**here**](https://build.sigfox.com/study).

<hr>
<br/>

<a name="debugpurpose"></a>
### 8. What is the Keil uVision setup to use debugger tools?

> 1. Open Keil uVision and go to _**Project -> Options for target -> Device**_ and select _**STM32LO52T8Yx**_:

<div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/1CyPrCSclun1YW_VBQcrAM6IYykpqHIce?alt=media&key=AIzaSyA3IM3FTM-B4ibyKf1dUPBdbmKgAN9LX-s">
</div>

> 2. Go to _**Debug**_ and select _**ST-Link Debugger**_ option:

<div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/1YIbnLN35FnBYIa9U2TBVhliwDIrLz-0T?alt=media&key=AIzaSyA3IM3FTM-B4ibyKf1dUPBdbmKgAN9LX-s">
</div>

> 3. Click _**Settings**_ and change the _**Connect**_ option to connect _**with Pre-reset**_.

<div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/1WRr57TvcUAmXhrc9mQv3FFUcGAjK7Hmu?alt=media&key=AIzaSyA3IM3FTM-B4ibyKf1dUPBdbmKgAN9LX-s">
</div>

> 4. Click _**Flash Download**_  tab and check if the HT32SX MCU (STM32LO) is already listed.

<div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/1hzXU6mNMWJryagcFwDXi-TOgRmXiovdD?alt=media&key=AIzaSyA3IM3FTM-B4ibyKf1dUPBdbmKgAN9LX-s">
</div>

 > Otherwise click _**Add**_ and select the right MCU:
 
 <div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/1Hrc5Lf4xckgbvH3_DC69MxJQeOSLOaT5?alt=media&key=AIzaSyA3IM3FTM-B4ibyKf1dUPBdbmKgAN9LX-s">
</div>

<hr>
<br/>

<a name="cudeide1"></a>
### 9. How to open the examples using STM32CubeIDE?
<hr>

> 1. Install [**STM32CubeIDE**](https://www.st.com/en/development-tools/stm32cubeide.html). <br/>
> 2. Clone the [**MASTER BRANCH V2.1**](https://github.com/htmicron/ht32sx/tree/master) or [**MASTER BRANCH V2.2**](https://github.com/htmicron/ht32sx/tree/master_2) of our [**GitHub Repository**](https://github.com/htmicron/ht32sx). <br/>
> 3. Open the directory where you cloned the repository and select the example which you want to use. <br/>
> 4. Open _**STM32CubeIDE**_ directory and double-click the _**cproject**_ file:

 <div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/1UZJkxDV7iRGEXEl9ZqBIAL0a-mfV9yTL?alt=media&key=AIzaSyA3IM3FTM-B4ibyKf1dUPBdbmKgAN9LX-s">
</div>

> 5. To open the STM32CubeMX graphical interface, double-click the _**ioc**_ file:

 <div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/14zD8fo-CUjGpl5tLffT9e6Tr93C_1K_w?alt=media&key=AIzaSyA3IM3FTM-B4ibyKf1dUPBdbmKgAN9LX-s">
</div>

<hr>
<br/>

<a name="eventmsg"></a>
### 10. Error: Invalid message sequence from Device #XXXXX: expected X, actual X
<hr>

> This is a message counter error. Both sides must have the same sequence number, otherwise all message will be rejected by the Backend side. <br/>
> A simple way to fix it, is disengaging the sequence number. <br/> 

> 1. Open [**SigFox Backend**](https://backend.sigfox.com/) website. <br/>
> 2. Click _**Device**_ tab: 

 <div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/1-u-sz7Adi7hbe048oZzAcpSq2Da_A5fk?alt=media&key=AIzaSyBazifvhYWroBBnEocbFpLG0mOjfUfelE4">
</div>

> 3. Look for your device ID and click on it:

 <div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/15KFeZQS8hMDV8PTi6fLRsBA-udrvvj5a?alt=media&key=AIzaSyBazifvhYWroBBnEocbFpLG0mOjfUfelE4">
</div>

> 4. Then click _**Disengage sequence number**_ and the counter will be reseted:

 <div align="center">
  <img src="https://www.googleapis.com/drive/v3/files/16l-_Zsxrbaaxs5n-2tsofN4xaHMi5Jjr?alt=media&key=AIzaSyBazifvhYWroBBnEocbFpLG0mOjfUfelE4">
</div>

<hr>
<br/>

<a name="setup"></a>
### 11. What softwares do I need to install to run the GitHub Examples?
<hr>

> 1. [**STM32CubeIDE**](https://www.st.com/en/development-tools/stm32cubeide.html) or [**Keil uVision**](https://www.keil.com/download/) (needs cortex M0 [**license**](https://www2.keil.com/stmicroelectronics-stm32/mdk)).
> 2. [**Termite**](https://www.compuphase.com/software_termite.htm).
> 3. [**ST-LINK Utility**](https://www.st.com/en/development-tools/stsw-link004.html).
> 4. [**Git**](https://git-scm.com/downloads).

<hr>
<br/>

<a name="senderror"></a>
### 12. I'm sending messages using one of GitHub examples but I can't see it at backend. What can I do?
<hr>

> 1. Check if you are using an antenna.
> 2. Check if you cloned the right firmware version. If you are using an Evaluation Board, you have a 2.1 hardware version, otherwise you have a 2.2 version. [**V2.1 Repository**](https://github.com/htmicron/ht32sx/tree/master) - [**V2.2 Repository**](https://github.com/htmicron/ht32sx/tree/master_2).
> 3. Check if you already configured your device to work on your current RC zone. Have a look at [**4th Question**](#configrc). 
> 4. If the previous step did not solve your problem, try to reset the sequence number. To do that, please have a look at the [**9th Question**](#eventmsg).
> 5. If the previous step did not work, check if you are in a zone covered by Sigfox. Go to [**here**](https://www.sigfox.com/en/coverage) and search for your country.
> 6. If you have a [**SDR Dongle**](https://build.sigfox.com/sdr-dongle), you are able to debug your project using [**Sigfox RSA**](https://build.sigfox.com/sdr-dongle#sigfox-radio-signal-analyzer-rsa) or [**Sigfox Network Emulator**](https://build.sigfox.com/sdr-dongle#sigfox-network-emulator-sne). Extra documentations can be found [**here**](https://support.sigfox.com/products#sdr).

<hr>
<br/>

<a name="pcbexamples"></a>
### 13. Is there any PCB design example?
<hr>

> 1. [**Disk91**](https://github.com/disk91/HTMICRON-iMCP-devkit).
> 2. [**Test Board**](https://www.pcbway.com/project/shareproject/iMCP_HT32SX___Test_Board___SMA.html).

<hr>
<br/>

<a name="stucktx"></a>
### 14. Why my device is stuck in TX?
<hr>

> * If your device is stuck in TX, you probably cloned a wrong firmware version from our GitHub page. <br/>
>   - In case you are using an Evaluation Board, please clone [**this**](https://github.com/htmicron/ht32sx/tree/master) branch.
>   - Otherwise, close [**this**](https://github.com/htmicron/ht32sx/tree/master_2) one.

<hr>
<br/>

<a name="flashcode"></a>
### 15. How can I flash a new firmware to my HT32SX?
<hr>

> There are two options for this:

> 1. Flash a new code through SWD interface.

> * To use SWD protocol, user must have a ST-LINK Debugger. ST-LINK examples:
>> 1. [**STLINK/V2**](https://www.st.com/en/development-tools/st-link-v2.html). 
>> 2. ST-LINK ona STM32 Dev Kit. [**Example**](https://www.st.com/en/evaluation-tools/nucleo-f030r8.html). Check [**this**](https://www.st.com/content/ccc/resource/technical/document/user_manual/98/2e/fa/4b/e0/82/43/b7/DM00105823.pdf/files/DM00105823.pdf/jcr:content/translations/en.DM00105823.pdf) document (topic 6.2.4) to learn how to use this option.

> * Once there is a ST-LINK Debugger available:

>> 1. Connect the SWDIO, SWDCLK, RST and GND of the ST-LINK pins to the corresponding HT32SX pins. 
>> 2. Then, open one of the firmware examples available on our [**GitHub**](https://github.com/htmicron/ht32sx) page (using STM32CubeIDE or Keil uVision IDE) and flash it clicking on _**run**_ or _**download**_.

> 2. Flash a new code using UART.

> * A bootloader must be used in this case. 

>> * To use the MCU bootloader: 
>> 1. Put Boot0 pin to VDD.
>> 2. Connect RX - TX USART pins (USART1 or USART2) to a FTDI.
>> 3. Open [**STM32CubeProgrammer**](https://www.st.com/en/development-tools/stm32cubeprog.html).
>> 4. Change the option _**ST-LINK**_ to _**UART**_ and click on _**connect**_.
>> 5. Click on _**Open File**_ and select the right binary file. 
>> 6. Click on _**Download**_ and the new firmware shall be flashed. 

> * To use a to use a proprietary bootloader:

>> 1. Click [**here**](https://github.com/htmicron/ht32sx/tree/master_2/Bootloader_Refresh) to see an example showing how a proprietary bootloader could worked in the HT32SX context. More info could be found at the same repository.

<hr>
<br/>

<a name="eepromwrite"></a>
### 16. I need to use EEPROM in my application. From which address may I start writing?
<hr>

> Sigfox credentials are stored from address 0x08080000 to 0x080800F0, so you can start writing from 0x08080100.

<hr>
<br/>

<a name="outputpower"></a>
### 17. How can I increase HT32SX output power?
<hr>

> * In this question we will use the [**Generic Push Button**](https://github.com/htmicron/ht32sx/tree/master_2/Generic_Push_Button) firmware as an example. <br/>

> * First of all, you need to be aware that our firmware examples have the same output power setup which was certificated by Sigfox.  Before increase your device output power, please have a look at [**this**](https://build.sigfox.com/sigfox-radio-configurations-rc) document to check the Sigfox recommended EIRP (dBm), related to your current RC zone. <br/>
> **We only recommend this solution, in case of your device be used in a remote place too far away from a base station.** <br/>

> * There are three ways to increase this value: turning on PA, making boost mode enable or increasing the respective output level constant.

>> 1. Turning on PA:
>>> * Use _**HT_API_switchPa(1)**_ to enable PA and _**HT_API_switchPa(0)**_ to disable.
>> 2. Making boost mode enable:
>>> * _**HT_API_setSmpsVoltageAction(7)**_ to enable and _**HT_API_setSmpsVoltageAction(1)**_ to disable.
>> 3. Increasing output level constant:
>>> * In _**main.h**_ file, you will see a couple of defines (RCZ1_OUTPUT_POWER, RCZ2_OUTPUT_POWER ... RCZ7_OUTPUT_POWER) which are responsible to increase or decrease output power. **The maximum accepted value is -32**.

> **Examples:**

> * Turning on PA:

>> 1. Open _**main.c**_ file and to _**configRegion**_ function. There you will see a switch/case routine with all RC zones available.
>> 2. Go to RC1 case and change _**HT_API_switchPa(0)**_ line to _**HT_API_switchPa(1)**_:

<div align="center">
  <img src="https://github.com/htmicron/ht32sx/blob/faq/.github/images/pa_on.PNG">
</div>

> * Making boost mode enable:

>> 1. Open _**main.c**_ file and to _**configRegion**_ function. There you will see a switch/case routine with all RC zones available.
>> 2. Go to RC2 case and add _**HT_API_setSmpsVoltageAction(7)**_ function just after _**HT_API_switchPa**_ line:

<div align="center">
  <img src="https://github.com/htmicron/ht32sx/blob/faq/.github/images/boost_enable.PNG">
</div>

<hr>
<br/>

<a name="credbin"></a>
### 18. I received new Sigfox credentials. How should I flash it to my device?
<hr>

> 1. Download and install [**STM32CubeProgrammer**](https://www.st.com/en/development-tools/stm32cubeprog.html).
> 2. Connect a STLINK into one of your USB connectors. 
> 3. Click on _**Connect**_ to access your device memory.

<div align="center">
  <img src="https://github.com/htmicron/ht32sx/blob/faq/.github/images/connect.png">
</div>

> 4. Click on _**Open File**_ and select the right binary file.
> 5. Click on _**Download**_ and change the _**Address**_ field to **0x08080000**:

<div align="center">
  <img src="https://github.com/htmicron/ht32sx/blob/faq/.github/images/address.png">
</div>

> 6. Press _**Enter**_ and your new Sigfox credentials shall be flashed.

<hr>
<br/>

## Contact Information

Head Office – São Leopoldo, RS <br/>
HT Micron Semiconductors <br/>
Unisinos Avenue, 1550 <br/>
São Leopoldo - RS <br/>
ZIP 93022-750 <br/>
Brazil <br/>
Tel: +55 51 3081-8650 <br/>
E-mail (Support): support_iot@htmicron.com.br <br/>
E-mail (General Enquiries): htmicron@htmicron.com.b <br/>
