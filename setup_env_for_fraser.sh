#!/bin/bash

# Check if we are root
if [ "$EUID" = 0 ]
 then echo "Please run the script as normal user. It will ask for root rights later"
 exit
fi

# Setup ssh login to localhost using keys

sudo apt update

sudo apt install -y openssh-server python-pip python3-pip

echo "--- The script will generate key-based login for localhost now. Press enter / type 'yes' for all questions"

ssh-keygen -t rsa -C "`whoami`@localhost" -b 4096

echo "--- Enter the password for your current username"
ssh-copy-id -f "`whoami`@localhost"

echo "--- Installing Python3.6"
# Switch to python 3.6

# Store installed libraries, so that we can reinstall it later and hopefully not kill the system
# Linux desktop environments rely in Python.

sudo -H pip3 install --upgrade pip

sudo -H pip3 freeze > packages.txt

sudo add-apt-repository -y ppa:jonathonf/python-3.6
sudo apt update

sudo apt install -y python3.6 python3.6-venv python3.6-dev
# sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.5 1
# sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.6 2
#
# sudo update-alternatives --set python3 /usr/bin/python3.5

echo "--- Installing pip for Python 3.6"
wget https://bootstrap.pypa.io/get-pip.py
sudo python3.6 get-pip.py

echo "--- Installing packages through APT"
sudo apt install -y git build-essential cmake python-yaml python3.6-gdbm libzmq3-dev libboost1.58-all-dev libpugixml-dev libxml2-dev libxslt-dev libssl-dev python3-openssl libboost-all-dev

echo "--- Installing Python packages"
# Libs needed for python2.7
sudo -H pip2 install --upgrade pip
sudo -H pip2 install --upgrade setuptools
sudo -H pip2 install --upgrade lxml
sudo -H pip2 install --upgrade configparser
sudo -H pip2 install --upgrade pyyaml

# Upgrade pip
sudo -H pip3.6 install --upgrade pip

# Re-install modules from the list stored from the previous Python version
sudo -H pip3.6 install  --upgrade -r packages.txt

# Install python modules from pip
sudo -H pip3.6 install --upgrade setuptools
sudo -H pip3.6 install --upgrade pyOpenSSL
sudo -H pip3.6 install --upgrade cryptography

echo "--- Compiling and installing Flatbuffers"
git clone https://github.com/google/flatbuffers.git
cd flatbuffers
cmake -G "Unix Makefiles"
make
sudo make install

chmod a+x flatc flathash flatsamplebinary flatsampletext flattests
sudo cp flatc flathash flatsamplebinary flatsampletext flattests /usr/local/bin

cd -

echo "--- Creating ~/tmp_simulation"
mkdir ~/tmp_simulation

echo "--- Installing development version of Ansible"
sudo -H pip3.6 install git+https://github.com/ansible/ansible.git@devel

echo "--- Cleaning up"
rm -f packages.txt
rm -Rf flatbuffers
rm -f get-pip.py

echo "Installation finished... hopefully"
