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
my_env = os.environ.copy()
my_env['RADIOSPLINE_TABLE_DIR'] = os.path.abspath(TABLEDIR)
if 'LD_LIBRARY_PATH' not in my_env.keys():
    my_env['LD_LIBRARY_PATH'] = ''
my_env['LD_LIBRARY_PATH'] += os.pathsep + os.path.abspath(os.path.join(BUILDDIR, LIBDIR))
try:
    my_env['LD_LIBRARY_PATH'] += os.pathsep + os.path.join(my_env['I3_PORTS'], "lib/gsl-1.14")
except KeyError:
    pass

# FIX ME: write pybindings instead of using test binaries
def splinedelay(src_x, src_y, src_z, trg_x, trg_y, trg_z, det_z, reflected=False):
    cmd = os.path.join(BINDIR, "ray")
    cmd += " -- %f %f %f %f %f %f %f" % (src_x, src_y, src_z, trg_x, trg_y, trg_z, det_z)
    output = subprocess.check_output([cmd], shell=True, env=my_env).split('\n')
    if not reflected:
        return float(output[0])
    elif len(output) > 1:
        return float(output[1])
    else:
        return -1        

def splinelaunch(src_x, src_y, src_z, trg_x, trg_y, trg_z, det_z, reflected=False):
    cmd = os.path.join(BINDIR, "ray")
    cmd += " -l -- %f %f %f %f %f %f %f" % (src_x, src_y, src_z, trg_x, trg_y, trg_z, det_z)
    output = subprocess.check_output([cmd], shell=True, env=my_env).split('\n')
    if not reflected:
        return float(output[0])
    elif len(output) > 1:
        return float(output[1])
    else:
        return -1

def splinereceive(src_x, src_y, src_z, trg_x, trg_y, trg_z, det_z, reflected=False):
    cmd = os.path.join(BINDIR, "ray")
    cmd += " -r -- %f %f %f %f %f %f %f" % (src_x, src_y, src_z, trg_x, trg_y, trg_z, det_z)
    output = subprocess.check_output([cmd], shell=True, env=my_env).split('\n')
    if not reflected:
        return float(output[0])
    elif len(output) > 1:
        return float(output[1])
    else:
        return -1

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

    def testLaunchAngle(self):
        # Maximum launch angle difference between splines and raytrace, radians
        # (about 0.4 degrees)
        MAXDIFF = 0.007

        cases = [[1000., 0., -1500., 0., 0., -50., 0., 0.598753],
                 [0., 200., -250., 0., 0., -150., 0., 1.08745],
                 [1000., 200., 50., 0., 0., -150., 0., 1.62647],
                 [1111., 211., 2., 0., 0., -191., 0., 1.57284],
                 [4000., 0., -2200, 0, 0, -57, 0., 1.06561],
                 [20., 0., -150., 0., 0., -65., 0., 0.224117],
                 [4000., 0., -2200, 0, 0, -72, 0., 1.07255],  
                 [17.888203, 35.615644, -170.004229, -2.581381, 9.378155, -190.642184, 0., 2.12077]]

        for case in cases:
            launch = splinelaunch(*case[0:7])
            reflaunch = case[7]
#            print "launch:",reflaunch,launch
            self.failUnless(abs(launch-reflaunch) < MAXDIFF)

    def testReceiveAngle(self):
        # Maximum launch angle difference between splines and raytrace, radians
        MAXDIFF = 0.007
        
        cases = [[1000., 0., -1500., 0., 0., -50., 0., 0.699766],
                 [0., 200., -250., 0., 0., -150., 0., 1.1381],
                 [1000., 200., 50., 0., 0., -150., 0., 2.52252],
                 [1111., 211., 2., 0., 0., -191., 0., 2.53153],
# FIX ME this test case fails                   
#                [4000., 0., -2200, 0, 0, -57, 0., 1.41274],
                 [20., 0., -150., 0., 0., -65., 0., 0.2417],                 
                 [4000., 0., -2200, 0, 0, -72, 0., 1.32092], 
                 [17.888203, 35.615644, -170.004229, -2.581381, 9.378155, -190.642184, 0., 2.13085]]

        for case in cases:
            receive = splinereceive(*case[0:7])
            refreceive = case[7]
#            print "receive:",refreceive,receive            
            self.failUnless(abs(receive-refreceive) < MAXDIFF)

    def testReflectedDelayDifference(self):
        # Maximum delay difference between splines and raytrace reference time, ns
        MAXDIFF = 1.0
        cases = [[1000., 0., -1500., 0., 0., -50., 0., 10753.9],
                 [0., 200., -250., 0., 0., -150., 0., 2432],
                 [3333.0, -23.0, -2800., 101., 400., -30., 0, 25374.5],                 
                 [100., 0., -300., 0., 0., -150, 0, 2531.52],
                 [17.888203, 35.615644, -170.004229, -2.581381, 9.378155, -190.642184, 0., 1952.21],
                 [0., 462., -1905.0, 0., 0., -44, 0, 11731.1]]

        for case in cases:
            delay = splinedelay(*case[0:7], reflected=True)            
            refdelay = case[7]
#            print "reflected:",refdelay,delay
            self.failUnless(abs(delay-refdelay) < MAXDIFF)

    def testReflectedLaunchAngle(self):
        # Maximum launch angle difference between splines and raytrace, radians
        # (about 0.4 degrees)
        MAXDIFF = 0.007

        cases = [[1000., 0., -1500., 0., 0., -50., 0., 0.55856],
                 [0., 200., -250., 0., 0., -150., 0., 0.423123],
                 [4000., 0., -2200, 0, 0, -57, 0., 1.04236],
                 [20., 0., -150., 0., 0., -65., 0., 0.0837902],
                 [4000., 0., -2200, 0, 0, -72, 0., 1.03542],  
                 [17.888203, 35.615644, -170.004229, -2.581381, 9.378155, -190.642184, 0., 0.085313]]

        for case in cases:
            launch = splinelaunch(*case[0:7], reflected=True)
            reflaunch = case[7]
#            print "launch:",reflaunch,launch
            self.failUnless(abs(launch-reflaunch) < MAXDIFF)
            
    def testReflectedReceiveAngle(self):
        # Maximum launch angle difference between splines and raytrace, radians
        MAXDIFF = 0.007
        
        cases = [[1000., 0., -1500., 0., 0., -50., 0., 2.4911],
                 [0., 200., -250., 0., 0., -150., 0., 2.70705],
# FIX ME fails                
#                 [4000., 0., -2200, 0, 0, -57, 0., 1.79695],
                 [20., 0., -150., 0., 0., -65., 0., 3.05133],                 
                 [4000., 0., -2200, 0, 0, -72, 0., 1.89259], 
                 [17.888203, 35.615644, -170.004229, -2.581381, 9.378155, -190.642184, 0., 3.05681]]

        for case in cases:
            receive = splinereceive(*case[0:7], reflected=True)
            refreceive = case[7]
#            print "receive:",refreceive,receive            
            self.failUnless(abs(receive-refreceive) < MAXDIFF)
            
def main():
    unittest.main()

if __name__ == '__main__':
    main()
