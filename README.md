

This is the second draft of an API allowing a high level communication between an user and a WidowX robotic arm.
Developed on linux OS, is theoretically cross-platform, until now.


Install
===

Not done yet. For now you can execute the following lines on  a terminal to download, compile and run the `main.cpp` file:

Get the code:
```
    git clone https://gitlab.insa-rennes.fr/Gael.Gendron/WidowXArmLearning.git
```

Build:
```
    cd build/
    make
```

Run. `sudo` authorization is required to access serial ports:
```
    sudo ./main
```



Libraries
===

*serial* library required for port communication.
Can be found here : <https://github.com/wjwwood/serial>


C++ Version
===

C++17 required for compilation of the project


License
===

Not yet


Author
===

Gaël Gendron (gael.gendron@insa-rennes.fr)