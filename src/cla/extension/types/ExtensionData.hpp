// ExtensionData.hpp

/** @file
 * Definitions for ExtensionData class
 */

#ifndef EXTENSION_DATA_HPP
#define EXTENSION_DATA_HPP

#include <iostream>
#include <vector>

#include "htm/types/Sdr.hpp"

namespace htm {

using NumCells = UInt;
using MeanType = double;


/**
 * ExtensionData implementation in C++.
 * 
 * @b Description
 * The ExtensionData is the container class to add the extensional data in
 * the SDR. 
 */
template<typename DataType>
class ExtensionData {

private:

	mutable std::vector<DataType> dataSparse_;
	
	std::vector<UInt> dimensions_;
	UInt size_;

public:

	const std::vector<UInt>& dimensions = dimensions_;
	const UInt& size = size_;

public:

	/**
	 * ExtensionData constructor
	 */
	ExtensionData() = default;

	/**
	 * ExtensionData constructor.
	 * 
	 * @param dimensions A list of dimension sizes, defining the shape of the
	 * SDR.  The product of the dimensions must be greater than zero.
	 */
	ExtensionData(
		const std::vector<UInt>& dimensions
	){
		initialize(dimensions);
	}

	/**
	 * Initialize ExtensionData
	 * 
	 * @param dimensions A list of dimension sizes, defining the shape of the
	 * SDR.  The product of the dimensions must be greater than zero.
	 */
	void initialize(
		const std::vector<UInt>& dimensions
	){
		dimensions_ = dimensions;
		
		size_ = SDR(dimensions_).size;
		zero();
	}

	/**
	 * Clear the data sparse and fill data dense zero.
	 */
	void zero(){
		dataSparse_.clear();
	}

	/**
	 * Get data dense. 
	 * 
	 * @param sparse The key of data dense. This vector is from SDR sparse.
	 * 
	 * @return data dense.
	 */
	const std::vector<DataType> getDataDense(const SDR_sparse_t& sparse) const {
		NTA_CHECK(dataSparse_.size() == sparse.size())
			<< "Not match size.";

		std::vector<DataType> dataDense(size_, static_cast<DataType>(0));

		for(std::size_t i = 0u, size = sparse.size(); i < size; ++i)
			dataDense.at(sparse.at(i)) = dataSparse_.at(i);

		return dataDense;
	}

	/**
	 * Get data sparse. If the ExtensionData does not have the data sparse,
	 * it calculates the data sparse from data dense and master(SDR) sparse.
	 *
	 * @return data sparse.
	 */
	const std::vector<DataType>& getDataSparse() const {	
		return dataSparse_;
	}

	/**
	 * Get the mean value of sparse data.
	 */
	const MeanType getMean() const {
		if(dataSparse_.size() == 0) return 0.0;

		return std::accumulate(
			dataSparse_.begin(), dataSparse_.end(),
			static_cast<DataType>(0)
		) / static_cast<MeanType>(dataSparse_.size());
	}

	/**
	 * Set data dense.
	 * 
	 * @param dataDense the data dense.
	 */
	void setDataDense(
		const std::vector<DataType>& dataDense, 
		const SDR_sparse_t& sparse
	){
		NTA_CHECK(dataDense.size() == static_cast<std::size_t>(size_))
			<<"Different size dense was inputted\n";

		dataSparse_.clear();
		dataSparse_.reserve(sparse.size());

		for(const ElemSparse& idx : sparse)
			dataSparse_.emplace_back(dataDense.at(idx));
	}

	/**
	 * Set data sparse.
	 * 
	 * @param dataSparse the data sparse.
	 */
	void setDataSparse(const std::vector<DataType>& dataSparse){
		dataSparse_.resize(dataSparse.size());
		dataSparse_.assign(dataSparse.begin(), dataSparse.end());
	}

	/**
	 * set and copy data.
	 */
	void setExtensionData(const ExtensionData<DataType>& data){
		initialize(data.dimensions);
		setDataSparse(data.getDataSparse());
	}

};



/**
 * ExtensionDataInterface implementation in C++.
 * 
 * @b Description
 * The ExtensionDataInterface is the static interface of extension data.
 */
template<typename ClassType, typename DataType>
class ExtensionDataInterface {

public:

	ExtensionDataInterface() = default;
	~ExtensionDataInterface() = default;

	void setExDataSparse(const std::vector<DataType>& sparse){
		static_cast<ClassType&>(this)->setExDataSparse(sparse);
	}

	void setExDataDense(const std::vector<DataType>& dense){
		static_cast<ClassType&>(this)->setExDataDense(dense);
	}

	void setExtensionData(const ExtensionData<DataType>& container){
		static_cast<ClassType&>(this)->setExtensionData(container);
	}

	const std::vector<DataType>& getExDataSparse() const {
		return static_cast<ClassType&>(this)->getExDataSparse();
	}

	const std::vector<DataType> getExDataDense() const {
		return static_cast<ClassType&>(this)->getExDataDense();
	}

	const ExtensionData<DataType>& getExtensionData() const {
		return static_cast<ClassType&>(this)->getExtensionData();
	}

	const MeanType getExDataMean() const {
		return static_cast<ClassType&>(this)->getExDataMean();
	}

};

} // namespace 

#endif // EXTENSION_DATA_HPP