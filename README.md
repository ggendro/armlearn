# WidowX Arm communication

API for a high level communication between an user and an Interbotix WidowX robotic arm, composed of 6 MX-64T Dynamixel servomotors and an Arbotix-m microcontroller.
Developed on linux OS but is theoretically cross-platform.


## Getting started


### Dependencies

*serial* library required for port communication.
Can be found here : <https://github.com/wjwwood/serial>

*KDL* library required for physics simulations in the Reinforcement Learning.
Can be found here : <http://www.orocos.org/kdl>

*Nlohmann/json* library required for reading json files.
Can be found here : <https://github.com/nlohmann/json>

*OpenMP* library used for computation tests.
Can be found here <https://www.openmp.org/> and installed by executing the following line on a terminal:
```
sudo apt-get install libomp-dev
```


Several libraries are required in order to build CNN and compute RL algorithms:

Python3 needs to be installed for the use of the following libraries.

*TensorFlow* library.
Can be found here : <https://www.tensorflow.org/> and installed by executing the following line on a terminal:
```
pip install tensorflow
```

*Keras* library.
Can be found here : <https://keras.io> and installed by executing the following line on a terminal:
```
pip install keras
```

*ScikitLearn* library.
Can be found here : <https://scikit-learn.org/> and installed by executing the following line on a terminal:
```
pip install scikit-learn
```

*GraphViz* library.
Can be found here : <https://www.graphviz.org/> and installed by executing the following line on a terminal:
```
pip install graphviz
```

*AnnVisualizer* library.
Can be found here : <https://pypi.org/project/ann_visualizer/> and installed by executing the following line on a terminal:
```
pip install ann_visualizer
```

*matplotlib* library.
Can be found here : <https://matplotlib.org/> and installed by executing the following line on a terminal:
```
pip install matplotlib
```

*SpinningUp* library.
Can be found here : <https://spinningup.openai.com/en/latest/index.html>


### Install

Not done yet. For now you can execute the following lines on a terminal to download and compile:

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

<div class="note">
Note that the previous command will generate all possible executable files.
</div>

To generate only the executable you want, run the following:
```
    cd build/
    cmake ..
```

Then go to the next section. Insert the make command between the instructions. Here is how to do for the ID example:
```
    cd examples/device_communication/example_id
    make
    sudo ./example_id
```


### Run tests and examples

After installation, you will find in the the build folder the project executables. For most of them, `sudo` authorization is required to access serial ports:

#### Run ID example

Run a code example setting the ID of a servomotor. 
```
    cd examples/device_communication/example_id
    sudo ./example_id
```

#### Run LED example

Run a code example turning ON and OFF the LED of a servomotor. 
```
    cd examples/device_communication/example_led
    sudo ./example_led
```

#### Run movement example

Run a code example executing several movements to the arm. 
```
    cd examples/device_communication/example_movement
    sudo ./example_movement
```

#### Run grab example

Run a code example executing a specific trajectory to the arm, grab and drop an object.
```
    cd examples/trajectories/example_grab
    sudo ./example_grab
```

#### Run coordinates example

Run a code example computing what coordinates are valid for the arm. Requires OpenMP.
```
    cd examples/computations/example_coordinates
    ./example_coordinates
```

#### Run converters example

Run a code example where positions are given and converted to cartesian and cylindrical coordinates.
```
    cd examples/computations/example_converters
    ./example_converters
```

#### Run cartesian mode example

Run a code example giving a (very) simple interface to control the arm by giving it cartesian oordinates.
```
    cd examples/computations/example_cartesianMode
    sudo ./example_cartesianMode
```

#### Run main

Run the main file. 
```
    cd tests/
    sudo ./main
```


## C++ Version

C++11 required for compilation of the project


## License

Not yet


## Author

Gaël Gendron (gael.gendron@insa-rennes.fr)