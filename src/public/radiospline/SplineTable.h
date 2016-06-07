
#ifndef RADIOSPLINE_SPLINETABLE_H_INCLUDED
#define RADIOSPLINE_SPLINETABLE_H_INCLUDED

#include <string>
#include <photospline/splinetable.h>

class SplineTable {
public:
	/**
	 * @param[in] path Path to a FITS file
	 */ 
	SplineTable(const std::string &path);
	virtual ~SplineTable();

	/** Convolve with a B-spline along the given dimension */
	void Convolve(int dim, const double *knots, size_t n_knots);

	/** Evaluate the spline surface
	 * 
	 * @param[in]       x N-dimensonal coordinates at which to evaluate
	 * @param[out] result Value of spline surface at coordinates
	 * @param[in] derivatives An array indicating the type of basis to use
	                          in each dimension. If an entry corresponding to
	                          a dimension is N>0, the basis in that
	                          dimension will consist of the Nth derivatives of
	                          the usual B-spline basis, and result
	                          will be the Nth-order gradient of the surface in
	                          that dimension. If NULL, 0 will be assumed.
	 * @returns 0 on success, non-zero otherwise
	 */
	int Eval(double *x, double *result, const unsigned *derivatives=NULL) const;

	/** Get the number of dimensions */
	unsigned GetNDim() const { return table_.ndim; };
	/** Get the extent of full support in dimension dim */
	std::pair<double, double> GetExtents(int dim) const;
    /** Return whether all coordinates are within the table extents */
    bool IsSupported(double *coordinates) const;
	
	/** Retrieve a value stored in the FITS header */
	double GetField(const std::string &key) const;
private:
	SplineTable(const SplineTable&);
	
	struct splinetable table_;
	double bias_;
};

#endif
