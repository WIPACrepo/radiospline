#include <iostream>
#include <string>
#include <cmath>
#include <radiospline/FirnShadow.h>
#include <radiospline/RayTrace.h>
#include <photospline/splinetable.h>

RayTraceResult::RayTraceResult(const std::string &icefit,
                               const std::string &airfit,
                               const std::string &firnfit) : icetable_(icefit),
                                                             airtable_(airfit),
                                                             shadow_(firnfit) { }
RayTraceResult::~RayTraceResult() { }

double RayTraceResult::GetResult(double r, double zRec, double zSrc) const {

    double coords[3] = {zSrc, zRec, r};
    int centers[3];
    double result = -1;
    int derivatives = 0;

    // FIX ME small distances are still likely bogus
    if ((r == 0.0) && (fabs(zRec-zSrc) > 0.1))
      return -1;

    // Is the source in air?
    if (zSrc > 0) {
        if (airtable_.is_supported(coords) && airtable_.searchcenters(coords, centers))
            result = airtable_.ndsplineeval(coords, centers, derivatives);
    }
    // Is the source outside the firn shadow?
    else if (!shadow_.IsShadowed(r, zRec, zSrc)) {
        if (icetable_.is_supported(coords) && icetable_.searchcenters(coords, centers))
            result = icetable_.ndsplineeval(coords, centers, derivatives);
    }

    return result;
}
