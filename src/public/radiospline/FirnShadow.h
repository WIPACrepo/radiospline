#ifndef RADIOSPLINE_FIRNSHADOW_H_INCLUDED
#define RADIOSPLINE_FIRNSHADOW_H_INCLUDED

#include <string>
#include <photospline/splinetable.h>

class FirnShadow {
 public:
	/**
	 * @param[in] path Path to a FITS file
	 */
    FirnShadow(const std::string &path);    
    ~FirnShadow();

    /** Get depth of firn shadow boundary
     *
     * @param[in]  r horizontal distance to emitter, m
     * @param[in]  zRec depth of receiver, m (negative)
     * @param[out] result depth of firn shadow boundary, m
     */
    double GetShadowDepth(double r, double zRec) const;

    /** Determine whether a point is in the firn shadow
     * 
     * @param[in] r horizontal distance to emitter, m
     * @param[in] z depth of emitter, m
     * @param[in] zRec depth of receiver, m
     * @param[out] result true if emitter is in shadow, false otherwise
     */
    bool IsShadowed(double r, double z, double zRec) const;

 private:
    photospline::splinetable<> table_;
};

#endif
