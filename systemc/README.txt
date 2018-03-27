
DOWNLOAD
SystemC class-library is available for free download at
http://www.accellera.org/downloads/standards/systemc
Get a suitable file, e.g.
http://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.2.tar.gz

INSTALL
Once the downloaded tarball is unzipped,
the README file mentions the platforms tested with this release of SystemC
and the INSTALL file details the installation instructions.

USING IN FRASER
Suppose the SystemC is installed at /path/to/systemc-2.3.2
Mention the SystemC home in the file FRASER_Co-Simulation/systemc/Makefile.rules
SYSTEMC_HOME=/path/to/systemc-2.3.2
If required, update the TARGET_ARCH
TARGET_ARCH      = linux64

FRASER-SYSTEMC EXAMPLES
FRASER_Co-Simulation/systemc/ has few examples to understand the initial concepts.
Run 'make' in each subfolder to generate the executable of the example.
e.g.
$ cd examples/helloworld/
$ make
$ ./helloworld.x
