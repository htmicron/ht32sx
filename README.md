## HT32SX - HT Micron's Sigfox™ Monarch SiP

<div align="center">
  <img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcSesenrhZDRBpVRdUHpQ5ouT6wUTu0t0zaYtSI5GZqXJjGc2tor4Q&s">
</div>

The HT32SX is a System-in-Package (SiP) device build for the Internet of Things providing a **ready-to-use** connectivity solution.
It means that you’ll have strong reduction in your BOM cost, aiming a fast integration, simple design and the lowest **time-to-market**. It provides both uplink (transmit) and downlink (receive) network communications. As a SigFox™ Monarch enabled device, it allows globetrotter devices to seamlessly roam across the planet taking advantage of the global SigFox™ network coverage without need of reconfiguration, this setup is done automatically.

## The SMALLEST Sigfox™ Monarch SiP in the world!

In this repository you can find all the information you'll need to star to build your IoT aplication using the HT32SX and the Sigfox network. You can develop aplications based in our [examples](/firmware_applications) using the [Keil uVision IDE](https://www.keil.com/download/product/). 

The HT32SX SiP uses an ARM M0+ Core powered by ST Microelectronics (STM32L052x8) that provides to software developers working with STM32 devices [a **free-to-use** tool suite](https://www2.keil.com/stmicroelectronics-stm32/mdk). The MDK-Essential for STM32F0, STM32G0 and STM32L0 edition includes the Arm C/C++ Compiler, the Keil RTX5 real-time operating system kernel, and the µVision IDE/Debugger. It only works with STM32 devices based on the Cortex-M0/M0+ cores and is limited to a code size of 256 KB. The STM32 peripherals can be configured using [STM32 CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) and the resulting project exported to MDK.

In summary you'll find here:
*  [Technical Documentation](/docs)
*  Examples - codes, aplication notes and design tips.
 
## Application Examples

We have some applications that you can use to start developing your own solution! Please check out our examples:

| Application        | Brief                                                          |
|:-------------------|:---------------------------------------------------------------|
| [MOVE-ME](firmware_applications/Monarch_MoveME) | Scan a SigFox beacon and send frame via motion detection  |
| [Monarch Scan](firmware_applications/Monarch_Scan) | Scan a SigFox beacon and send frame to found region automatically |
| [SigFox Push-Button](firmware_applications/Push_Button/) | Send a SigFox frame after a button is pressed |


More code informations can be found at [HT Micron's Application Notes](https://htmicron.github.io/index.html) page.

Enjoy! =)

If you need help, please send a message to suppot_iot@htmicron.com.br 

---
