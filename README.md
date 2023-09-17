# FlowLink
![FlowLink (Custom)](https://github.com/ForSuhr/FlowLink/assets/105584788/2884b0d0-aad6-466a-9e79-e23dd0e4b812)

## Introduction
A cross-platform software designed to chat, transfer files and streaming within local network.

## Get Started
### Installation
#### Windows
Windows? Unzip and run.
#### Linux
```
chmod +x FlowLink-1.0.0-installer-linux-x64.run
```
```
./FlowLink-1.0.0-installer-linux-x64.run
```
Now you may find the shortcut on application launcher, just click it to run.

If you didn't find it, go to the directory where it was installed, type in terminal
```
./FlowLink.sh
```

*Notice: The released binary is deployed and tested on Ubuntu 22.04.2 (GNOME), there is no guarantee of running on other Linux distributions. But if you want, build from source. How to build? Grab a Qt5/6 (Qt 6.3.2 recommended) and build it with Qt Creator.*

### How to use
Open FlowLink in two different devices respectively, hit the connect button on one of them, it will start discovering its peer device within local network. No matter which one you click, they will connect to each other.

*Note that it makes no sense to hit connect on another device again, this may cause problems.*

![image](https://github.com/ForSuhr/FlowLink/assets/105584788/58953a64-6c7f-4892-bc5d-4be422dfb682)

Once a peer device appears, hit on it, use the following button to start your transfer.

![image](https://github.com/ForSuhr/FlowLink/assets/105584788/2484ccce-29fb-499c-b82f-801d77663e44)

## Features
- [x] Chat
- [x] File transfer
- [ ] Video Meeting (Streaming)

## Requirements & Dependencies
### Platform
Ubuntu Jammy jellyfish or Windows 10
### Libraries
- [Qt5/6](https://download.qt.io/archive/qt/)
- [ADS](https://github.com/githubuser0xFFFF/Qt-Advanced-Docking-System)
- [OBS](https://github.com/obsproject/obs-studio)
- [plog](https://github.com/SergiusTheBest/plog)
