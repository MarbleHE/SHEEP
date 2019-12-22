# Reverse Polish Notation calculator powered by SHEEP, a Homomorphic Encryption Evaluation Platform

## Installation - local

## Install git, gcc, cmake, tbb

### get wget git etc
```
apt-get update; apt-get -y install git
apt-get update; apt-get -y install wget
```

### get gcc-7 (gcc >=6 needed for SEAL).
```
apt-get -y install software-properties-common
add-apt-repository -y  ppa:ubuntu-toolchain-r/test
apt-get update; apt-get -y install gcc-7 g++-7
apt-get update; apt-get -y install build-essential
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 10
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 10
update-alternatives --set gcc /usr/bin/gcc-7
update-alternatives --set g++ /usr/bin/g++-7
```

### build cmake from source (to get a new enough version for SEAL)
```
wget https://cmake.org/files/v3.11/cmake-3.11.4.tar.gz
tar -xvzf cmake-3.11.4.tar.gz
cd cmake-3.11.4; export CC=gcc-7; export CXX=g++-7; ./bootstrap; make -j4; make install
```

### install intel-tbb for parallelisation
```
apt-get -y install libtbb-dev
```

## Build the RPN calculator
```
git clone --recurse-submodules -j8 https://github.com/stmario/SHEEP.git
```

## Build HElib

### get gmp (needed for HElib)
```
apt-get -y install m4
wget https://gmplib.org/download/gmp/gmp-6.1.2.tar.xz
tar -xvf gmp-6.1.2.tar.xz
cd gmp-6.1.2; export CC=gcc-7; export CXX=g++-7; ./configure; make; make install
```

### get ntl (needed for HElib)
```
wget http://www.shoup.net/ntl/ntl-11.1.0.tar.gz
tar -xvzf ntl-11.1.0.tar.gz
cd ntl-11.1.0/src; export CC=gcc-7; export CXX=g++-7; ./configure NTL_GMP_LIP=on NTL_EXCEPTIONS=on; make; make install
```

### build HElib
```
cd SHEEP/backend/lib/HElib; git reset --hard 9c50908a3538f5df77df523e525e1f9841f22eb2
cd SHEEP/backend/lib/HElib; git submodule update --init
cd SHEEP/backend/lib/HElib/src ; export CC=gcc-7; export CXX=g++-7; make clean; make;
```

## Build TFHE

### get fftw3 (needed for TFHE)
```
apt-get install -y libfftw3-dev
```

### build TFHE
```
cd SHEEP/backend/lib/tfhe; git reset --hard a65271bc8f5f0015c71351ed8746dd8eec051e29
cd SHEEP/backend/lib/tfhe; git submodule update --init

cd SHEEP/backend/lib/tfhe/build; export CC=gcc-7; export CXX=g++-7; cmake ../src -DENABLE_TESTS=on -DENABLE_FFTW=on -DCMAKE_BUILD_TYPE=optim -DENABLE_NAYUKI_PORTABLE=off -DENABLE_SPQLIOS_AVX=off -DENABLE_SPQLIOS_FMA=off -DENABLE_NAYUKI_AVX=off
cd SHEEP/backend/lib/tfhe/build; make; make install;
```

### get and build libpaillier
```
wget http://hms.isi.jhu.edu/acsc/libpaillier/libpaillier-0.8.tar.gz
tar -xvzf libpaillier-0.8.tar.gz
cd libpaillier-0.8 ; ./configure; make; make install
```

### install SEAL (Not working)
```
RUN git clone https://github.com/microsoft/SEAL.git
RUN cd SEAL/native/src; export CC=gcc-7; export CXX=g++-7 ; cmake .; make; make install
```

### install PALISADE (Not working)
```
RUN apt-get -y install lzip
RUN apt-get -y install flex
RUN apt-get -y install bison
RUN git clone https://git.njit.edu/palisade/PALISADE.git
RUN cd PALISADE && ./configure.sh
RUN cd PALISADE && echo "@@@@@" && g++ --version && CC=gcc-7 CXX=g++-7 make 
```

### get cpprestsdk (for the REST API) (OPTIONAL, not required for the RPN)
```
RUN apt-get update
RUN apt-get -y install libssl-dev
RUN apt-get -y install libboost-all-dev
RUN apt-get update
RUN git clone --recurse-submodules  https://github.com/Microsoft/cpprestsdk.git casablanca
RUN cd casablanca/Release; mkdir build.debug; cd build.debug; export CC=gcc-7; export CXX=g++-7; cmake .. -DCMAKE_BUILD_TYPE=Debug; make install
```

# SHEEP is a Homomorphic Encryption Evaluation Platform

## Disclaimer

SHEEP is **development** code.  We strongly recommend that you do not deploy this **prototype** application in a production setting.    SHEEP is intended to be an academic research framework to explore the homomorphic encryption (HE) space, and the characteristics of some HE libraries.  However, we are not responsible for the content of these external libraries, and we offer no guarantees regarding their use.

## Summary

The overall aim of the SHEEP project is to provide a platform for practitioners to evaluate the state-of-the-art of (fully) homomorphic encryption technology in the context of their concrete application. SHEEP enables this evaluation to be done across libraries implementing different HE schemes (with in some cases incomparable security).

<figure>
<img src="https://i.imgur.com/M9KPfZi.png"/>
<figcaption> Components of the SHEEP platform. These include an interpreter for the SHEEP language, an abstraction layer to incorporate HE libraries via their corresponding wrappers, and a reporting component. The user can either provide a program written in the high-level SHEEP language, or run a set of predefined benchmarks. Different aspects of the results can be visualized through a web interface.</figcaption>
</figure>


<!-- ![Components of SHEEP. These include an interpreter for the SHEEP language, an abstraction layer to incorporate HE libraries via their corresponding wrappers, and a reporting component. The user can either provide a program written in the high-level SHEEP language, or run a set of predefined benchmarks. Different aspects of the results can be visualized through a web interface.](https://i.imgur.com/M9KPfZi.png) -->


To achieve this goal, the SHEEP platform includes an **library agnostic language** so that computations can be described generically. One can think of it as an *assembly language*.

To incorporate a library into the platform, all that needs to be done is write a wrapper for it, implementing the operations in the **SHEEP instruction set**. These corresponds to common operations in a HE scheme such as encryption, decryption, homomorphic addition, multiplication, comparison, etc.

Besides allowing the user to execute their own programs via either a web interface or programatically, the library incorporates a set of **native benchmarks**. These are classified in three categories according to their complexity, ranging from low-level microbenchmarks of the basic homomorphic arithmetic operations, to data analysis tasks.

Either on a native benchmark or a user-defined input, the platform evaluates the corresponding program on all the HE libraries that support the instructions of the program. The result is checked for correctness, and metrics such as running time (encryption, decryption, and evaluation) and ciphertext size are reported to a web interface and stored in a database. The web interface also allows convenient plotting of the results for easy visualisation.

The platform hence enables two main use cases:
1. It allows users to evaluate a concrete custom computation across libraries (and thus encryption schemes) by simply describing it in a high-level language. This allows to set up a easily reproducible baseline when trying to optimize a given primitive based on HE, e.g. Private Information Retrieval, secure inner product, etc. For example, addition of two numbers in SHEEP looks like this:
```
INPUTS a b res
OUTPUTS res
a b ADD res
```
2. The set of predefined benchmarks allows performance snapshots of the state-of-the-art of (fully) homomorphic encryption implementations to be continually produced, as new schemes are invented and their implementations are updated and optimized.

In summary, the general problem addressed by SHEEP is that of getting a satisfying answer, from the practitioner's point of view, to the following questions:

> "What is the current state of the implementations of homomorphic encryption?"

> "What homomorphic encryption scheme (and implementation) should I use in my application, given some functional and non-functional, e.g. security, latency or bandwidth constraints?"

In its current state, the SHEEP platform does not provide a complete solution to these problems, as we believe this requires a community effort. This is why SHEEP's code is open source.


## Installation - docker (recommended)

Assuming you have docker installed, from this directory, do:
```
docker-compose build
```
This will build three Docker images: one for the "server", one for a web-based frontend, and one for some Jupyter notebooks containing examples
of SHEEP usage.

To run the web frontend, do:
```
docker-compose run -p 5000:5000 sheep-frontend
```
If you point your browser to `0.0.0.0:5000` you should get the SHEEP frontend.

To run the notebooks, do:
```
docker-compose run -p 8888:8888 sheep-notebook
```
Now if you paste the URL given in the terminal for "sheep-notebook_1"
into your browser, but with `localhost` instead of the hex string immediately after the "http://", you can access the Jupyter notebook examples.



## Installation - local

Clone this repository onto your local machine.  There are a few packages which the SHEEP platform itself uses, and then there are the HE libraries
that we want to evaluate.  Instructions for both follow:

### Core SHEEP dependencies

SHEEP uses the ***cpprestsdk*** library to provide a RESTful API for the sheep server.
On OSX, this can be installed with homebrew:
```
brew install cpprestsdk
```
For Linux (Ubuntu 16.04), see the relevant section of ```Dockerfile``` for a working recipe.

For parallel evaluation of circuits, SHEEP uses Intel's ***tbb*** library.  This can be installed on OSX using homebrew:
```
brew install tbb
```
or for apt-based Linux installations:
```
apt-get -y install libtbb-dev
```

There are a few Python packages that are needed for the web front-end and/or the Jupyter notebooks.  Install these with:
```
pip install  flask
pip install  wtforms
pip install  pytest
pip install  sqlalchemy
pip install  python-nvd3
pip install  requests
pip install  jupyter
pip install  matplotlib
pip install  pandas
```


### Example HE libraries

After cloning the SHEEP repo, change to the SHEEP directory then do:
```
git submodule update --init --recursive
```
This will checkout the HElib and TFHE submodules in the lib/ directory.

#### TFHE

To build TFHE, first install the fftw library (download source from http://www.fftw.org/download.html then follow the instructions on that page)

Then build TFHE as follows:
```
cd lib/tfhe
mkdir build
cd build
cmake ../src -DENABLE_TESTS=on -DENABLE_FFTW=on -DCMAKE_BUILD_TYPE=optim -DENABLE_NAYUKI_PORTABLE=off -DENABLE_SPQLIOS_AVX=off -DENABLE_SPQLIOS_FMA=off -DENABLE_NAYUKI_AVX=off
make
sudo make install
```

#### HElib

HElib depends on the ***GMP*** and ***NTL*** libraries.  To install gmp on OSX do:
```
brew install gmp
```
(this will install into ```/usr/local/Cellar/gmp``` - this location is needed when building ntl.)
For Linux, gmp can be installed from source - see the relevant section of ```Dockerfile```.

For ntl, download and follow instructions from [here](http://sid.ethz.ch/debian/ntl/ntl-5.5.2/doc/tour-unix.html).  At the ```./configure``` step, add the argument
```GMP_PREFIX=/usr/local/Cellar/gmp``` if you installed gmp using homebrew, or the relevant location if you used another method.

Then build HElib as follows:
```
cd ../../HElib/src
make
mkdir build
cd build
cmake ..
make all
```

#### SEAL

Download and follow the instructions [here](https://www.microsoft.com/en-us/download/details.aspx?id=56202).

#### libpaillier

Download the ```tar.gz``` file from [here](http://hms.isi.jhu.edu/acsc/libpaillier/), unpack the archive, cd to the directory, then do
```
./configure
make
make install
```
Note that libpaillier depends on gmp - follow the instructions for installing this from the "HElib" section if you haven't already.


## Building SHEEP

And finally build SHEEP:
```
cd <base SHEEP dir>
mkdir build
cd build
cmake ../
make all
```
This builds the examples in `build/bin`.

Run the tests with:
```
make test
```



## Circuit language and input files

The inputs to SHEEP are ***circuit*** files, which have the extension `.sheep` and the following format:
```
INPUTS <circuit_input_0> ...
OUTPUTS <circuit_output_0> ...
<gate_input_0> ...  <GATE> <gate_output>
...
```
