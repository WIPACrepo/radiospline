#!/usr/bin/env python
#
import unittest
import subprocess
import os

dir = os.path.abspath(os.path.dirname(__file__))
BUILDDIR = os.path.abspath(os.path.join(dir, "../../../build"))
BINDIR = os.path.join(BUILDDIR, "install/bin")
LIBDIR = os.path.join(BUILDDIR, "install/lib")

TABLEDIR = "../../../resources/tables/exponential"

# Set up environment (UGLY)
my_env = os.environ
if 'DYLD_LIBRARY_PATH' not in my_env:
    my_env['DYLD_LIBRARY_PATH'] = ""
if 'LD_LIBRARY_PATH' not in my_env:
    my_env['LD_LIBRARY_PATH'] = ""

my_env['RADIOSPLINE_TABLE_DIR'] = os.path.abspath(TABLEDIR)
my_env['DYLD_LIBRARY_PATH'] += os.pathsep + os.path.abspath(os.path.join(BUILDDIR, LIBDIR))
my_env['LD_LIBRARY_PATH'] += os.pathsep + os.path.abspath(os.path.join(BUILDDIR, LIBDIR))
try:
    my_env['LD_LIBRARY_PATH'] += os.pathsep + os.path.join(my_env['I3_PORTS'], "lib/gsl-1.14")
except KeyError:
    pass


# FIX ME: write pybindings instead of using test binaries
def splinedelay(src_x, src_y, src_z, trg_x, trg_y, trg_z, det_z):
    cmd = os.path.join(BINDIR, "ray")
    cmd += " %f %f %f %f %f %f %f" % (src_x, src_y, src_z, trg_x, trg_y, trg_z, det_z)
    output = subprocess.check_output([cmd], shell=True, env=my_env)
    return float(output.split('\n')[0])
    
class RayTests(unittest.TestCase):
        
    def testDelayDifference(self):
        # Maximum delay difference between splines and raytrace reference time, ns
        MAXDIFF = 1.0
        cases = [[1000., 0., -1500., 0., 0., -50., 0., 10389.],
                 [0., 200., -250., 0., 0., -150., 0., 1302.98],
                 [1000., 200., 50., 0., 0., -150., 0., 4025.11],
                 [1000., 200., 150., 0., 0., -50., -100., 4025.11],
                 [412., 1203., -1231., -104., -806., -175., 0., 13795.6],
                 [17.888203, 35.615644, -170.004229, -2.581381, 9.378155, -190.642184, 0., 227.302]]

        for case in cases:
            delay = splinedelay(*case[0:7])
            refdelay = case[7]
            self.failUnless(abs(delay-refdelay) < MAXDIFF)

    def testFirnShadow(self):
        cases = [[1000., 200., -50., 0., 0., -50., 0.],
                 [900., 220., -250., 0., 0., -30., 0.],
                 [412., 1203., -1231., -104., -806., -25., 0.]]
        
        for case in cases:
            delay = splinedelay(*case[0:7])
            self.assertEqual(delay, -1)

    def testUnsupported(self):
        cases = [[5000., 5000., 0., 0., 0., -50., 0.],
                 [100., 100., -100., 0., 0., -5., 0.],
                 [222., 222., -2900., 0., 100., -50., 0.]]
        for case in cases:
            delay = splinedelay(*case[0:7])
            self.assertEqual(delay, -1)
            
def main():
    unittest.main()

if __name__ == '__main__':
    main()
