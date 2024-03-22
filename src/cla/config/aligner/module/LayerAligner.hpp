// LayerAligner.hpp

/** 
 * @file
 * Definitions for the LayerAligner class in C++
 */

#ifndef LAYER_ALIGNER_HPP
#define LAYER_ALIGNER_HPP

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

#include "cla/model/generator/LayerGenerator.hpp"
#include "cla/config/aligner/module/ReceiverAligner.hpp"
#include "cla/config/aligner/module/SpatialPoolerAligner.hpp"
#include "cla/config/aligner/module/TemporalMemoryAligner.hpp"

namespace cla {

using json = nlohmann::json;


/**
 * LayerJsonAligner implementation in C++.
 * 
 * @b Description
 * The LayerJsonAligner is for the htm layer parameters. This class aligns the
 * htm layer parameters of JSON based on the complemented modules.
 */
class LayerJsonAligner {

private:

	std::string type_;
	json config_;

	ReceiverJsonAligner receiver_;

	SpatialPoolerJsonAligner sp_;
	TemporalMemoryJsonAligner tm_;

	std::vector<htm::UInt> inputDimensions_;
	bool isInputDimensionsUpdated_;

private:

	/**
	 * Initialize the LayerJsonAligner by the HtmLayer.
	 * 
	 * @param type The name of the layer module type.
	 * @param config The config of the htm layer module on the JSON.
	 */
	void initializeByHtmLayer_(
		const std::string& type,
		const json& config
	);

	/**
	 * Align the config of the htm layer module.
	 */
	void alignHtmLayerConfig_();

public:

	/**
	 * LayerJsonAligner constructor.
	 */
	LayerJsonAligner() = default;

	/**
	 * LayerJsonAligner constructor with the config.
	 * 
	 * @param config The config of the layer module on the JSON.
	 */
	LayerJsonAligner(const json& config);

	/**
	 * LayerJsonAligner constructor with the config.
	 * 
	 * @param type The name of the layer module type.
	 * @param config The config of the layer module on the JSON.
	 */
	LayerJsonAligner(
		const std::string& type,
		const json& config
	);

	/**
	 * LayerJsonAligner destructor.
	 */
	~LayerJsonAligner() = default;

	/**
	 * Initialize the IoJsonAligner with the config.
	 * 
	 * @param config The config of the layer module on the JSON>
	 */
	void initialize(const json& config);

	/**
	 * Initialize the IoJsonAligner with the config.
	 * 
	 * @param type The name of the layer module type.
	 * @param config The config of the layer module on the JSON.
	 */
	void initialize(
		const std::string& type,
		const json& config
	);

	/**
	 * Align the config of the layer module.
	 */
	void align();

	/**
	 * Get the model type aligned in ModelJsonAligner.
	 */
	const std::string& getType() const;

	/**
	 * Get the config of the model.
	 * 
	 * @return const json& The aligned config of the model.
	 */
	const json& getConfig() const;

	/**
	 * Get the reference of the receiver.
	 * 
	 * @return ReceiverJsonAligner& The reference of the reciever. 
	 */
	ReceiverJsonAligner& getReceiver();

	/**
	 * Get the reference of the spatial pooler.
	 * 
	 * @return SpatialPoolerJsonAligner& The reference of the spatial pooler.
	 */
	SpatialPoolerJsonAligner& getSP();

	/**
	 * Get the reference of the temporal memory.
	 * 
	 * @return TemporalMemoryJsonAligner& The reference of the temporal memory.
	 */
	TemporalMemoryJsonAligner& getTM();

	/************************************************
	 * Setters.
	 ***********************************************/

	/**
	 * Set the seed value. Seed values for all modules on the layer are unified 
	 * to this value.
	 * 
	 * @param seed The seed value.
	 */
	void setSeed(const int seed);

	/**
	 * Set the input dimensions of the layer module. The input dimensions for all 
	 * modules on the layer are unified to this value.
	 * 
	 * @param dimensions The input dimensions.
	 */
	void setInputDimensions(const std::vector<htm::UInt>& dimensions);

	/**
	 * Set the external input dimensions of the layer module from the upper layer
	 * module. The external dimensions for the all modules on the layer are 
	 * unfired to this value.
	 * 
	 * @param dimensions The external input dimensions.
	 */
	void setExternalDimensions(const std::vector<htm::UInt>& dimensions);


	/************************************************
	 * Getters.
	 ***********************************************/

	/**
	 * Get the dimensions for the upper layer.
	 * 
	 * @return const std::vector<htm::UInt>& The dimensions.
	 */
	const std::vector<htm::UInt> getDimensionsForUpperLayer() const;

	/**
	 * Get the dimensions for the lower layer.
	 * 
	 * @return const std::vector<htm::UInt>& The dimensions.
	 */
	const std::vector<htm::UInt> getDimensionsForLowerLayer() const;

};

} // namespace cla

#endif // LAYER_ALIGNER_HPP