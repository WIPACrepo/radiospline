
radiospline
-----------

A library for fast radio raytracing through ice using spline-table interpolation
of precomputed ray solutions.

Uses the [photospline](http://www.sciencedirect.com/science/article/pii/S0010465513001434) library by N. Whitehorn and J. Van Santen for spline evaluation and table I/O (.fits files), published in the CPC Program Library.

Requirements
------------

- cmake, version >= 2.8.12.  A recent version is available on the WIPAC
  machines at `~jkelley/bin/cmake`.  

- GSL, cfitsio, and BLAS libraries

  On the WIPAC cluster, these are available in the CVMFS area
  and will be detected by the build process if you have sourced the CVMFS
  setup.sh file, by adding this to your `.bash_profile` (or similar):
  
      eval `/cvmfs/icecube.opensciencegrid.org/py2-v2/setup.sh`
      export CFITSIOROOT=${SROOT}

  On MacOS X, these can be installed using the Homebrew tool
  (http://brew.sh): 

      brew tap homebrew/science                 
      brew install gsl openblas cfitsio cmake

Building
--------

The libraries and test binaries can be built with the following steps:

    $ mkdir build; cd build
    $ cmake ..
    $ make

The test binaries and radiospline library will be installed in the build
directory in `install/bin` and `install/lib` respectively.

To rebuild, the following steps are currently required before re-running
cmake and make:  

    $ cd build; rm -rf CMakeFiles CMakeCache.txt

Usage
-----

The examples in `src/private/test` show how to use the library for
ray-tracing time delay calculation and firn shadow determination.  A
sample Makefile is provided to show how to use the built libraries in your
own project. 

To run any of the sample binaries, add the library build path to your library path.  

On Linux:

    $ export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:<radiospline directory>/build/install/lib

On Mac OS X:

    $ export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:<radiospline directory>/build/install/lib

Finally, you'll need to tell the program where the table files are located by setting RADIOSPLINE_TABLE_DIR:

    $ export RADIOSPLINE_TABLE_DIR=<your radiospline tree>/resources/tables/exponential



