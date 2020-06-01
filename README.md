# HT32SX - HT Micron's Sigfox™ Monarch SiP

## Readme Organization

* [Introduction](#the-smallest-sigfox-monarch-sip-in-the-world)
* [About this repository](#about-this-repository)
  * [Using GIT for the first time?](/git_basic_usage_with_ht32sx_repo.md)
* [FAQ, additional information and contact.](#faq)

<hr>

## The SMALLEST Sigfox™ Monarch SiP in the world!

<div align="center">
  <img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcSesenrhZDRBpVRdUHpQ5ouT6wUTu0t0zaYtSI5GZqXJjGc2tor4Q&s">
</div>

The HT32SX is a System-in-Package (SiP) device build for the Internet of Things providing a **ready-to-use** connectivity solution.
You’ll have strong reduction in your BOM cost aiming a fast integration, simple design and the lowest **time-to-market**.

As a SigFox™ Monarch enabled device, it allows globetrotter devices to seamlessly roam across the planet taking advantage of the global SigFox™ network coverage without need of reconfiguration since region setup is done automatically.

<hr>

## About this repository


### Branches

(Coming soon: how to identify your hardware version/revision, for now assume it is *2.1*)

* **INFO**: this informative branch.
* **[release_h2.1_1.0](https://github.com/htmicron/ht32sx/tree/release_h2.1_1.0)**: Software 1.0 of HW 2.1 **STABLE** (it will only receive bugfixes from now, EOL 1st July, 2020)
  * Credentials stored in reserved Flash memory area
  * Monarch Regions Enabled and Certified: RC1, RC2, RC3, RC4, RC5 and RC6.
  * IDE/Compiler: Arm Keil
  * Bootloader capable
* **[release_h2.1_2.0](https://github.com/htmicron/ht32sx/tree/release_h2.1_2.0)**: Software 2.0 of HW 2.1 **UNSTABLE** (it is under development, EOL 20th of January, 2021)
  * Credentials stored in EEPROM memory
  * Regions:
    * Monarch (Certified): RC1, RC2, RC3, RC4, RC5, RC6 and RC7*.
  * IDE/Compiler:
    * Arm Keil
    * Coming soon: ST CubeIDE
  * Bootloader capable
* **[release_h2.2_1.0](https://github.com/htmicron/ht32sx/tree/release_h2.2_1.0)**: Software 1.0 of HW 2.2 **STABLE** (it will only receive bugfixes from now, EOL 20th of January, 2021)
  * Credentials stored in EEPROM memory
  * Regions:
    * Monarch (Certified): RC1, RC2, RC3, RC4, RC5, RC6 and RC7*.
  * IDE/Compiler:
    * Arm Keil
    * ST CubeIDE
* **[release_h2.2_2.0](https://github.com/htmicron/ht32sx/tree/release_h2.2_2.0)**: Software 2.0 of HW 2.2 **UNSTABLE** (it is under development, EOL 1st of July, 2021)
  * Credentials stored in EEPROM memory
  * Regions:
    * Monarch (Certified): RC1, RC2, RC3, RC4, RC5, RC6 and RC7*.
  * IDE/Compiler:
    * Arm Keil
    * ST CubeIDE
  * Bootloader capable
* **[master](https://github.com/htmicron/ht32sx/tree/master)**: (obsolete, pending to be removed) lastest code under development for HW version 1.0 **VERY UNSTABLE**
* **[master_2](https://github.com/htmicron/ht32sx/tree/master_2)**: lastest code under development for HW version 2.0 **VERY UNSTABLE**

Legend: * RC7 is pending certification

#### USING GIT/GITHUB FOR THE FIRST TIME?
Check it out: [GIT basic usage with HT32SX](/git_basic_usage_with_ht32sx_repo.md)

#### Which branch to start a new project on?
Use the lastest software version for your hardware version.

#### Why there are different software versions related to the same hardware version?
To provied a stable code-base for developers, the **stable** versions are provided. In these, only bugfixes are made. So, when a new fature is add to the code-base, a new software version is created.

(More in [FAQ](https://htmicron.github.io/FAQ/html/index.html))

### Bugtrack
If you want to issue a bug to our development team, please follow our [bugtrack guideline](How_to_bugtracker.md)

### Tree view of the branches

You'll find here:
*  Examples - codes, aplication notes and design tips.

This map is from the master branch, yet it provides the general overview of the branches' structure.
```
 ├─ applications [application notes, examples]
 │  ├─ Evaluation_Board_Design_Reference
 │  ├─ Generic_Push_Button
 │  ├─ Monarch_Scan
 │  ├─ Monarch_MoveMe
 │  ├─ Push_Button
 │  ├─ antenna_design_reference
 │  ├─ bootloaderRefresh
 ├─ LICENSE
 └─ README.md
```

<hr>

### FAQ

Do you need help? Check our [FAQ](https://htmicron.github.io/FAQ/html/index.html) or send an email to suppot_iot@htmicron.com.br. 


More code informations can be found at [HT Micron's Application Notes](https://htmicron.github.io/index.html) page.

Enjoy! =)

---
