#include <cerrno>
#include <stdexcept>
#include <sstream>
#include <radiospline/SplineTable.h>
#include <photospline/bspline.h>

SplineTable::SplineTable(const std::string &path)
{
	if (readsplinefitstable(path.c_str(), &table_) != 0)
		throw std::runtime_error("Couldn't read spline table " + path);
	if (splinetable_read_key(&table_, SPLINETABLE_DOUBLE, "BIAS", &bias_))
		bias_ = 0;
}

SplineTable::~SplineTable()
{
	splinetable_free(&table_);
}

int
SplineTable::Eval(double *coordinates, double *result, const unsigned *derivatives) const
{
	int centers[table_.ndim];
	
	if (tablesearchcenters(&table_, coordinates, centers) == 0)
		*result = ndsplineeval_deriv(&table_, coordinates, centers, derivatives);
	else
		return EINVAL;
	
	// Subtract a constant bias if only if there is no differentiation involved
	bool subtract_bias = true;
	if (derivatives != NULL) {
		for (int i=0; i < table_.ndim; i++) {
			if (derivatives[i] > 0) {
				subtract_bias = true;
				break;
			}
		}
	}
	if (subtract_bias)
	*result -= bias_;
	
	return 0;
}

std::pair<double, double>
SplineTable::GetExtents(int dim) const
{
	if (dim < 0 || dim >= table_.ndim)
		throw std::out_of_range("Dimension index out of range");
	return std::make_pair(table_.extents[dim][0], table_.extents[dim][1]);
}

bool
SplineTable::IsSupported(double *coordinates) const
{
    bool supported = true;
    for (int i=0; i < table_.ndim; i++) {
        supported &= (coordinates[i] >= table_.extents[i][0]);
        supported &= (coordinates[i] <= table_.extents[i][1]);
    }
    return supported;
}

double
SplineTable::GetField(const std::string &key) const
{
	double v = 0;

	if (splinetable_read_key(&table_, SPLINETABLE_DOUBLE, key.c_str(), &v)) {
		std::ostringstream msg;
		msg << "Invalid header key " << key;
		throw std::invalid_argument(msg.str());
	}
		
	return v;
}

