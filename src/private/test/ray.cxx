#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "radiospline/IceGeometry.h"
#include "radiospline/RayTrace.h"
#include "radiospline/FirnShadow.h"

void usage(char *prog) {
    std::cout << "Usage: " << prog << " -[adhlr] -- radius z_receiver z_source" << std::endl;
    std::cout << "    OR " << prog << " -[adhlr] -- src_x src_y src_z trg_x trg_y trg_z detector_z" << std::endl;
    std::cout << "   -h  print this usage" << std::endl;
    std::cout << "   -a  print all available raytrace results (delay, launch angle, receipt angle)" << std::endl;
    std::cout << "   -l  print launch angle (radians)" << std::endl;
    std::cout << "   -r  print receipt angle (radians)" << std::endl;
    std::cout << "   -d  print propagation delay (ns) [default]" << std::endl;
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

    // Get command line arguments
    double r, zRec, zSrc;
    int c;
    int delay = 1;
    int launch = 0;
    int receipt = 0;
    while ((c = getopt(argc, argv, "adhlr")) != -1) {
        switch (c) {
        case 'a':
            delay = launch = receipt = 1;
            break;
        case 'd':
            delay = 1;
            break;
        case 'l':
            launch = 1;
            delay = 0;
            break;
        case 'r':
            receipt = 1;
            delay = 0;
            break;
        case 'h':
        default:
            usage(argv[0]);
            return 0;
            break;
        }
    }

    // Source and receiver specified in cylindrical coordinates
    if (argc-optind == 3) {
        r = std::atof(argv[optind]);
        zRec = std::atof(argv[optind+1]);
        zSrc = std::atof(argv[optind+2]);
    }
    //
    // Source and receiver specified in Cartesian coordinates
    // NOTE: you can use the helper function Detector2Cylinder below to convert 
    // your own coordinates into the table coordinate system
    //
    else if (argc-optind == 7) {
        double coordSrc[3] = {std::atof(argv[optind]), std::atof(argv[optind+1]), std::atof(argv[optind+2])};
        double coordTrg[3] = {std::atof(argv[optind+3]), std::atof(argv[optind+4]), std::atof(argv[optind+5])};
        double zZero = std::atof(argv[optind+6]);
        if (Detector2Cylinder(coordSrc, coordTrg, zZero, &r, &zRec, &zSrc) != 0)
            std::cout << "ERROR: couldn't convert to cylindrical coordinates." << std::endl;
    }
    else {
        usage(argv[0]);
        return 0;
    }

    // Interface to the various spline tables
    // This object just needs to be created once
    RayTrace ray(tablePathStr);

    // Calculate the raytrace results requests
    if (delay)
        std::cout << ray.GetPropagationTime(r, zRec, zSrc) << " ";
    if (launch)
        std::cout << ray.GetLaunchAngle(r, zRec, zSrc) << " ";
    if (receipt)
        std::cout << ray.GetReceiveAngle(r, zRec, zSrc) << " ";        
    std::cout << std::endl;
    return 0;
}
