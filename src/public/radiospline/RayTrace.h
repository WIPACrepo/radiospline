#ifndef RADIOSPLINE_RAYTRACE_H_INCLUDED
#define RADIOSPLINE_RAYTRACE_H_INCLUDED

#include <string>
#include <photospline/splinetable.h>
#include <radiospline/FirnShadow.h>

class RayTraceResult {
 public:
	/**
	 * @param[in] icefit Path to a FITS file for in-ice raytrace solution
     * @param[in] airfit Path to a FITS file for in-air raytrace solution
     * @param[in] firnfit Path to a FITS file for firn shadow boundary
	 */
    RayTraceResult(const std::string &icefit,
             const std::string &airfit,
             const std::string &firnfit);    
    ~RayTraceResult();

    /** Get result of raytrace from source to receiver
     *
     * @param[in]  r horizontal distance to emitter, m
     * @param[in]  zRec depth of receiver, m (negative)
     * @param[in]  zSrc depth of source, m
     * @param[out] raytrace result
     */
    double GetResult(double r, double zRec, double zSrc) const;

 private:
    photospline::splinetable<> icetable_;
    photospline::splinetable<> airtable_;
    FirnShadow shadow_;
};

class RayDelay : public RayTraceResult {
public:
    RayDelay(const std::string &icefit,
             const std::string &airfit,
             const std::string &firnfit) : RayTraceResult(icefit, airfit, firnfit) { }
    
    /** Get time delay of raytrace from source to receiver
     *
     * @param[in]  r horizontal distance to emitter, m
     * @param[in]  zRec depth of receiver, m (negative)
     * @param[in]  zSrc depth of source, m
     * @param[out] tdelay propagation delay in ns (negative if no solution) 
     */
    double GetPropagationTime(double r, double zRec, double zSrc) {
        double v = GetResult(r, zRec, zSrc);
        // Don't return zero results
        if (v == 0)
            return -1;
        return v;
    }
};

class RayLaunch : public RayTraceResult {
public:
    RayLaunch(const std::string &icefit,
             const std::string &airfit,
             const std::string &firnfit) : RayTraceResult(icefit, airfit, firnfit) { }
    
    /** Get launch angle of raytrace from source to receiver
     *
     * @param[in]  r horizontal distance to emitter, m
     * @param[in]  zRec depth of receiver, m (negative)
     * @param[in]  zSrc depth of source, m
     * @param[out] launch zenith angle from source, radians (negative if no solution) 
     */
    double GetLaunchAngle(double r, double zRec, double zSrc) {
        return GetResult(r, zRec, zSrc);
    }
};

class RayReceive : public RayTraceResult {
public:
    RayReceive(const std::string &icefit,
               const std::string &airfit,
               const std::string &firnfit) : RayTraceResult(icefit, airfit, firnfit) { }
    
    /** Get receipt angle of raytrace from source to receiver
     *
     * @param[in]  r horizontal distance to emitter, m
     * @param[in]  zRec depth of receiver, m (negative)
     * @param[in]  zSrc depth of source, m
     * @param[out] receipt zenith angle from source, radians (negative if no solution) 
     */
    double GetReceiveAngle(double r, double zRec, double zSrc) {
        return GetResult(r, zRec, zSrc);
    }
};

class RayTrace {
public:
    RayTrace(const std::string &path) : delay_(path+"/"+"delay_inice.fits",
                                               path+"/"+"delay_inair.fits",
                                               path+"/"+"firn_shadow.fits"
                                               ),
                                        delay_reflected_(path+"/"+"delay_inice_reflected.fits",
                                                         path+"/"+"delay_inair.fits",
                                                         path+"/"+"firn_shadow.fits"),
                                        launch_(path+"/"+"launch_inice.fits",
                                                path+"/"+"launch_inair.fits",
                                                path+"/"+"firn_shadow.fits"),
                                        launch_reflected_(path+"/"+"launch_inice_reflected.fits",
                                                          path+"/"+"launch_inair.fits",
                                                          path+"/"+"firn_shadow.fits"),
                                        receive_(path+"/"+"receipt_inice.fits",
                                                 path+"/"+"receipt_inair.fits",
                                                 path+"/"+"firn_shadow.fits"),
                                        receive_reflected_(path+"/"+"receipt_inice_reflected.fits",
                                                           path+"/"+"receipt_inair.fits",
                                                           path+"/"+"firn_shadow.fits") { }

    ~RayTrace() { }

    /** Get time delay of raytrace from source to receiver
     *
     * @param[in]  r horizontal distance to emitter, m
     * @param[in]  zRec depth of receiver, m (negative)
     * @param[in]  zSrc depth of source, m
     * @param[out] tdelay propagation delay in ns (negative if no solution) 
     */
    double GetPropagationTime(double r, double zRec, double zSrc) {
        return delay_.GetPropagationTime(r, zRec, zSrc);
    }

    /** Get time delay of raytrace from source to receiver (secondary path)
     *
     * @param[in]  r horizontal distance to emitter, m
     * @param[in]  zRec depth of receiver, m (negative)
     * @param[in]  zSrc depth of source, m
     * @param[out] tdelay propagation delay in ns (negative if no solution) 
     */
    double GetReflectedPropagationTime(double r, double zRec, double zSrc) {
        if (zSrc < 0)
            return delay_reflected_.GetPropagationTime(r, zRec, zSrc);
        else
            return delay_.GetPropagationTime(r, zRec, zSrc);
    }

    
    /** Get launch angle of raytrace from source to receiver
     *
     * @param[in]  r horizontal distance to emitter, m
     * @param[in]  zRec depth of receiver, m (negative)
     * @param[in]  zSrc depth of source, m
     * @param[out] launch zenith angle from source, radians (negative if no solution) 
     */        
    double GetLaunchAngle(double r, double zRec, double zSrc) {
        return launch_.GetLaunchAngle(r, zRec, zSrc);
    }

    /** Get receipt angle of raytrace from source to receiver
     *
     * @param[in]  r horizontal distance to emitter, m
     * @param[in]  zRec depth of receiver, m (negative)
     * @param[in]  zSrc depth of source, m
     * @param[out] receipt zenith angle from source, radians (negative if no solution) 
     */    
    double GetReceiveAngle(double r, double zRec, double zSrc) {
        return receive_.GetReceiveAngle(r, zRec, zSrc);
    }

    /** Get launch angle of raytrace from source to receiver (secondary path)
     *
     * @param[in]  r horizontal distance to emitter, m
     * @param[in]  zRec depth of receiver, m (negative)
     * @param[in]  zSrc depth of source, m
     * @param[out] launch zenith angle from source, radians (negative if no solution) 
     */        
    double GetReflectedLaunchAngle(double r, double zRec, double zSrc) {
        if (zSrc < 0)
            return launch_reflected_.GetLaunchAngle(r, zRec, zSrc);
        else
            return launch_.GetLaunchAngle(r, zRec, zSrc);
    }

    /** Get receipt angle of raytrace from source to receiver (secondary path)
     *
     * @param[in]  r horizontal distance to emitter, m
     * @param[in]  zRec depth of receiver, m (negative)
     * @param[in]  zSrc depth of source, m
     * @param[out] receipt zenith angle from source, radians (negative if no solution) 
     */    
    double GetReflectedReceiveAngle(double r, double zRec, double zSrc) {
        if (zSrc < 0)
            return receive_reflected_.GetReceiveAngle(r, zRec, zSrc);
        else
            return receive_.GetReceiveAngle(r, zRec, zSrc);            
    }

private:
    RayDelay delay_;
    RayDelay delay_reflected_;
    RayLaunch launch_;
    RayLaunch launch_reflected_;
    RayReceive receive_;
    RayReceive receive_reflected_;
};


#endif
