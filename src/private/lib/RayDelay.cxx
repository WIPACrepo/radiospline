#include <iostream>
#include <string>
#include <cmath>
#include <radiospline/FirnShadow.h>
#include <radiospline/RayDelay.h>
#include <radiospline/SplineTable.h>

RayDelay::RayDelay(const std::string &icefit,
                   const std::string &airfit,
                   const std::string &firnfit) : icetable_(icefit),
                                                 airtable_(airfit),
                                                 shadow_(firnfit) { }
RayDelay::~RayDelay() { }

double RayDelay::GetPropagationTime(double r, double zRec, double zSrc) const {

    double coords[3] = {zSrc, zRec, r};
    double tdelay = -1;

    // Is the source in air?
    if (zSrc > 0) {
        if (airtable_.IsSupported(coords))
            airtable_.Eval(coords, &tdelay);
    }
    // Is the source outside the firn shadow?
    else if (!shadow_.IsShadowed(r, zRec, zSrc)) {
        if (icetable_.IsSupported(coords))
            icetable_.Eval(coords, &tdelay);
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
