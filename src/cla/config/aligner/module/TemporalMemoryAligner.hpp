// TemporalMemoryAligner.hpp

/** 
 * @file
 * Definitions for the TemporalMemoryAligner class in C++
 */

#ifndef TEMPORAL_MEMORY_ALIGNER_HPP
#define TEMPORAL_MEMORY_ALIGNER_HPP

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

#include "cla/model/generator/TemporalMemoryGenerator.hpp"

namespace cla {

using json = nlohmann::json;


/**
 * TemporalMemoryJsonAligner implementation in C++.
 * 
 * @b Description
 * The TemporalMemoryJsonAligner is for the htm temporal memory parameters. This class aligns the
 * htm temporal memory parameters of JSON based on the complemented modules.
 */
class TemporalMemoryJsonAligner {

private:

	std::string type_;
	json config_;

	int seed_;
	htm::UInt numRegions_;
	std::vector<htm::UInt> columnDimensions_;
	htm::UInt cellsPerColumn_;
	htm::UInt externalPredictiveInputs_;

	bool isSeedUpdated_;
	bool isNumRegionsUpdated_;
	bool isColumnDimensionsUpdated_;
	bool isCellsPerColumnUpdated_;
	bool isExternalPredictiveInputsUpdated_;

private:

	/**
	 * Initialize the TemporalMemoryJsonAligner by the HtmTemporalMemory.
	 * 
	 * @param type The name of the htm temporal memory module type.
	 * @param config The config of the htm temporal memory module on the JSON.
	 */
	void initializeByHtmTemporalMemory_(
		const std::string& type,
		const json& config
	);

	/**
	 * Align the config of the htm temporal memory module.
	 */
	void alignHtmTemporalMemoryConfig_();

public:

	/**
	 * TemporalMemoryJsonAligner constructor.
	 */
	TemporalMemoryJsonAligner();

	/**
	 * TemporalMemoryJsonAligner constructor with the config.
	 * 
	 * @param config The config of the temporal memory module on the JSON.
	 */
	TemporalMemoryJsonAligner(const json& config);

	/**
	 * TemporalMemoryJsonAligner constructor with the config.
	 * 
	 * @param type The name of the temporal memory with the config.
	 * @param config The config of the temporal memory module on the JSON.
	 */
	TemporalMemoryJsonAligner(
		const std::string& type,
		const json& config
	);

	/**
	 * TemporalMemoryJsonAligner destructor.
	 */
	~TemporalMemoryJsonAligner() = default;

	/**
	 * Initialize the TemporalMemoryJsonAligner with the config.
	 * 
	 * @param config The config of the temporal memory module on the JSON.
	 */
	void initialize(const json& config);

	/**
	 * Initialize the TemporalMemoryJsonAligner with the config.
	 * 
	 * @param type The name of the temporal memory module type.
	 * @param config The config of the temporal memory module on the JSON.
	 */
	void initialize(
		const std::string& type,
		const json& config
	);

	/**
	 * Align the config of the temporal memory module.
	 */
	void align();

	/**
	 * Get the module type aligned in TemporalMemoryJsonAligner.
	 * 
	 * @return The module type.
	 */
	const std::string& getType() const;

	/**
	 * Get the config of the temporal memory module.
	 * 
	 * @return const json& The aligned config of the temporal memory module.
	 */
	const json& getConfig() const;


	/************************************************
	 * Setters.
	 ***********************************************/

	/**
	 * Set the seed value. 
	 * 
	 * @param seed The seed value.
	 */
	void setSeed(const int seed);

	/**
	 * Set the number of the regions on the temporal memory module.
	 * 
	 * @param numRegions The number of the region on the temporal memory module.
	 */
	void setNumRegions(const htm::UInt numRegions);

	/**
	 * Set the column dimensions of the temporal memory module.
	 * 
	 * @param dimensions The column dimensions of the temporal memory module.
	 */
	void setColumnDimensions(const std::vector<htm::UInt>& dimensions);

	/**
	 * Set tht number of the cells per a column.
	 * 
	 * @param cellsPerColumn The number of the cells per a column.
	 */
	void setCellsPerColumn(const htm::UInt cellsPerColumn);

	/**
	 * Set the external predictive inputs.
	 * 
	 * @param externalPredictiveInputs The external predictive inputs.
	 */
	void setExternalPredictiveInputs(const htm::UInt externalPredictiveInputs);

};

} // namespace cla

#endif // TEMPORAL_MEMORY_ALIGNER_HPP