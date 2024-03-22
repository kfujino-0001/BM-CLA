//SDRClassifierScalar.hpp

#ifndef SDR_CLASSIFIER_SCALAR_HPP
#define SDR_CLASSIFIER_SCALAR_HPP

/** @file
 * Definitions for SDRClassifierScalar class
 */

#include <vector>
#include <utility>
#include <algorithm>
#include <functional>

#include <cla/extension/types/SdrExtension.hpp>
#include <htm/encoders/ScalarEncoder.hpp>

namespace htm {

/**
 * ClassifierScalar implementation in C++.
 * 
 * @b Description
 * 
 */
class ClassifierScalar {

public:

	const ScalarEncoderParameters& parameters = args_;

	/**
	 * ClassifierScalar constructor
	 */
	ClassifierScalar() = default;

	/**
	 * ClassifierScalar constructor
	 * 
	 * @param params The parameters of ClassifierScalar
	 */
	ClassifierScalar(const ScalarEncoderParameters& params);

	/**
	 * ClassifierScalar destructor
	 */
	~ClassifierScalar() = default;

	/**
	 * Initialize ClassifierScalar
	 * 
	 * @param params The parameters of ClassifierScalar
	 */
	void initialize(const ScalarEncoderParameters& params);

	/**
	 * Infer the real value from SDR.
	 * 
	 * @param input The SDR.
	 * 
	 * @return The real value.
	 */
	Real64 infer(const SDR& input)const;
	
	/**
	 * Infer the real value from SDR with active cells.
	 * 
	 * @param input The SDR with active cells.
	 * 
	 * @return The real value
	 */
	template<typename ExData>
	Real64 infer(const SDRex<ExData>& input) const {
		NTA_CHECK(args_.size == input.size) << "input different size SDR.";
	
		return inferForInverse_(input.getExDataDense());
	}

private:

	ScalarEncoderParameters args_;

	template<typename Weight>
	Real64 inferForInverse_(const std::vector<Weight>& inputDense)const {
		NTA_CHECK(args_.activeBits != 0u) << "activeBits is zero.";

		const auto& chunk = args_.activeBits;

		std::vector<Weight> weightForChunk(inputDense.size() - (std::size_t)chunk + 1u, static_cast<Weight>(0));
		auto denseBegin = inputDense.begin();

		for (auto&& weight : weightForChunk) {
			std::for_each(denseBegin, denseBegin + chunk, [&](auto&& num) {weight += num; });
			denseBegin++;
		}

		const Weight& max = *std::max_element(weightForChunk.begin(), weightForChunk.end());
		std::vector<std::size_t> max_idx;

		for (
			auto&& itr = std::find(weightForChunk.begin(), weightForChunk.end(), max);
			itr != weightForChunk.end();
			itr = std::find(itr + 1, weightForChunk.end(), max)
		) {
			max_idx.push_back(std::distance(weightForChunk.begin(), itr));
		}

		const std::size_t& size = max_idx.size();
		//const std::size_t& median = (size %2 == 0)
		//	?(max_idx[(size - 1) / 2] + max_idx[size / 2]) / 2 : max_idx[size / 2];

		const std::size_t& median = max_idx[size / 2];

		Real64 predictiveValue =
			(args_.maximum - args_.minimum) * (Real64)median / (Real64)(args_.size - chunk) + args_.minimum;

		return predictiveValue;
	}

	template<typename Weight>
	std::pair<Real64, std::vector<Real64>> inferForFeedBack_(const std::vector<Weight>& inputDense)const {
		NTA_CHECK(args_.activeBits != 0u) << "activeBits is zero.";

		const auto& chunk = args_.activeBits;

		std::vector<Real64> weightForChunk(inputDense.size() - (std::size_t)chunk + 1u, static_cast<Weight>(0));
		auto denseBegin = inputDense.begin();

		for (auto&& weight : weightForChunk) {
			std::for_each(denseBegin, denseBegin + chunk, [&](auto&& num) {weight += static_cast<Real64>(num); });
			denseBegin++;
		}

		const Weight& max = *std::max_element(weightForChunk.begin(), weightForChunk.end());
		std::vector<std::size_t> max_idx;

		for (
			auto&& itr = std::find(weightForChunk.begin(), weightForChunk.end(), max);
			itr != weightForChunk.end();
			itr = std::find(itr + 1, weightForChunk.end(), max)
		) {
			max_idx.push_back(std::distance(weightForChunk.begin(), itr));
		}

		const std::size_t& size = max_idx.size();
		//const std::size_t& median = (size %2 == 0)
		//	?(max_idx[(size - 1) / 2] + max_idx[size / 2]) / 2 : max_idx[size / 2];

		const std::size_t& median = max_idx[size / 2];

		Real64 predictiveValue =
			(args_.maximum - args_.minimum) * (Real64)median / (Real64)(args_.size - chunk) + args_.minimum;

		return std::make_pair(predictiveValue, weightForChunk);
	}
};

}//namespace htm

#endif // !SDR_CLASSIFIER_SCALAR_HPP
