
radiospline
J. Kelley, January 2015

A library for radio raytracing through ice using spline-table interpolation
of precomputed ray solutions.

Uses the photospline library by N. Whitehorn and J. Van Santen for spline
evaluation and table I/O (.fits files), availaible from the CPC Program
Library.

REQUIREMENTS
------------

- cmake, version >= 2.8.12.  A recent version is available on the WIPAC
  machines at ~jkelley/bin/cmake.  

- GSL, cfitsio, and BLAS libraries

  On the WIPAC cluster, these are available in the CVMFS I3_PORTS area
  and will be detected by the build process if you have sourced the CVMFS
  setup.sh file. 

  On MacOS X, these can be installed using the Homebrew tool
  (http://brew.sh): 

     brew tap homebrew/science                 
     brew install gsl openblas cfitsio cmake

BUILDING
--------

The libraries and test binaries can be built with the following steps:

  $ mkdir build; cd build
  $ cmake ..
  $ make

The test binaries and radiospline library will be installed in the build
directory in install/bin and install/lib respectively.

To rebuild, the following steps are currently required before re-running
cmake and make:  

  # Careful with this step
  $ cd build; rm -rf *  


USAGE
-----

The examples in src/private/test show how to use the library for
ray-tracing time delay calculation and firn shadow determination.  A
sample Makefile is provided to show how to use the built libraries in your
own project. 

In an IceCube environment, you may need to add the GSL library to your
library path:

  $ export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${I3_PORTS}/lib/gsl-1.14

Finally, to run any of the sample binaries, you'll need to tell the program
where the table files are located by setting RADIOSPLINE_TABLE_DIR:

  $ export RADIOSPLINE_TABLE_DIR=<your radiospline tree>/resources/tables/exponential



