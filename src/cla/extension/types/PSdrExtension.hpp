// PsdrExtension.hpp

/** @file
 * Definitions for PSdrExtension class
 */

#ifndef PSDR_EXTENSION_HPP
#define PSDR_EXTENSION_HPP

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

#include "htm/types/Sdr.hpp"
#include "cla/extension/types/ExtensionData.hpp"
#include "cla/extension/types/SdrExtension.hpp"
#include "cla/extension/types/Psdr.hpp"

namespace htm{


/**
 * PartialSDR_Extension implementation in C++.
 * 
 * @b Description
 * The PartialSDR_Extension (PSDRex) is the extended PSDR class. This class 
 * can split and concatenate SDRs and extension data. This class basically 
 * assumes that it splits and concatenates the SDRs which are same dimensions.
 */
template<typename DataType>
class PartialSDR_Extension :
	public PSDR, // extended

	public PartialInterface<
		PartialSDR_Extension<DataType>, SDRex<DataType>>, // implements
	
	public ExtensionDataInterface<
		PartialSDR_Extension<DataType>, DataType> // implements
{

private:

	mutable ExtensionData<DataType> container_;

public:

	using ExtensionDataType = DataType;

	/**
	 * PartialSDR_Extension constructor
	 */
	PartialSDR_Extension() = default;

	/**
	 * PartialSDR_Extension constructor
	 * 
	 * @param dimensions A list of dimension sizes, defining the shape of the
	 * SDR.  The product of the dimensions must be greater than zero.
	 * @param splitNum The number of split SDR. The splitNum must be greater
	 * than zero.
	 * @param axis The axis splitting a SDR in the dimensions. 
	 */
	PartialSDR_Extension(
		const std::vector<UInt>& dimensions,
		const UInt splitNum = 1u,
		const UInt axis = 0u
	){
		initialize(dimensions, splitNum, axis);
	}

	/**
	 * PartialSDR_Extension
	 * 
	 * @param sdrex The SDRex based on PSDRex
	 * @param splitNum The number of split SDR. The splitNum must be greater
	 * than zero.
	 * @param axis The axis splitting a SDR in the dimensions. 
	 */
	PartialSDR_Extension(
		const SDRex<DataType>& sdrex,
		const UInt splitNum = 1u,
		const UInt axis = 0u
	){
		initialize(sdrex.dimensions, splitNum, axis);
		setSDRex(sdrex);
	}

	/**
	 * PartialSDR_Extension destructor.
	 */
	~PartialSDR_Extension() = default;

	/**
	 * Initialize PartialSDR_Extension
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
	){
		PSDR::initialize(dimensions, splitNum, axis);
		container_.initialize(dimensions);
	}

	/**
	 * Clear the data sparse and fill data dense zero.
	 */
	void zero(){
		SDR::zero();
		container_.zero();
	}

	/**
	 * Split PSDRex
	 */
	const std::vector<SDRex<DataType>> split() const {
	
		getDense();

		std::vector<SDR_dense_t> denses(splitNum_);
		std::vector<std::vector<DataType>> dataDenses(splitNum_);

		std::vector<SDR_dense_t::iterator> denseBegins;
		std::vector<std::vector<DataType>::iterator> dataDenseBegins;

		const UInt subDenseSize = SDR(partialDimensions_).size;

		for(std::size_t i = 0u, size = denses.size(); i < size; ++i){
			denses.at(i).resize(subDenseSize);
			denseBegins.emplace_back(denses.at(i).begin());

			dataDenses.at(i).resize(subDenseSize);
			dataDenseBegins.emplace_back(dataDenses.at(i).begin());
		}

		PartialDenseFuncs::splitDense(
			dense_.begin(),
			dense_.end(),
			denseBegins,
			dimensions,
			splitNum_, axis_
		);

		const auto dataDense = getExDataDense();

		PartialDenseFuncs::splitDense(
			dataDense.begin(),
			dataDense.end(),
			dataDenseBegins,
			dimensions,
			splitNum_, axis_
		);

		std::vector<SDRex<DataType>> sdrexs(splitNum_);

		for(std::size_t i = 0u, size = sdrexs.size(); i < size; ++i){
			sdrexs.at(i).initialize(partialDimensions_);
			sdrexs.at(i).setDense(denses.at(i));
			sdrexs.at(i).setExDataDense(dataDenses.at(i));
		}

		return sdrexs;
	}

	/**
	 * Concatenate PSDRexs
	 * 
	 * @param psdrs The PSDRexs
	 */
	void concatenate(const std::vector<SDRex<DataType>>& sdrexs){
	
		NTA_CHECK(sdrexs.size() == static_cast<std::size_t>(splitNum_))
			<< "Not match the subspace num and sdrexs size.\n";

		// Check sdrs.
		// NTA_CHECK( sdrexs.size() >= 2u )
		// 	<< "Not enough sdrs to SDR::concatenate, need at least 2 SDRs got " << sdrexs.size() << "!";
		NTA_CHECK( axis_ < dimensions.size() );
			UInt concat_axis_size = 0u;

		for(const auto &sdrex : sdrexs ) {
			NTA_CHECK( sdrex.dimensions.size() == dimensions.size() )
				<< "All sdrs to SDR::concatenate must have the same number of dimensions as the output SDR!";
			for(auto dim = 0u; dim < dimensions.size(); dim++ ) {
				if( dim == axis_ ) {
					concat_axis_size += sdrex.dimensions.at(axis_);
				} else {
					NTA_CHECK( sdrex.dimensions.at(dim) == dimensions.at(dim) )
						<< "All dimensions except the axis must be the same!";
				}
			}
		}
		NTA_CHECK( concat_axis_size == dimensions.at(axis_))
			<< "Axis of concatenation dimensions do not match, sdrs sum to "
			<< concat_axis_size << ", output expects " << dimensions.at(axis_) << "!";
	
		std::vector<SDR_dense_t::iterator> denseBegins;
		std::vector<std::vector<DataType>> dataDenses;
		std::vector<std::vector<DataType>::iterator> dataDensesBegins;
		denseBegins.reserve(sdrexs.size());
		dataDenses.reserve(sdrexs.size());
		dataDensesBegins.reserve(sdrexs.size());


		for(const auto& sdrex : sdrexs){
			denseBegins.emplace_back(sdrex.getDense().begin());
			dataDenses.emplace_back(sdrex.getExDataDense());
			dataDensesBegins.emplace_back(dataDenses.back().begin());
		}

		zero();
		dense_.resize(size);
		std::vector<DataType> dataDense(size);

		PartialDenseFuncs::concatenateDense(
			dense_.begin(), dense_.end(),
			denseBegins, partialDimensions_,
			axis_
		);

		PartialDenseFuncs::concatenateDense(
			dataDense.begin(), dataDense.end(),
			dataDensesBegins, partialDimensions_,
			axis_
		);

		SDR::setDenseInplace();
		setExDataDense(dataDense);
	}

	void setSDRex(const SDRex<DataType>& sdrex){
		NTA_CHECK(size == sdrex.size)
			<< "PSDRex cannot set different size SDRex.";

		setSDR(sdrex);
		setExDataSparse(sdrex.getExDataSparse());
	}

	/**
	 * Set extension sparse data.
	 * 
	 * @param sparse The extension sparse data.
	 */
	void setExDataSparse(const std::vector<DataType>& sparse){
		container_.setDataSparse(sparse);
	}

	/**
	 * Set extension dense data.
	 * 
	 * @param dense The extension dense data.
	 */
	void setExDataDense(const std::vector<DataType>& dense){
		container_.setDataDense(dense, getSparse());
	}

	/**
	 * Set extension data container.
	 * 
	 * @param container The container of extension data.
	 */
	void setExtensionData(const ExtensionData<DataType>& container){
		container_.setDataSparse(container.getDataSparse());
	}

	/**
	 * Get the extension data sparse.
	 * 
	 * @return The extension data sparse.
	 */
	const std::vector<DataType>& getExDataSparse() const {
		return container_.getDataSparse();
	}

	/**
	 * Get the extension data dense.
	 * 
	 * @return The extension data sparse.
	 */
	const std::vector<DataType> getExDataDense() const {
		return container_.getDataDense(getSparse());
	}

	/**
	 * Get the container of extension data.
	 * 
	 * @return The container of extension data.
	 */
	const ExtensionData<DataType>& getExtensionData() const {
		return container_;
	}

	/**
	 * Get the mean value of extension data.
	 */
	const MeanType getExDataMean() const {
		return container_.getMean();
	}

	PartialSDR_Extension<DataType>& operator=(
		const PartialSDR_Extension<DataType>& value
	){
		if(dimensions.empty())
			initialize(value.dimensions, value.splitNum_, value.axis_);
			
		setSDR(value);
		container_.setDataSparse(value.getExDataSparse());
		
		return *this;
	}

	explicit operator SDRex<DataType>() const noexcept{
		SDRex<DataType> sdrex(dimensions);

		// Cast the data to CONST, which forces the SDR to copy the vector
		// instead of swapping it with its current data vector.  This protects
		// the input SDR from being changed.
		const SDR_sparse_t& sparse = getSparse();
		sdrex.setSparse(sparse);
		
		sdrex.setExDataSparse(getExDataSparse());

		return sdrex;
	}
};

template<typename DataType>
using PSDRex = PartialSDR_Extension<DataType>;
	
} // namespace htm


#endif // PSDR_EXTENSION_HPP