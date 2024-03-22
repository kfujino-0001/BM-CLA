// Psdr.cpp

/** 
 * @file
 * Implementation of PartialSdr
 */

#include <utility>
#include <numeric>
#include <algorithm>

#include "cla/extension/types/Psdr.hpp"

namespace htm{

template<typename T>
bool operator==(const std::vector<T>& vec1, const std::vector<T>& vec2){
	if(vec1.size() != vec2.size())return false;

	for(std::size_t i = 0u, size = vec1.size(); i < size; ++i)
		if(vec1.at(i) != vec2.at(i)) return false;

	return true;
}

/**
 * PartialDenseFuncs
 */

const std::vector<UInt> PartialDenseFuncs::splitDimensions(
	const std::vector<UInt>& dimensions,
	const UInt splitNum,
	const UInt axis
){
	NTA_CHECK(dimensions.size() > 0u)
		<< "There is not dimensions!";

	// Calculate the SDR's size.
	const UInt size 
		= std::accumulate(
			dimensions.begin(), dimensions.end(), 
			1u, std::multiplies<int>()
		);

	// Special case: placeholder for SDR type used in NetworkAPI
	if(dimensions != std::vector<UInt>{0}) {
		NTA_CHECK(size > 0) 
			<< "all dimensions must be > 0";
	}

	NTA_CHECK(splitNum > 0u) 
		<< "split num must be > 0";
	NTA_CHECK(axis < dimensions.size()) 
		<< "Index out of bounds of this dimensions";
	NTA_CHECK((dimensions.at(axis) % splitNum) == 0u)
		<< "This dimensions cannot be splitted properly.";

	std::vector<UInt> dim(dimensions.begin(), dimensions.end());
	dim.at(axis) = dim.at(axis) / splitNum;

	return dim;
}



/**
 * PartialSDR methods
 */
PartialSDR::PartialSDR(
	const std::vector<UInt>& dimensions,
	const UInt splitNum,
	const UInt axis	
){
	initialize(dimensions, splitNum, axis);
}

PartialSDR::PartialSDR(
	const SDR& sdr,
	const UInt splitNum,
	const UInt axis
){
	initialize(sdr.dimensions, splitNum, axis);
	setSDR(sdr);
}

void PartialSDR::initialize(
	const std::vector<UInt>& dimensions,
	const UInt splitNum,
	const UInt axis
){
	SDR::initialize(dimensions);

	splitNum_ = splitNum;
	axis_ = axis;

	partialDimensions_
		= PartialDenseFuncs::splitDimensions(
			dimensions, splitNum_, axis_
		);
}

const std::vector<SDR> PartialSDR::split() const {
	getDense();
	
	std::vector<SDR_dense_t> denseVec(splitNum_);
	std::vector<SDR_dense_t::iterator> denseBegins;

	const UInt subDenseSize = SDR(partialDimensions_).size;

	for(auto& dense : denseVec){
		dense.resize(subDenseSize);
		denseBegins.emplace_back(dense.begin());
	}

	PartialDenseFuncs::splitDense(
		dense_.begin(), dense_.end(),
		denseBegins, 
		dimensions,
		splitNum_, axis_
	);

	std::vector<SDR> sdrs(splitNum_);

	for(std::size_t i = 0u, size = sdrs.size(); i < size; ++i){
		sdrs.at(i).initialize(partialDimensions_);
		sdrs.at(i).setDense(denseVec.at(i));
	}

	return sdrs;
}

void PartialSDR::concatenate(const std::vector<SDR>& sdrs) {
	// Check sdrs.
	// NTA_CHECK( sdrs.size() >= 2u )
	// 	<< "Not enough sdrs to SDR::concatenate, need at least 2 SDRs got " << sdrs.size() << "!";
	NTA_CHECK( axis_ < dimensions.size() );
		UInt concat_axis_size = 0u;

	for( const auto &sdr : sdrs ) {
		NTA_CHECK( sdr.dimensions.size() == dimensions.size() )
			<< "All sdrs to SDR::concatenate must have the same number of dimensions as the output SDR!";
		for( auto dim = 0u; dim < dimensions.size(); dim++ ) {
			if( dim == axis_ ) {
				concat_axis_size += sdr.dimensions.at(axis_);
			} else {
				NTA_CHECK( sdr.dimensions.at(dim) == dimensions.at(dim) )
					<< "All dimensions except the axis must be the same!";
			}
		}
	}
	NTA_CHECK( concat_axis_size == dimensions.at(axis_))
		<< "Axis of concatenation dimensions do not match, sdrs sum to "
		<< concat_axis_size << ", output expects " << dimensions.at(axis_) << "!";


	std::vector<SDR_dense_t::iterator> denseBegins;
	denseBegins.reserve(sdrs.size());

	for(auto& sdr : sdrs){
		denseBegins.emplace_back(sdr.getDense().begin());
	}

	zero();
	dense_.resize(size);

	PartialDenseFuncs::concatenateDense(
		dense_.begin(), dense_.end(),
		denseBegins, partialDimensions_,
		axis_
	);

	SDR::setDenseInplace();
}

void PartialSDR::setSDR(const SDR& sdr){
	NTA_CHECK(size == sdr.size)
		<< "PSDR cannot set different size SDR.";

	// Cast the data to CONST, which forces the SDR to copy the vector
	// instead of swapping it with its current data vector.  This protects
	// the input SDR from being changed.
	const SDR_sparse_t& sparse = sdr.getSparse();
	setSparse(sparse);
}


} // namespace htm