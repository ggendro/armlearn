# WidowX Arm communication and learning

API for a high level communication between an user and an Interbotix WidowX robotic arm, composed of 6 MX-64T Dynamixel servomotors and an Arbotix-m microcontroller.
Includes framework to simulate the arm, compute kinematics and learn IK using Reinforcement Learning. 
Developed on linux OS but is theoretically cross-platform.

 ** Warning! ** python/ and preesm/ folders have been temporary removed from repository. Consequently, the learning part is not currently available. python/ folder will be back soon. To retrieve the content of the preesm/ folder, create a folder named preesm/ and copy-paste the folder org.ietr.preesm.deep_deterministic_policy_gradient from the <https://github.com/preesm/preesm-apps> project (may not be in master branch).


## Getting started


### Dependencies

*serial* library v1.2.1 required for port communication.
Can be found here : <https://github.com/wjwwood/serial>

*KDL* library v1.4.0 required for physics simulations in the Reinforcement Learning.
Can be found here : <http://www.orocos.org/kdl>

*Nlohmann/json* library v3.6.1 required for reading json files.
Can be found here : <https://github.com/nlohmann/json>

*OpenMP* library version amd64/bionic 5.0.1-1 used for computation tests.
Can be found here <https://www.openmp.org/> and installed by executing the following line on a terminal:
```bash
sudo apt-get install libomp-dev
```

*GoogleTest* library v1.8.1 used for unitary tests.
Can be found here : <https://github.com/google/googletest> and installed by executing the follwing lines on a terminal:
```
sudo apt-get install libgtest-dev
```

*LCOV* library used for unitary tests coverage.
Can be found here : <http://ltp.sourceforge.net/coverage/lcov.php> and installed by executing the following lines on a terminal:
```bash
sudo apt-get install lcov
```

*Preesm* tool v3.14.1 is required for in order to build an run learners based on SDF graphs.
Can be found here : <https://preesm.github.io/>


Several libraries are required in order to build CNN and compute RL algorithms:

Python3 needs to be installed for the use of the following libraries.

*TensorFlow* library v1.14.0.
Can be found here : <https://www.tensorflow.org/> and installed by executing the following line on a terminal:
```bash
pip install tensorflow
```

*Keras* library v2.2.4.
Can be found here : <https://keras.io> and installed by executing the following line on a terminal:
```bash
pip install keras
```

*ScikitLearn* library v0.21.2.
Can be found here : <https://scikit-learn.org/> and installed by executing the following line on a terminal:
```bash
pip install scikit-learn
```

*GraphViz* library v0.11.
Can be found here : <https://www.graphviz.org/> and installed by executing the following line on a terminal:
```bash
pip install graphviz
```

*AnnVisualizer* library v2.5.
Can be found here : <https://pypi.org/project/ann_visualizer/> and installed by executing the following line on a terminal:
```bash
pip install ann_visualizer
```

*matplotlib* library v3.0.3.
Can be found here : <https://matplotlib.org/> and installed by executing the following line on a terminal:
```bash
pip install matplotlib
```

*tinyik* library v1.2.0.
Can be found here : <https://pypi.org/project/tinyik/> and installed by executing the following line on a terminal:
```bash
pip install tinyik
```


### Install

You can execute the following lines on a terminal to download and install the project and the library:

Get the code:
```bash
    git clone https://gitlab.insa-rennes.fr/Gael.Gendron/WidowXArmLearning.git
```

Install:
```bash
    cd build/
    cmake ..
    make
    make install
```

**Note:** *The make command will also generate all possible executable files.*


To do not install the library and generate only an executable you want, run the following:
```bash
    cd build/
    cmake ..
```

Then go to the next section. Insert the make command between the instructions. Here is how to do for the ID example:
```bash
    cd examples/device_communication/example_id
    make
    sudo ./example_id
```


### Run tests and examples

To better understand how the library works, some examples directly executable are provided. You will find in the the build folder the project executables. For most of them, `sudo` authorization is required to access serial ports:

#### Run ID example

Run a code example setting the ID of a servomotor. 
```bash
    cd examples/device_communication/example_id
    sudo ./example_id
```

#### Run LED example

Run a code example turning ON and OFF the LED of a servomotor. 
```bash
    cd examples/device_communication/example_led
    sudo ./example_led
```

#### Run movement example

Run a code example executing several movements to the arm. 
```bash
    cd examples/device_communication/example_movement
    sudo ./example_movement
```

#### Run grab example

Run a code example executing a specific trajectory to the arm, grab and drop an object.
```bash
    cd examples/trajectories/example_grab
    sudo ./example_grab
```

#### Run coordinates example

Run a code example computing what coordinates are valid for the arm. Requires OpenMP.
```bash
    cd examples/computations/example_coordinates
    ./example_coordinates
```

#### Run converters example

Run a code example where positions are given and converted to cartesian and cylindrical coordinates.
```bash
    cd examples/computations/example_converters
    ./example_converters
```

#### Run cartesian mode example

Run a code example giving a (very) simple interface to control the arm by giving it cartesian oordinates.
```bash
    cd examples/computations/example_cartesianMode
    sudo ./example_cartesianMode
```

#### Run learn example

Run a code example executing a Reinforcement Learning algorithm to control a simulation of the arm.
```bash
    cd examples/learning/example_learn
    ./example_learn
```

#### Run learn device example

Run a code example executing a Reinforcement Learning algorithm to control a simulation of the arm and testing it on a real arm.
```bash
    cd examples/learning/example_learn_device
    sudo ./example_learn_device
```


#### Run unitary tests

Run the unitary Google Tests for the C++ library.
```bash
    cd tests/gtests
    ./test-main
```

To generate the coverage, you can execute the following lines afterwards. Open the index.html file to see the coverage.
```bash
    lcov --capture --directory ./ --output-file coverage.info
    genhtml coverage.info --output-directory coverage
```


#### Run python learning tests

Run a python script testing the learning algorithm. It is possible to run several tests simultaneously using the following script, and it is possible to modify learning settings. The second command displays the help.
```bash
    cd ../python/
    ./multitester -h
    ./multitester
```

The following example creates a 3D arm with a first chain composed of a z axis motor and a rigit part of length 125, a second part composed of a x axis motor and a rigid part of length 142, ... The learning is done on the following set of taregt coordinates [[50, 25, 50], [200, 60, 200], [100, 0, 300], [75, 215, 125], [300, 300, 15]]. The hyperparamters for the learning are contained in the file ddpgstandardhyperparamranges.json, the path to the file explicited. Each model will trained 5 times. The mode of environment update is set to scaledSet.
```bash
    ./multitester.py --arms3D z125 x142 x142 x74 z41 z40 --coordinates  50 25 50 200 60 200 100 0 300 75 215 125 300 300 15 --hyperparameters ../files/learnSettings/ddpgstandardhyperparamranges.json --nbrepeat 5 --mode scaledSet
```

You can extract results and compare al your learnings. By default, test files are created and saved at files/learnSaves/testers/.
Run the following to visualize evolution of reward for your tests. Once again, command arguments are provided to change visualization parameters.
```bash
    ./multitester
    ./resultextractor
```

#### Run preesm

Run the learning algorithm implemented with Dataflow graphs. To correct the code generation done by Preesm, a prior script has to be run.

```bash
    ../preesm/org.ietr.preesm.reinforcement_learning/Spider/correctCodegen.py ../preesm/org.ietr.preesm.reinforcement_learning/Spider/generated/pi_ddpg.cpp
    cd Debug/
    ./reinforcement-learning
```

**Note:** *The Makefile is not located in the same folder than the executable. Running the following commands will compile the Preesm application. Compilation must be done after correction using python script.*


```bash
    cd preesm/org.ietr.preesm.reinforcement_learning/Spider/
    make
```


<div style="padding: 20px; background-color: #ffcc80; color: white;">
  <strong> Warning! </strong> The Preesm application is not currently working (v3.41.1 of Preesm). Wait for a new version of the Preesm tool.
</div>

<div style="padding: 20px; background-color: #ff9980; color: white;">
  <strong> Danger! </strong> The current Preesm application generates a huge number of actors when parameters values are great. Memory may entirely be filled.
</div>


## C++ Version

C++11 required for compilation of the project

## Python Version

Python3 required for executing learning scripts


## License

CeCILL-C


## Author

Gaël Gendron (gael.gendron@insa-rennes.fr)
