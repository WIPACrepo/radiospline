#include <iostream>
#include <cstdlib>
#include <sys/time.h>

#include "radiospline/IceGeometry.h"
#include "radiospline/RayDelay.h"
#include "radiospline/FirnShadow.h"

#define AIR_FILE "delay_inair.fits"
#define ICE_FILE "delay_inice.fits"
#define SHADOW_FILE "firn_shadow.fits"

typedef unsigned long long timestamp_t;
static timestamp_t get_timestamp () {
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

int main(int argc, char **argv) {

    // Check that we know where to find the tables
    char * tablePath = getenv("RADIOSPLINE_TABLE_DIR");
    if (tablePath == NULL) {
        std::cout << "ERROR: please point the RADIOSPLINE_TABLE_DIR environment variable to" << std::endl;
        std::cout << " the spline .fits table directory." << std::endl;
        return -1;
    }
    std::string tablePathStr(tablePath);
      
    // Number of times for spline lookup
    int nrays = 1000000;
    if (argc == 2) {
        nrays = std::atoi(argv[1]);
    }

    // Interface to the various spline tables
    RayDelay ray(tablePathStr+"/"+ICE_FILE,
                 tablePathStr+"/"+AIR_FILE,
                 tablePathStr+"/"+SHADOW_FILE);

    double coordSrc[3] = {0.0, 0.0, 0.0};
    double coordTrg[3] = {0.0, 0.0, 0.0};
    double tdelay, src_x, src_y, src_z, trg_z, r, zRec, zSrc;
    timestamp_t tstart = get_timestamp();
    srand(time(NULL));
    for (int i = 0; i < nrays; i++) {        
        coordTrg[2] = (double)rand()/RAND_MAX*-200.0 - 20.0;
        coordSrc[0] = (double)rand()/RAND_MAX*1000.0;
        coordSrc[1] = (double)rand()/RAND_MAX*1000.0;
        coordSrc[2] = (double)rand()/RAND_MAX*-2500.0;
        Detector2Cylinder(coordSrc, coordTrg, 0.0, &r, &zRec, &zSrc);
        tdelay = ray.GetPropagationTime(r, zRec, zSrc);
    }
    timestamp_t tstop = get_timestamp();
    std::cout << "Time for " << nrays << " ray lookups: " << (tstop-tstart)/1000000.0L << " s" << std::endl;
    return 0;
}
