// SdrExtension.hpp

/** @file
 * Definitions for ExtensionData class
 */

#ifndef SDR_EXTENSION_HPP
#define SDR_EXTENSION_HPP

#include "htm/types/Sdr.hpp"

#include "cla/extension/types/ExtensionData.hpp"

namespace htm{

/**
 * SparseDistributedRepresentationExtension implementation in C++.
 * 
 * @b Description
 * 
 */
template<typename DataType>
class SparseDistributedRepresentationExtension:
	public SDR, // extended
	public ExtensionDataInterface<
		SparseDistributedRepresentationExtension<DataType>, 
		DataType
	> // implements
{

private:

	mutable ExtensionData<DataType> container_;

public:

	using ExtensionDataType = DataType;

	/**
	 * SparseDistributedRepresentationExtension constructor.
	 */
	SparseDistributedRepresentationExtension() = default;

	/**
	 * SparseDistributedRepresentationExtension constructor.
	 * 
	 * @param dimensions A list of dimension sizes, defining the shape of the
     * SDR.  The product of the dimensions must be greater than zero.
	 */
	SparseDistributedRepresentationExtension(const std::vector<UInt> &dimensions){
		initialize(dimensions);
	}

	/**
	 * SparseDistributedRepresentationExtension destructor
	 */
	~SparseDistributedRepresentationExtension() = default;

	/**
	 * Initialize SparseDistributedRepresentationExtension
	 * 
	 * @param dimensions A list of dimension sizes, defining the shape of the
     * SDR.  The product of the dimensions must be greater than zero.
	 */
	void initialize(const std::vector<UInt> &dimensions){
		SDR::initialize(dimensions);

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

	SparseDistributedRepresentationExtension<DataType>& operator=(
		const SparseDistributedRepresentationExtension<DataType>& value
	){
		if(dimensions.empty())
			initialize(value.dimensions);
			
		setSDR(value);
		container_.setDataSparse(value.getExDataSparse());

		return *this;
	}

};

template<typename DataType>
using SDRex = SparseDistributedRepresentationExtension<DataType>;

} // namespace ht


#endif // SDR_EXTENSION_HPP