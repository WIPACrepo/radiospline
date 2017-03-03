#include <string>
#include <radiospline/IceGeometry.h>
#include <radiospline/FirnShadow.h>
#include <photospline/splinetable.h>

FirnShadow::FirnShadow(const std::string &path) : table_(path) { }
FirnShadow::~FirnShadow() { }

double FirnShadow::GetShadowDepth(double r, double zRec) const {
    double coords[2] = {zRec, r};
    int centers[2];
    double result = 0;
    int derivatives = 0;
    if ((zRec < ICE_Z_SURFACE) && (r >= 0))
        if (table_.searchcenters(coords, centers))
            result = table_.ndsplineeval(coords, centers, derivatives);

    return result;
}

bool FirnShadow::IsShadowed(double r, double zRec, double zSrc) const {
    return (zSrc < ICE_Z_SURFACE) && (zSrc >= this->GetShadowDepth(r, zRec));
}
