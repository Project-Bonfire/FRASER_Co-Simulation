# FRASER_Co-Simulation

Get Started
-------

git clone --recursive https://github.com/Project-Bonfire/FRASER_Co-Simulation.git

Make sure that you also clone the submodules within the repository by adding the flag
--recursive.

Dependencies
-------
Note: The instructions on this were tested on Linux Mint 18.2 (based on on Ubuntu 16.04 LTS). It is also assumed that the tool is run on a local machine.

First of all, it needed to have key-based SSH login to the local machine:
  * Check if you have a key already. If the following returns a string starting with "ssh-rsa", then you can skip the key generation part
    - cat ~/.ssh/id_rsa.pub
  * Generate SSH key:
    - ssh-keygen -t rsa -C "your.email@example.com" -b 4096
  * Store the localhost's SSH key for using it for login
    - ssh-copy-id <your_username>@localhost
  * Try the SSH login to localhost - it will be working if you can SSH into localhost without the ssh command asking you a password:
    - ssh <your_username>@localhost
    - Type "exit" to close the ssh session

The following dependencies need to be satisfied in order to run the tool:
 * Python 3.6 (find current version with python3 -V ):
  - sudo add-apt-repository ppa:jonathonf/python-3.6
  - sudo apt update
  - sudo apt-get install python3.6
  - sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.5 1
  - sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.6 2
  - update-alternatives --list python3
  - sudo update-alternatives --config python3
  - python3 -V (check if install was successful)
* Install Python libraries and ansible with pip:
  - pip -V (make sure the version is 3.6. If this returns 2.x, you probably have to call pip using pip3)
  - sudo pip3.6 install --upgrade pip
  - sudo pip3.6 install setuptools
  - sudo pip3.6 install git+https://github.com/ansible/ansible.git@devel

* Install libraries for Python 2.7:
  - sudo pip2.7 install lxml configparser pyyaml
* Additional dependencies:
  - sudo apt-get install python-yaml python3.6-gdbm libzmq3-dev libboost1.58-all-dev libpugixml-dev python3.6-dev
  python3.6-venv
* Install Flatbuffers (needed to compile from source)
  - git clone https://github.com/google/flatbuffers.git
  - cd flatbuffers
  - cmake -G "Unix Makefiles"
  - make
  - sudo cp flatc flathash flatsamplebinary flatsampletext flattests /usr/local/bin

Finally, make a temporary folder for the simulation in the home folder
 * mkdir ~/tmp_simulation

Running
-------
* cd fraser-template
* make all

Note: This will ask you for a sudo password in order to install additional dependencies.

License
-------

Unless otherwise noted the source code is licensed under the
terms of the *GNU GENERAL PUBLIC LICENSE Version 3*.

The following open source software is used:

 - Fraser-template (licensed under MPL V2)
