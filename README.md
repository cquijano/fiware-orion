# Orion Context Broker

This is the code repository for the Orion Context Broker, a C++ implementation of the NGSI9/10 REST API binding
developed as a part of the FI-WARE platform.

If this is your first contact with Orion Context Broker, it is highly recommended to have a look to the brief Quick Start guide:

https://wiki.fi-ware.org/Publish/Subscribe_Broker_-_Orion_Context_Broker_-_Quick_Start_for_Programmers

You will find all the information on Orion Context Broker in its page in the FI-WARE Catalogue:

http://catalogue.fi-ware.eu/enablers/publishsubscribe-context-broker-orion-context-broker

Note that you don't need this repository code if you install the broker via its RPM package (check Orion Context
Broker installation and administration manual about installing via RPM).

## Vagrant support
If you want yo have an Orion Context Broker ready to develop in your machine easily we provide a Vagrant file so that you can get up and running. Just run:

    vagrant up

You just need to install Vagrant if you want to use this, and at least Virtualbox. Especially useful for those that use virtual machines to develop on a Mac or a Linux distribution that is not CentOS. Keep in mind that the Vagrant file specifies CentOS 6.5 as a base operating system.

For example, to compile in debug mode and install it in the home directory simply run:

     vagrant ssh -c 'INSTALL_DIR=/home/vagrant make di -C fiware-orion'

After a few minutes you can run contextBroker. Again, you can either ssh into the machine with 

    vagrant ssh

and run it from the command-line, or directly run the broker with something like

    vagrant ssh -c 'contextBroker -multiservice -t 0-255'

Orion Context Broker will be accessible at `127.0.0.1:1026` (`5683` for CoAP).

You can also use these commands to automate building and running from your favorite IDE.

*NOTE:* The virtualbox machine that is created uses additional resources to those from the broker itself. It uses around 512 MiB of RAM and around 1.20 GiB of disk space with Orion already compiled in debug mode.



The bootstrap script basically goes through the installation instructions in the README.
## Installing and Using the broker

The administration and programming manuals for Orion Context Broker are found in the FI-WARE Catalogue page,
under the "Documentation" tab.

http://catalogue.fi-ware.eu/enablers/publishsubscribe-context-broker-orion-context-broker

## Building Orion Context Broker

Orion Context Broker reference distribution is CentOS 6.3. This doesn't mean that the broker cannot be
built in other distributions (actually, it can). This section also includes indications on how to build
in other distributions, just in the case it may help people that don't use CentOS. However, note that
the only "officially supported" procedure is the one for CentOS 6.3; the others are provided "as is"
and can get obsolete from time to time.

**Note:** the build includes both contextBroker binary and proxyCoap. If you are not interested in proxyCoap
at all, you can disable it just commenting out the following line in the CMakeList.txt file:

```
ADD_SUBDIRECTORY(src/app/proxyCoap)
```

and removing the following files:

```
test/functionalTest/cases/coap_basic.test
test/functionalTest/cases/coap_command_line_options.test
test/functionalTest/cases/coap_version.test
```

In that case, you can also ignore all the steps in the building process marked as "(Optional proxyCoap)"

### CentOS 6.3 (official)

The Orion Context Broker uses the following libraries as build dependencies:

* boost: 1.41 (the one that comes in EPEL6 repository)
* libmicrohttpd: 0.9.22 (the one that comes in EPEL6 repository)
* libcurl: 7.19.7
* Mongo Driver: 2.2.3 (from source)
* gtest (only for `make unit_test` building target): 1.5 (from sources)
* gmock (only for `make unit_test` building target): 1.5 (from sources)
* cantcoap (for proxyCoap)

We assume that EPEL6 repository is configured in yum, given that many RPM packages are installed from there
(check the procedure at http://fedoraproject.org/wiki/EPEL#How_can_I_use_these_extra_packages.3F):

The basic procedure is as follows (assuming you don't run commands as root, we use sudo for those
commands that require root privilege):

* Install the needed building tools (compiler, etc.).

```
sudo yum install make cmake gcc-c++ scons
```

* Install the required libraries (except the mongo driver and gmock, described in following steps).

```
sudo yum install libmicrohttpd-devel boost-devel libcurl-devel
```

* Install the Mongo Driver from source (reference procedure http://docs.mongodb.org/ecosystem/tutorial/getting-started-with-cpp-driver/):

```
wget http://downloads.mongodb.org/cxx-driver/mongodb-linux-x86_64-2.2.3.tgz
# Don't pay attention to the '-linux-x86_64' in the name: it is actually source code for any platform
tar xfvz mongodb-linux-x86_64-2.2.3.tgz
cd mongo-cxx-driver-v2.2
scons                                         # The build/libmongoclient.a library is generated as outcome
sudo scons install                            # This puts .h files in /usr/local/include and libmongoclient.a in /usr/local/lib
sudo chmod a+r -R /usr/local/include/mongo    # It seems that scons install breaks permissions
```

* Install Google Test/Mock from sources (there are RPM pacakges for this, but they don't seem to be working with the
  current CMakeLists.txt configuration)

```
wget http://googlemock.googlecode.com/files/gmock-1.5.0.tar.bz2
tar xfvj gmock-1.5.0.tar.bz2
cd gmock-1.5.0
./configure
make
sudo make install  # installation puts .h files in /usr/local/include and library in /usr/local/lib
sudo ldconfig      # just in case... it doesn't hurt :)
```

* (Optional proxyCoap) Install cantcoap (with dependencies). Note that we are using a particular snapshot of the code (corresponding to around July 21st, 2014) given that cantcoap repository doesn't provide any releasing mechanism.
```
sudo yum install clang CUnit-devel

git clone https://github.com/staropram/cantcoap
cd cantcoap
git checkout 749e22376664dd3adae17492090e58882d3b28a7
make
sudo cp cantcoap.h /usr/local/include
sudo cp dbg.h /usr/local/include
sudo cp nethelper.h /usr/local/include
sudo cp libcantcoap.a /usr/local/lib
```

* Get the code (alternatively you can download it using a zipped version or a different URL pattern, e.g `git clone git@github.com:telefonicaid/fiware-orion.git`):

```
sudo yum install git
git clone https://github.com/telefonicaid/fiware-orion
```

* Build the source:

```
cd fiware-orion
make
```

* (Optional but highly recommended) run unit test. Firstly, you have to install MongoDB (as the unit tests rely on mongod
  running in localhost).

```
sudo yum install mongodb-server
sudo yum  update pcre            # otherwise, mongod crashes in CentOS 6.3
sudo /etc/init.d/mongod start
sudo /etc/init.d/mongod status   # to check that mongod is actually running
make unit_test
```

* Install the binary. You can use INSTALL_DIR to set the installation prefix path (default is
/usr), thus the broker is installed in `$INSTALL_DIR/bin` directory.

```
sudo make install INSTALL_DIR=/usr
```

* Check that everything is ok, invoking the broker version message:

```
contextBroker --version
```

The Orion Context Broker comes with a suite of valgrind and end-to-end tests that you can also run,
following the following procedure (optional):

* Install the required tools:

```
sudo yum install python python-flask python-jinja2 curl libxml2 nc mongodb valgrind libxslt 
```

* (Optional proxyCoap) Install COAP client (an example application included in the libcoap sources).
```
wget http://sourceforge.net/projects/libcoap/files/coap-18/libcoap-4.1.1.tar.gz/download
mv download libcoap-4.1.1.tar.gz
tar xvzf libcoap-4.1.1.tar.gz
cd libcoap-4.1.1
./configure
make
sudo cp examples/coap-client /usr/local/bin
```

* Run valgrind tests (it takes some time, please be patient):

```
make valgrind
```

* Prepare the environment for test harness. Basically, you have to install the accumulator-server.py script
  and in a path under your control, ~/bin is the recommended one. Alternatively, you can install them in a
  system directory such as /usr/bin but it could collide with an RPM installation, thus it is not
  recommended. In addition, you have to set several environment variables used by the harness script
  (see scripts/testEnv.sh file).

```
mkdir ~/bin
export PATH=~/bin:$PATH
make install_scripts INSTALL_DIR=~
. scripts/testEnv.sh
```

* Run test harness (it takes some time, arm yourself with patience).

```
make functional_test INSTALL_DIR=~
```

You can generate coverage reports for the Orion Context Broker using the following procedure
(optional):

* Install the required tools. You need lcov 1.10 - the one that comes with CentOS 6.3 (lcov 1.9) is
  not valid.

```
sudo rpm -Uhv http://downloads.sourceforge.net/ltp/lcov-1.10-1.noarch.rpm
```

* Do first a successful pass for unit_test and functional_test, to check that everything is ok (see above)

* Run coverage

```
make coverage INSTALL_DIR=~
```

You can generate the RPM for the source code (optional):

* Install the required tools

```
sudo yum install rpm-build
```

* Generate the RPM

```
make rpm
```

* The generated RPMs are placed in directory ~/rpmbuild/RPMS/x86_64

### Debian 7 (unofficial)

To be polished.

The packages are basically the same described for RedHat/CentOS above, except that we need to
install packages using apt-get instead of yum.

Install Google Test and Google Mock version 1.5 directly from sources.

The version of lcov that comes with Debian 7.0 (1.9) has a bug (see https://bugs.launchpad.net/ubuntu/+source/lcov/+bug/1163758).
Install lcov 1.10 from sources.

## License

Orion Context Broker is licensed under Affero General Public License (GPL) version 3.

## Troubleshooting

### Building MongoDB C++ Driver

#### Error at "boost/exception/detail/exception_ptr_base.hpp"

* OS version: CentOS/RHEL release 6.4
* GCC version: 4.4.7

You can experience the following error when building the driver:

```
scons: Building targets ...
g++ -o build/mongo/util/file_allocator.o -c -O3 -pthread -D_SCONS -DMONGO_EXPOSE_MACROS
-Ibuild -Isrc -Ibuild/mongo -Isrc/mongo src/mongo/util/file_allocator.cpp
In file included from /usr/include/boost/thread/future.hpp:12,
                 from /usr/include/boost/thread.hpp:24,
                 from src/mongo/util/file_allocator.cpp:20:
/usr/include/boost/exception_ptr.hpp:43: error: looser throw specifier for 'virtual
boost::exception_ptr::~exception_ptr()'
/usr/include/boost/exception/detail/exception_ptr_base.hpp:26: error:   overriding
'virtual boost::exception_detail::exception_ptr_base::~exception_ptr_base() throw ()'
scons: *** [build/mongo/util/file_allocator.o] Error 1
scons: building terminated because of errors.
```

This is due to a virtual destructor within boost/exception/detail/exception_ptr_base.hpp (a boost library source file) added in order to avoid a warning appearing when using an old version of gcc. This virtual method must be commented (safe comment, see the references) if the above error wants to be avoided.

```
23:                         protected:
24: /*
25:                         virtual
26:                         ~exception_ptr_base() throw()
27:                             {
28:                             }
29: */
30:                         };
```

Reference: https://groups.google.com/forum/?fromgroups=#!topic/boost-list/gBodhfp9LjI

### Building Boost Libraries

* OS version: openSUSE 13.1
* GCC version: 4.8.1

You may encounter this problem:

```
/usr/local/include/boost/atomic/atomic.hpp:202:16: error: ‘uintptr_t’ was not declared in this scope
typedef atomic<uintptr_t> atomic_uintptr_t;

/usr/local/include/boost/atomic/atomic.hpp:202:25: error: template argument 1 is invalid
typedef atomic<uintptr_t> atomic_uintptr_t;
```

There seems to be a problem with boost libraries. It can be fixed changing /usr/local/include/boost/cstdint.hpp. Find the line

```
#if defined(BOOST_HAS_STDINT_H) && (!defined(__GLIBC__) || defined(__GLIBC_HAVE_LONG_LONG)) 
```

And change it for

```
#if defined(BOOST_HAS_STDINT_H)                                 \ 
    && (!defined(__GLIBC__)                                       \ 
        || defined(__GLIBC_HAVE_LONG_LONG)                        \ 
        || (defined(__GLIBC__) && ((__GLIBC__ > 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ >= 17))))) 
```

Reference: https://svn.boost.org/trac/boost/changeset/84950
