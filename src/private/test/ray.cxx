#include <iostream>
#include <cstdlib>

#include "radiospline/IceGeometry.h"
#include "radiospline/RayDelay.h"
#include "radiospline/FirnShadow.h"

//
// Spline table names.  If you don't want to use the
// environment variable RADIOSPLINE_TABLE_DIR below, feel
// free to hard-code the paths here.
//
#define AIR_FILE "delay_inair.fits"
#define ICE_FILE "delay_inice.fits"
#define SHADOW_FILE "firn_shadow.fits"

int main(int argc, char **argv) {

    // Check that we know where to find the tables
    char * tablePath = getenv("RADIOSPLINE_TABLE_DIR");
    if (tablePath == NULL) {
        std::cout << "ERROR: please point the RADIOSPLINE_TABLE_DIR environment variable to" << std::endl;
        std::cout << " the spline .fits table directory." << std::endl;
        return -1;
    }
    std::string tablePathStr(tablePath);
      
    // Get command line arguments
    double r, zRec, zSrc;
    // Source and receiver specified in cylindrical coordinates
    if (argc == 4) {
        r = std::atof(argv[1]);
        zRec = std::atof(argv[2]);
        zSrc = std::atof(argv[3]);
    }
    //
    // Source and receiver specified in Cartesian coordinates
    // NOTE: you can use the helper function Detector2Cylinder below to convert 
    // your own coordinates into the table coordinate system
    //
    else if (argc == 8) {
        double coordSrc[3] = {std::atof(argv[1]), std::atof(argv[2]), std::atof(argv[3])};
        double coordTrg[3] = {std::atof(argv[4]), std::atof(argv[5]), std::atof(argv[6])};
        double zZero = std::atof(argv[7]);
        if (Detector2Cylinder(coordSrc, coordTrg, zZero, &r, &zRec, &zSrc) != 0)
            std::cout << "ERROR: couldn't convert to cylindrical coordinates." << std::endl;
    }
    else {
        std::cout << "Usage: " << argv[0] << " radius z_receiver z_source" << std::endl;
        std::cout << "    OR " << argv[0] << " src_x src_y src_z trg_x trg_y trg_z detector_z" << std::endl;
        return 0;
    }

    // Interface to the various spline tables
    // This object just needs to be created once
    RayDelay ray(tablePathStr+"/"+ICE_FILE,
                 tablePathStr+"/"+AIR_FILE,
                 tablePathStr+"/"+SHADOW_FILE);

    // Calculate the raytrace propagation time
    double tdelay = ray.GetPropagationTime(r, zRec, zSrc);
    std::cout << tdelay << std::endl;
    return 0;
}
