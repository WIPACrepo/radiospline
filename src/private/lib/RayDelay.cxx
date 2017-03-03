#include <iostream>
#include <string>
#include <cmath>
#include <radiospline/FirnShadow.h>
#include <radiospline/RayDelay.h>
#include <photospline/splinetable.h>

RayDelay::RayDelay(const std::string &icefit,
                   const std::string &airfit,
                   const std::string &firnfit) : icetable_(icefit),
                                                 airtable_(airfit),
                                                 shadow_(firnfit) { }
RayDelay::~RayDelay() { }

double RayDelay::GetPropagationTime(double r, double zRec, double zSrc) const {

    double coords[3] = {zSrc, zRec, r};
    int centers[3];
    double tdelay = -1;
    int derivatives = 0;
    
    // Is the source in air?
    if (zSrc > 0) {
        if (airtable_.is_supported(coords) && airtable_.searchcenters(coords, centers))
            tdelay = airtable_.ndsplineeval(coords, centers, derivatives);
    }
    // Is the source outside the firn shadow?
    else if (!shadow_.IsShadowed(r, zRec, zSrc)) {
        if (icetable_.is_supported(coords) && icetable_.searchcenters(coords, centers))
            tdelay = icetable_.ndsplineeval(coords, centers, derivatives);
    }

    // FIX ME deal with very small distances

    // Check that we're in the spline support region;
    // don't return a zero value
    if ((tdelay == 0.0) && 
        ((r > 0.0) || ((r == 0.0) && (fabs(zRec-zSrc) > 0.1))))
      return -1;
    else
      return tdelay;
}
