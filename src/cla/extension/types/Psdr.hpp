// Psdr.hpp

/** @file
 * Definitions for PartialSDR class
 */

#ifndef PARTIAL_SDR_HPP
#define PARTIAL_SDR_HPP

#include <algorithm>

#include "htm/types/Sdr.hpp"

namespace htm{

/**
 * PartialDenseFuncs implementation in C++.
 * 
 * @b Description
 * The PartialDenseFuncs is the packages of splitter and concatenator of dense.
 * These functions copy data with iterator while referring to the partial
 * dimensions and axis. These functions assume that the arg dense have been allocated.
 */
struct PartialDenseFuncs {

	/**
	 * Split dense
	 * 
	 * @param denseBegin The splitted dense iterator begin.
	 * @param denseEnd The splitted dense iterator end.
	 * @param subBegins The begin iterators of subspace dense. 
	 * @param dimensions The dimensions of dense.
	 * @param splitNum The number of split dense. The splitNum must be greater
	 * than zero.
	 * @param axis The axis splitting a dense in the dimensions. 
	 */
	template<typename Iterator, typename Iterators>
	static void splitDense(
		Iterator denseBegin, Iterator denseEnd,
		Iterators subBegins,
		const std::vector<UInt>& dimensions,
		const UInt splitNum = 1u,
		const UInt axis = 0u
	){
		const auto& subDims 
			= splitDimensions(dimensions, splitNum, axis);

		UInt row = 1u;
		for(UInt d = axis, size = static_cast<UInt>(subDims.size()); d < size; ++d)
			row *= subDims.at(d);

		while(denseBegin < denseEnd){
			for(UInt i = 0u, size = static_cast<UInt>(subBegins.size()); i < size; ++i){
				const auto& buf = subBegins.at(i);
				std::copy(denseBegin, denseBegin + row, buf);
				subBegins.at(i) += row;
				denseBegin += row;
			}
		}
	}

	/**
	 * Concatenate dense
	 * 
	 * @param denseBegin The concatenated dense iterator begin.
	 * This function copy the subspace dense to this iterator.
	 * @param denseEnd The concatenated dense iterator end.
	 * @param subBegins The begin iterators of subspace dense. 
	 * @param subDims The dimensions of subspace dense.
	 * @param axis The axis splitting a dense in the dimensions.
	 */
	template<typename Iterator, typename Iterators>
	static void concatenateDense(
		Iterator denseBegin, Iterator denseEnd,
		Iterators subBegins,
		const std::vector<UInt>& subDims,
		const UInt axis = 0u
	){
		UInt row = 1u;
		for(UInt d = axis, size = static_cast<UInt>(subDims.size()); d < size; ++d)
			row *= subDims.at(d);

		while(denseBegin < denseEnd){
			for(UInt i = 0u, size = static_cast<UInt>(subBegins.size()); i < size; ++i){
				const auto& buf = subBegins.at(i);
				std::copy(buf, buf + row, denseBegin);
				subBegins.at(i) += row;
				denseBegin += row;
			}
		}
	}

	/**
	 * Split the dimension. Any other class can use this function.
	 * 
	 * @param dimensions A list of dimension sizes, defining the shape of the
	 * SDR.  The product of the dimensions must be greater than zero.
	 * @param splitNum The number of split dense. The splitNum must be greater
	 * than zero.
	 * @param axis The axis splitting a dense in the dimensions. 
	 */
	static const std::vector<UInt> splitDimensions(
		const std::vector<UInt>& dimensions,
		const UInt splitNum = 1u,
		const UInt axis = 0u
	);
};


/**
 * PartialInterface definition in C++.
 * 
 * @b Description
 * The partialInterface is the static interface of Partial data.
 */
template<typename ClassType, typename ChildClassType>
class PartialInterface{

public:
	PartialInterface() = default;
	~PartialInterface() = default;

	const std::vector<ChildClassType> split() const {
		static_cast<ClassType&>(this)->split();
	}

	void concatenate(const std::vector<ChildClassType>& insts){
		static_cast<ClassType&>(this)->concatenate(insts);
	}
};



/**
 * PartialSDR implementation in C++.
 * 
 * @b Description
 * The PartialSDR can split and concatenate SDRs. This class basically 
 * assumes that it splits and concatenates the SDRs which are same dimensions.
 */
class PartialSDR: 
	public SDR, // extended
	public PartialInterface<PartialSDR, SDR> // implements
{

protected:

	UInt splitNum_;
	UInt axis_;

	std::vector<UInt> partialDimensions_;

public:

	/**
	 * PartialSDR constructor
	 */
	PartialSDR() = default;

	/**
	 * PartialSDR constructor
	 * 
	 * @param dimensions A list of dimension sizes, defining the shape of the
	 * SDR.  The product of the dimensions must be greater than zero.
	 * @param splitNum The number of split SDR. The splitNum must be greater
	 * than zero.
	 * @param axis The axis splitting a SDR in the dimensions. 
	 */
	PartialSDR(
		const std::vector<UInt>& dimensions,
		const UInt splitNum = 1u,
		const UInt axis = 0u	
	);

	/**
	 * PartialSDR
	 * 
	 * @param sdr The SDR based on PartialSDR.
	 * @param splitNumThe number of split SDR. The splitNum must be greater
	 * than zero.
	 * @param axis The axis splitting a SDR in the dimensions. 
	 */
	PartialSDR(
		const SDR& sdr,
		const UInt splitNum = 1u,
		const UInt axis = 0u
	);

	/**
	 * PartialSDR destructor
	 */
	virtual ~PartialSDR() = default;

	/**
	 * Initialize PartialSDR
	 * 
	 * @param dimensions A list of dimension sizes, defining the shape of the
	 * SDR.  The product of the dimensions must be greater than zero.
	 * @param splitNum The number of split SDR. The splitNum must be greater
	 * than zero.
	 * @param axis The axis splitting a SDR in the dimensions. 
	 */
	void initialize(
		const std::vector<UInt>& dimensions,
		const UInt splitNum = 1u,
		const UInt axis = 0u
	);

	/**
	 * Split this PartialSDR for some SDR.
	 * 
	 * @return Some SDRs.
	 */
	const std::vector<SDR> split() const;

	/**
	 * Concatenate some SDR.
	 * 
	 * @param sdrs Some SDR.
	 */
	void concatenate(const std::vector<SDR>& sdrs);

	/**
	 * Set SDR.
	 */
	void setSDR(const SDR& sdr);

	/**
	 * Get the partial dimensions.
	 */
	const std::vector<UInt>& getPartialDimensions() const {
		return partialDimensions_;
	}

	/**
	 * Get the number of splits in this partial SDR.
	 */
	const UInt getSplitNum() const {
		return splitNum_;
	}

	explicit operator SDR() const noexcept {
		SDR sdr(dimensions);
		sdr.setSparse(getSparse());

		return sdr;
	}

};

using PSDR = PartialSDR;

} // namespace htm

#endif // PARTIAL_SDR_HPP