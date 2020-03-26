GIT is a Version Control System (VCS). So, when used in repository of software' code, it will keep all the history of the code files. Through a command line interface or through a GUI, you may recover all those versions.

GitHub is a sort of standartized tool where people and companies put repositories with software code, sometimes internal to the company, sometimes public as [HT32SX repo](https://github.com/htmicron/ht32sx/).

To use GIT, you may need one of the following softwares:

* RedHat Linux and variants: on a terminal ```sudo yum install git -y ```
* Debian Linux and variants: on a terminal ```sudo apt-get install git -y```
* Windows: download and install https://git-scm.com/download/win


One of the features of GIT is the *branch*ing, where one repository can have different branches. Here, at HT32SX repo we use that to different versions of the software to different revisions (or versions) of the hardware.

(Coming soon: how to identify your hardware version/revision, for now assume it is 2.1)

Lets say you have the version 2.1 of the hardware, today as ``` March, 2020 ``` you may have two options:

* release_h2.1_1.0: Software 1.0 of HW 2.1 STABLE (it will only receive bugfixes from now, EOL 1st July, 2020)

* release_h2.1_2.0: Software 2.0 of HW 2.1 UNSTABLE (it is under development, EOL 20th of January, 2021) 

The first one (h2.1_1.0) is already stable: so if you use it as base for your code, you are safe. It will only be changed if a critical bug is found, and it makes the program work in a not properly manner.

The second release (h2.1_2.0) is still unstable. It means that new features still being add to it. If you start your own program with this code base, and this code base gets updated, your program almost sure will broke, so it will require some changes to receive the update properly.

The *master* branch is attached to the last hardware version. You should not use it, unless you want to test the latest features, but be warned: the code there may not even compile/build.

Back to the git, to download your prefered branch, click on that one in the initial page (https://github.com/htmicron/ht32sx#about-this-repository) and look for this:

```
To clone this branch directly (hardware version X, software version X), use:
```

for example, hardware version 2.1, software version 1.0, open your terminal tool (in Git for Windows it is called Git Bash - type it in your start menu) and type there:

```
cd ./Desktop #(puting the terminal in your Desktop folder)
git clone --single-branch --branch release_h2.1_1.0 https://github.com/htmicron/ht32sx.git #cloning the specified version.
```

These commands will create a folder named ht32sx in your Desktop with all the branch data.

Now, select one example inside the folder Desktop/ht32sx/firmware_applications. Lets say it is the Push_Button one. Copy the entire folder *Push_Button* to an other place in your computer (lets say My Documents folder).
All changes you make to this folder will be safe from updates in the code base.

If you look inside the sub-folder MDK-ARM you will find a Arm-KEIL project ready to use and build.


If you want to update the code base (in Desktop/ht32sx), run the following command (BE WARNED: YOUR CHANGES IN THE DESKTOP/HT32SX FOLDER WILL BE LOST FOREVER):

```
cd Desktop/ht32sx
git stash
git pull
```

