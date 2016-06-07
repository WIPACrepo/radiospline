#include <string>
#include <radiospline/IceGeometry.h>
#include <radiospline/FirnShadow.h>
#include <radiospline/SplineTable.h>

FirnShadow::FirnShadow(const std::string &path) : table_(path) { }
FirnShadow::~FirnShadow() { }

double FirnShadow::GetShadowDepth(double r, double zRec) const {
    double coords[2] = {zRec, r};
    double result = 0;
    if ((zRec < ICE_Z_SURFACE) && (r >= 0))
        table_.Eval(coords, &result);

    return result;
}

bool FirnShadow::IsShadowed(double r, double zRec, double zSrc) const {
    return (zSrc < ICE_Z_SURFACE) && (zSrc >= this->GetShadowDepth(r, zRec));
}
