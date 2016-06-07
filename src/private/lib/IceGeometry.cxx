#include <cstdlib>
#include <cmath>

int Detector2Cylinder(double *coordSrc, double *coordTrg,
                      double zZero,
                      double *r, double *zRec, double *zSrc) {

    if ((coordSrc == NULL) || (coordTrg == NULL) || (r == NULL) ||
        (zRec == NULL) || (zSrc == NULL)) {
        return -1;
    }
    
    // First shift to coordinates centered at target
    // and fix up z-coordinate
    double trgShift[3] = {0.0, 0.0, coordTrg[2]+zZero};
    double srcShift[3] = {coordSrc[0]-coordTrg[0],
                          coordSrc[1]-coordTrg[1],
                          coordSrc[2]+zZero};
  
    *r = sqrt(srcShift[0]*srcShift[0] + srcShift[1]*srcShift[1]);
    *zSrc = srcShift[2];
    *zRec = trgShift[2];
    
    return 0;
    
}
