#ifndef RADIOSPLINE_ICEGEOMETRY_H_INCLUDED
#define RADIOSPLINE_ICEGEOMETRY_H_INCLUDED

// Where the ice surface and bedrock are in the coordinate system
// of the spline tables and raytracer
#define ICE_Z_SURFACE     0.0
#define ICE_Z_BEDROCK -2832.0

/** Translate from a detector cartesian coordinate system
 * to a cylindrical coordinate system centered on the receiver.
 * The latter is used in the spline tables and assumes azimuthal
 * symmetry.
 *
 * @param[in]  coordSrc 3-dim array of source (x,y,z)
 * @param[in]  coordTrg 3-dim array of target (x,y,z)
 * @param[in], zZero z-coordinate of detector origin relative to ice surface (negative)
 * @param[out]  r horizontal distance to source, m
 * @param[out]  zRec depth of receiver, m (negative)
 * @param[out]  zSrc depth of source, m
 */
int Detector2Cylinder(double *coordSrc, double *coordTrg,
                      double zZero,
                      double *r, double *zRec, double *zSrc);

#endif

