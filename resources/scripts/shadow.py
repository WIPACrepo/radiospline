#!/usr/bin/env python
#
# Iteratively call ray_solver to find the boundaries of
# the firn shadow as a function of receiver depth and
# (horizontal) distance.
#
# J. Kelley
# 1 October 2014
#
import subprocess
import numpy as np

# Bedrock and surface according to the raytracer
SURFACE = 0.
BEDROCK = -2850.

# Which reflections to use
SURFACE_REFLECT = 1
BEDROCK_REFLECT = 0

# Path to the AraSim standalone raytracer (C. Weaver)
# Edit as necessary
#ARASIM = '../../../../AraSim-WorkingRayTracing'
#RAYSOLVER = ARASIM+'/ray_solver'
ARASIM = '../../../../AraSim/lu'
RAYSOLVER = ARASIM+'/singleray'

# Stop when critical depth is determined to this
# precision
DELTA_Z = 0.1

# Does a ray trace solution exist for this
# source / receiver depth and distance?
def rayExists(zRec, zSrc, dist):
    #args = " --src_x %g --src_y 0 --src_z %g" % (dist, zSrc)
    #args += " --trg_x 0 --trg_y 0 --trg_z %g" % (zRec)
    args = " %g 0 %g" % (dist, zSrc)
    args += " 0 0 %g" % (zRec)
    args += " %d %d" % (SURFACE_REFLECT, BEDROCK_REFLECT)
    proc = subprocess.Popen(RAYSOLVER+args, shell=True,
                              stdout=subprocess.PIPE)
    line = proc.stdout.readline()
    return "No solution" not in line

# Recursively find the critical depth at which
# all shallower solutions are forbidden
def findCriticalDepth(zRec, dist, zMin, zMax):
    #print "Checking ",zMin,zMax
    if (zMax-zMin < DELTA_Z):
        return zMax
    if rayExists(zRec, (zMin+zMax)/2., dist):
        return findCriticalDepth(zRec, dist, (zMin+zMax)/2., zMax)
    else:
        return findCriticalDepth(zRec, dist, zMin, (zMin+zMax)/2.)    


# Number of points to solve in each dimension
DMIN, DMAX, NDIST = 5, 5000, 90
ZMIN, ZMAX, NZ = -250, -10, 48

zArr = np.linspace(ZMIN, ZMAX, num=NZ)
distArr = np.linspace(DMIN, DMAX, num=NDIST)

for z in zArr:
    for d in distArr:        
        zCrit = findCriticalDepth(z, d, BEDROCK, SURFACE)
        print "%2.f\t%.2f\t%.2f" % (z,d,zCrit)

