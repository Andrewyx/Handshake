# Handshake: Robotics Anywhere

> Isometric View of Design 2: WallE
![WallEAssemblyIso](https://github.com/user-attachments/assets/0fcd6f2f-14b1-4e6c-b612-d5d0b7ba8db2)

## Description
Keep in touch with those too far away for a simple handshake! Handshake allows you and your friends to remotely control a
handshake robot via a web interface to interact with friends beyond just a screen or video call! 

This HCI project aims to produce a cohesive, easy-to-deploy, and user-friendly software application & test bed that allows for robot controls to be managed anywhere in the world. 
Traditional robotics use radio, LAN, Bluetooth, or similar range-limited wireless technologies to integrate user-interaction features. These solutions, often times seek to reduce latency as much as possible at the expense of functional range. 
### Thus, what if we try the inverse - design a robot with near-limitless range at the expense of some latency? 
This project contains two components: 
1) Software - This mainly includes on-robot code & server communications (also CI/CD)
2) Hardware - This is a huge component ranging from: PCB circuitry, CAD parts, STLs, and all the various manufacturing components (yes this was made in real life)

## Getting Started- Users
1) Ensure you have an assembled robot & have the most recent software flashed.
2) Follow the on-screen instructions to connect the robot to your local wifi. You must connect to the robot's broadcasted network and go to `192.168.4.1` on it to register your WiFi. 
3) Log in at https://handshake-664b7.firebaseapp.com/ to access controls.
4) Enter the unique 5-character ID displayed on the robot's screen to connect. Looks like `XX:XX`.
5) Congrats! You should now have control over the robot via the web UI!

## Technologies

### Software
To allow for control over any network, we leverage WiFi & an external server to handle data/communication. Specifically, for the sake of price, we use a backend as a service (BAAS) like Firebase. This is where user/account data is stored/updated during runtime to allow for network-based modifications from any WiFi-capable device/location. 
The robot software is written in C++ making use of the Arduino & PlatformIO frameworks. The WiFi setup portal is hosted natively onboard the ESP32 via the SPIFFS filesystem manager allowing the hosting of ReactJS on the microcontroller. Of course, communication is WiFi-based and takes advantage of the Firebase API to interact with the Firebase database.

### Hardware
The Handshake Robot is designed with an ESP32, SH1106 mini OLED display, and 2 SG90 9g continuous Micro Servos and powered by a standard 9V battery. However the DC step-down is technically rated for ~20V so feel free to use a higher capacity battery - you will just need to adjust the chassis. 
I decided to make my chassis based on WallE since I had done something similar in one of my first robotics projects years ago. Everything fits together with either friction or a few M3 screws & nuts. A roughly 1.5 gear reduction is present just to reduce torque on the motors and to fit the motors closer to the chassis. All the rest of the parts are 3D printed and need nothing more special than PLA (my Ender 3v2 was sufficient precision). 

> Cross Section of Design 2: WallE
![WallEAssemblyHalf](https://github.com/user-attachments/assets/d64c5445-bd2d-40f5-8bcf-905392bf67c1)
