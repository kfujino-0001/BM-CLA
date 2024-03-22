// SpatialPoolerExtension.hpp

/**
 * @file
 * Definitions for the SpatialPoolerExtension in C++
 */

#ifndef SPATIAL_POOLER_EXTENSION_HPP
#define SPATIAL_POOLER_EXTENSION_HPP

#include <functional>
#include <nlohmann/json.hpp>
#include <string>

#include <cla/extension/types/SdrExtension.hpp>
#include <htm/algorithms/SpatialPooler.hpp>


namespace htm {

using json = nlohmann::json;



/**
 * SpatialPoolerExtensionParameters implementation in C++.
 *
 * @b Description
 * The SpatialPoolerExtensionsParameters contains the parameter of
 * SpatialPooler and SpatialPoolerExtension. This class supports
 * json files.
 *
 * @param synInitPermanence
 * The initial value of permanence when synapses are initialized by
 * const value.
 *
 * @param constSynInitPermanence
 * The boolean value whether synapses are initialized by const value or
 * random value.
 *
 * For any other params
 * See htm/algorithm/SpatialPooler.hpp
 */
struct SpatialPoolerExtensionParameters {
	std::vector<htm::UInt> inputDimensions;
	std::vector<htm::UInt> columnDimensions;
	htm::UInt potentialRadius = 16u;
	htm::Real potentialPct = 0.5f;
	bool globalInhibition = true;
	htm::Real localAreaDensity = 0.05f;
	htm::UInt stimulusThreshold = 0u;
	htm::Real synPermInactiveDec = 0.01f;
	htm::Real synPermActiveInc = 0.1f;
	htm::Real synPermConnected = 0.1f;
	htm::Real synInitPermanence = 0.5f;
	htm::Real minPctOverlapDutyCycles = 0.001f;
	htm::UInt dutyCyclePeriod = 1000u;
	htm::Real boostStrength = 0.0f;
	htm::Int seed = 1;
	htm::UInt spVerbosity = 0u;
	bool wrapAround = true;
	bool constSynInitPermanence = false;

	/**
	 * SpatialPoolerExtensionParameters constructor.
	 */
	SpatialPoolerExtensionParameters() = default;
};

std::ostream& operator<<(
	std::ostream& os,
	const SpatialPoolerExtensionParameters& params
);

void to_json(json& j, const SpatialPoolerExtensionParameters& p);
void from_json(const json& j, SpatialPoolerExtensionParameters& p);

using SPEParameters = SpatialPoolerExtensionParameters;



/**
 * SpatialPoolerExtension implementation in C++
 *
 * @b Description
 * The SpatialPoolerExtension is the extended class to compute inverse
 * spatial pooler.
 */
class SpatialPoolerExtension : public htm::SpatialPooler {

protected:

	bool constSynInitPermanence_ = false;
	htm::Real synInitPermanence_ = 0.5f;

public:

	/**
	 * SpatialPoolerExtension constructor.
	 */
	SpatialPoolerExtension() = default;

	/**
	 * SpatialPoolerExtension constructor.
	 *
	 * For the parameters.
	 * See the definition of SpatialPoolerExtensionParameters.
	 */
	SpatialPoolerExtension(
		const std::vector<htm::UInt>& inputDimensions,
		const std::vector<htm::UInt>& columnDimensions,
		const htm::UInt potentialRadius = 16u,
		const htm::Real potentialPct = 0.5f,
		const bool globalInhibition = true,
		const htm::Real localAreaDensity = 0.05f, // 5%
		const htm::UInt stimulusThreshold = 0u,
		const htm::Real synPermInactiveDec = 0.01f,
		const htm::Real synPermActiveInc = 0.05f,
		const htm::Real synPermConnected = 0.1f,
		const htm::Real synInitPermanence = 0.5f,
		const htm::Real minPctOverlapDutyCycles = 0.001f,
		const htm::UInt dutyCyclePeriod = 1000u,
		const htm::Real boostStrength = 0.0f,
		const htm::Int seed = 1,
		const htm::UInt spVerbosity = 0u,
		const bool wrapAround = true,
		const bool constSynInitPermanence = false
	);

	/**
	 * SpatialPoolerExtension constructor.
	 *
	 * @param params The parameters of SpatialPoolerExtensionParameters.
	 */
	SpatialPoolerExtension(const SpatialPoolerExtensionParameters& params);

	/**
	 * SpatialPoolerExtension destructor.
	 */
	~SpatialPoolerExtension() = default;

	/**
	 * Initialize SpatialPoolerExtension
	 *
	 * @param params The parameters of SpatialPoolerExtensionParameters.
	 */
	void initialize(const SpatialPoolerExtensionParameters& params);

	/**
	 * Initialize SpatialPoolerExtension.
	 *
	 * For the parameters.
	 * See the definition of SpatialPoolerExtensionParameters.
	 */
	void initialize(
		const std::vector<htm::UInt>& inputDimensions,
		const std::vector<htm::UInt>& columnDimensions,
		const htm::UInt potentialRadius = 16u,
		const htm::Real potentialPct = 0.5f,
		const bool globalInhibition = true,
		const htm::Real localAreaDensity = 0.05f, // 5%
		const htm::UInt stimulusThreshold = 0u,
		const htm::Real synPermInactiveDec = 0.01f,
		const htm::Real synPermActiveInc = 0.05f,
		const htm::Real synPermConnected = 0.1f,
		const htm::Real synInitPermanence = 0.5f,
		const htm::Real minPctOverlapDutyCycles = 0.001f,
		const htm::UInt dutyCyclePeriod = 1000u,
		const htm::Real boostStrength = 0.0f,
		const htm::Int seed = 1,
		const htm::UInt spVerbosity = 0u,
		const bool wrapAround = true,
		const bool constSynInitPermanence = false
	);

	/**
	 * Compute inverse spatial pooler. This functions requests type of args
	 * implements the ExtensionDataInterface.
	 *
	 * @param predictiveColumns The predictive columns.
	 * @param predictiveBits The predictive bits.
	 */
	template <
		typename SdrType,
		typename DataType = SdrType::ExtensionDataType>
	const std::vector<CellIdx> inverseCompute(
		const SdrType& predictiveColumns,
		SdrType& predictiveBits
	) const {
		const auto& callback
			= [](const auto v1, const auto v2) { return v1 + v2; };
		return inverseCompute(predictiveColumns, predictiveBits, callback);
	}

	/**
	 * Compute inverse spatial pooler. This functions requests type of args
	 * implements the ExtensionDataInterface.
	 *
	 * @param predictiveColumns The predictive columns.
	 * @param predictiveBits The predictive bits.
	 * @param callback The function to compile exdata from the predictive
	 * columns.
	 */
	template <
		typename SdrType,
		typename Func,
		typename DataType = SdrType::ExtensionDataType>
	const std::vector<CellIdx> inverseCompute(
		const SdrType& predictiveColumns,
		SdrType& predictiveBits,
		const Func& callback
	) const {

		predictiveColumns.reshape(columnDimensions_);
		predictiveBits.reshape(inputDimensions_);

		predictiveBits.zero();

		const SDR_sparse_t& predictiveColumnsSparse
			= predictiveColumns.getSparse();

		const auto& columnsDataDense = predictiveColumns.getExDataDense();

		std::vector<DataType> bitsDataDense(
			predictiveBits.size, static_cast<DataType>(0)
		);

		std::set<htm::CellIdx> uniqueBits;

		for(const ElemSparse column : predictiveColumnsSparse) {
			const auto& synapses = connections_.synapsesForSegment(column);
			const auto& exData = columnsDataDense.at(column);

			for(const Synapse synapse : synapses) {
				const auto& synData = connections_.dataForSynapse(synapse);

				if(synData.permanence > synPermConnected_) {
					const CellIdx bitIdx = synData.presynapticCell;
					uniqueBits.insert(bitIdx);
					bitsDataDense.at(bitIdx)
						= callback(bitsDataDense.at(bitIdx), exData);
					// bitsDataDense.at(bitIdx) += exData;
				}
			}
		}

		std::vector<htm::CellIdx> activeBits(
			uniqueBits.begin(), uniqueBits.end()
		);

		predictiveBits.setSparse(activeBits);
		predictiveBits.setExDataDense(bitsDataDense);

		return activeBits;
	}

	/**
	 * Returns the boolean value whether or not the init permanence is
	 * used.
	 *
	 * @return true: Using the init permanence.
	 * @return false Not using the init permanence.
	 */
	const bool getConstSynInitPermanence() const;

	/**
	 * Returns the amount of the initial permanence.
	 * 
	 * @return const htm::Real: The initial permanence.
	 */
	const htm::Real getSynInitPermanence() const;
};

} // namespace htm

#endif // SPATIAL_POOLER_EXTENSION_HPP