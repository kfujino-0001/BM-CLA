// RealDataEnv.hpp

/** 
 * @file
 * Definitions for the RealDataEnv class in C++
 */

#ifndef REAL_DATA_ENV_HPP
#define REAL_DATA_ENV_HPP

#include <iostream>
#include <istream>
#include <string>
#include <vector>

#include "cla/utils/Csv.hpp"
#include "cla/utils/Checker.hpp"
#include "cla/environment/core/CoreEnv.hpp"

namespace cla {

/**
 * CoreRealData implementation in C++.
 * 
 * @b Description
 * The CoreData is the data structure for the RealDataEnv. An extension
 * class of this class that defines specific types of data.
 */
struct CoreData {

public:

	/**
	 * CoreRealData constructor.
	 */
	CoreData() = default;

	/**
	 * CoreRealData destructor.
	 */
	virtual ~CoreData() = default;

	virtual Values toValues() const = 0;
	virtual void fromIS(std::istream& is) = 0; 

public:

	explicit operator Values() const noexcept { return toValues(); }
	friend std::istream& operator>>(std::istream& is, CoreData& data);

};

inline std::istream& operator>>(std::istream& is, CoreData& data) {
	data.fromIS(is);
	return is;
}



/**
 * RealDataEnv implementation in C++
 * 
 * @b Description
 * The RealDataEnv is the environment for the real data.
 */
template<class DataType>
class RealDataEnv : public CoreEnv {

private:

	std::string filename_;
	std::vector<DataType> data_;

public:

	inline static const std::string name = "RealDataEnv";

private:

	template<typename Cmp>
	Values selectElements_(
		const Values& val1, 
		const Values& val2, 
		const Cmp& cmp
	) {
		CLA_CHECK(
			val1.size() == val2.size(),
			"The different size values are compared."
		)

		const auto selector = [&](const auto& a, const auto& b){return cmp(a, b) ? a : b;};
	
		Values val;
		val.reserve(val1.size());

		for(std::size_t i = 0u, size = val1.size(); i < size; ++i) 
			val.emplace_back(selector(val1.at(i), val2.at(i)));

		return val;
	}

	void initializeDataInfo_(
		Values& mins,
		Values& maxs
	) {

		mins.clear();
		maxs.clear();

		mins = maxs = static_cast<Values>(data_.front());
	
		for(const auto& line : data_) {
			mins = selectElements_(mins, static_cast<Values>(line), [](const auto& a, const auto& b){return a < b;});
			maxs = selectElements_(maxs, static_cast<Values>(line), [](const auto& a, const auto& b){return a > b;});
		}
	}

public:

	/**
	 * RealDataEnv constructor
	 * 
	 * @param filename The file name of the data.
	 * @param dimension The dimension of this environment.
	 * @param lim The limit of reading lines. (lim = 0 means unlimited)
	 */
	RealDataEnv(
		const std::string& filename,
		const std::size_t dimension,
		const std::size_t lim = 0u
	):
		CoreEnv()
	{
		filename_ = filename;
		_setDimension(dimension);

		csv::Csv csv(filename_);
		csv.read(data_, false, lim);

		CLA_CHECK(
			!data_.empty(),
			"Data is empty."
		)

		CLA_CHECK(
			static_cast<Values>(data_.front()).size() == getDimension(),
			"The dimensions set does not match the dimensions fo the actual data."
		)

		Values mins, maxs;
		initializeDataInfo_(mins, maxs);

		_setMins(mins);
		_setMaxs(maxs);
	}

	/**
	 * RealDataEnv destructor
	 */
	~RealDataEnv() = default;

	/**
	 * Get the value of real data env.
	 * 
	 * @return The value of the real data env.
	 */
	const Values getValues() const override {
		if(getStep() == data_.size())
			return getMins();

		return static_cast<Values>(data_.at(getStep()));
	}

	/**
	 * Get the data size.
	 * 
	 * @return The data size.
	 */
	inline const std::size_t getDataSize() const {
		return data_.size();
	}

	/**
	 * Get json config of the environment.
	 * 
	 * @return The json instance.
	 */
	const json getJsonConfig() const override {
		json j;

		j[name]["data"] = DataType::name;
		j[name]["filename"] = filename_;
		j[name]["dimension"] = getDimension();

		return j;
	}
	

};


} // namespace cla

#endif // REAL_DATA_ENV_HPP