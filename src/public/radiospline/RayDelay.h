#ifndef RADIOSPLINE_RAYDELAY_H_INCLUDED
#define RADIOSPLINE_RAYDELAY_H_INCLUDED

#include <string>
#include <photospline/splinetable.h>
#include <radiospline/FirnShadow.h>

class RayDelay {
 public:
	/**
	 * @param[in] icefit Path to a FITS file for in-ice raytrace solutions
     * @param[in] airfit Path to a FITS file for in-air raytrace solutions
     * @param[in] firnfit Path to a FITS file for firn shadow boundary
	 */
    RayDelay(const std::string &icefit,
             const std::string &airfit,
             const std::string &firnfit);    
    ~RayDelay();

    /** Get time delay of raytrace from source to receiver
     *
     * @param[in]  r horizontal distance to emitter, m
     * @param[in]  zRec depth of receiver, m (negative)
     * @param[in]  zSrc depth of source, m
     * @param[out] tdelay propagation delay in ns (negative if no solution) 
     */
    double GetPropagationTime(double r, double zRec, double zSrc) const;

 private:
    photospline::splinetable<> icetable_;
    photospline::splinetable<> airtable_;
    FirnShadow shadow_;
};

#endif
