#include <iostream>
#include <cstdlib>
#include "radiospline/FirnShadow.h"

#define SHADOW_FILE "firn_shadow.fits"

int main(int argc, char **argv) {

    // Check that we know where to find the table
    char * tablePath = getenv("RADIOSPLINE_TABLE_DIR");
    if (tablePath == NULL) {
        std::cout << "ERROR: please point the RADIOSPLINE_TABLE_DIR environment variable to";
        std::cout << " the spline .fits file diectory." << std::endl;
        return -1;
    }
    std::string tablePathStr(tablePath);

    // Get command line arguments
    if ((argc != 3) && (argc != 4)) {
        std::cout << "Usage: " << argv[0] << " radius z_receiver <z_source>" << std::endl;
        return 0;
    }
    double r = std::atof(argv[1]);
    double zRec = std::atof(argv[2]);

    // If the user supplied a source depth, check if it's the firn shadow or not
    double zSrc;
    bool checkIsShadowed = (argc == 4);
    if (checkIsShadowed)
        zSrc = std::atof(argv[3]);
    
    // The firn shadow spline table
    FirnShadow shadow(tablePathStr+"/"+SHADOW_FILE);
   
    std::cout << "Firn shadow depth at (r, zRec) = (" << r << " m, " << zRec << " m): "
              << shadow.GetShadowDepth(r, zRec) << " m" << std::endl;

    if (checkIsShadowed) {
        std::cout << "Source at (r, zSrc) = (" << r << " m, " << zSrc << " m): "
                  << (shadow.IsShadowed(r, zRec, zSrc) ? "is" : "is NOT")
                  << " in the firn shadow for a receiver at " << zRec << " m" << std::endl;
    }
    return 0;
}
