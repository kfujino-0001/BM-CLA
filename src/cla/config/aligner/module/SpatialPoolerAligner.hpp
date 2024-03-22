// SpatialPoolerAligner.hpp

/** 
 * @file
 * Definitions for the SpatialPoolerAligner class in C++
 */

#ifndef SPATIAL_POOLER_ALIGNER_HPP
#define SPATIAL_POOLER_ALIGNER_HPP

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

#include "cla/model/generator/SpatialPoolerGenerator.hpp"

namespace cla {

using json = nlohmann::json;


/**
 * SpatialPoolerJsonAligner implementation in C++.
 * 
 * @b Description
 * The SpatialPoolerJsonAligner is for the htm spatial pooler parameters. This class aligns the
 * htm spatial pooler parameters of JSON based on the complemented modules.
 */
class SpatialPoolerJsonAligner {

private:

	std::string type_;
	json config_;

	std::vector<htm::UInt> inputDimensions_;
	std::vector<htm::UInt> columnDimensions_;
	int seed_;

	bool isSeedUpdated_;
	bool isInputDimensionsUpdated_;
	bool isColumnDimensionsUpdated_;

private:

	/**
	 * Initialize the SpatialPoolerJsonAligner by the HtmSpatialPooler.
	 * 
	 * @param type The name of the spatial pooler module type.
	 * @param config The config of the htm spatial pooler module on the JSON.
	 */
	void initializeByHtmSpatialPooler_(
		const std::string& type,
		const json& config
	);

	/**
	 * Align the config of the htm spatial pooler module.
	 */
	void alignHtmSpatialPoolerConfig_();

public:

	/**
	 * SpatialPoolerJsonAligner constructor.
	 */
	SpatialPoolerJsonAligner();

	/**
	 * SpatialPoolerJsonAligner constructor with the config.
	 * 
	 * @param config The config of the spatial pooler module on the JSON.
	 */
	SpatialPoolerJsonAligner(const json& config);

	/**
	 * SpatialPoolerJsonAligner constructor with the config.
	 * 
	 * @param type  The name of the spatial pooler module type.
	 * @param config The config of the spatial pooler module on the JSON.
	 */
	SpatialPoolerJsonAligner(
		const std::string& type,
		const json& config
	);

	/**
	 * SpatialPoolerJsonAligner destructor.
	 */
	~SpatialPoolerJsonAligner() = default;

	/**
	 * Initialize the SpatialPoolerJsonAligner with the config.
	 * 
	 * @param config The config of the spatial pooler module on the JSON.
	 */
	void initialize(const json& config);

	/**
	 * Initialize the SpatialPoolerJsonAligner with the config.
	 * 
	 * @param type The name of the spatial pooler module type.
	 * @param config The config of the spatial pooler module on the JSON.
	 */
	void initialize(
		const std::string& type,
		const json& config
	);

	/**
	 * Align the config of the spatial pooler module.
	 */
	void align();

	/**
	 * Get the module type aligned in SpatialPoolerJsonAligner.
	 * 
	 * @return The module type.
	 */
	const std::string& getType() const;

	/**
	 * Get the config of the spatial pooler module.
	 * 
	 * @return const json& The aligned config of the spatial pooler module.
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
	 * Set the input dimensions of the spatial pooler modules.
	 * 
	 * @param dimensions The input dimension of the spatial pooler modules.
	 */
	void setInputDimensions(const std::vector<htm::UInt>& dimensions);

	/**
	 * Set the column dimensions of the spatial pooler modules.
	 * 
	 * @param dimensions The column dimensions of the spatial pooler modules.
	 */
	void setColumnDimensions(const std::vector<htm::UInt>& dimensions);

};

} // namespace cla

#endif // SPATIAL_POOLER_ALIGNER_HPP