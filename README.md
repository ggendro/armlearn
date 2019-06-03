# WidowX Arm communication

API for a high level communication between an user and an Interbotix WidowX robotic arm, composed of 6 MX-64T Dynamixel servomotors and an Arbotix-m microcontroller.
Developed on linux OS but is theoretically cross-platform.

## Getting started

### Install

Not done yet. For now you can execute the following lines on a terminal to download, compile and run the `main.cpp` file:

Get the code:
```
    git clone https://gitlab.insa-rennes.fr/Gael.Gendron/WidowXArmLearning.git
```

Build:
```
    cd build/
    cmake ..
    make
```

Run. `sudo` authorization is required to access serial ports:
```
    sudo ./main
```



### Dependencies

*serial* library required for port communication.
Can be found here : <https://github.com/wjwwood/serial>


## C++ Version

C++11 required for compilation of the project


## License

Not yet


## Author

Gaël Gendron (gael.gendron@insa-rennes.fr)