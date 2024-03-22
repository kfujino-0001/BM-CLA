// ModelBuilder.cpp

/**
 * @file
 * Implementation of ModelBuilder.cpp
 */

#include "cla/model/generator/ClaGenerator.hpp"
#include "cla/config/builder/ModelBuilder.hpp"
#include "cla/config/builder/module/IoBuilder.hpp"
#include "cla/config/builder/module/LayerBuilder.hpp"
#include "cla/config/utils/ConfigHelpers.hpp"
#include "cla/config/utils/JsonParamDefinition.hpp"
#include "cla/utils/Checker.hpp"

namespace cla {

using MJLabel = ModelJsonLabels;


/************************************************
 * ModelJsonBuilder private functions.
 ***********************************************/

PCLA ModelJsonBuilder::buildMlclaModel_(const json& config) {

	std::vector<PLayer> layers;
	PIO io = nullptr;

	for(const auto& [key, value] : config.items()) {
		if(KeyHelper::contain(key, MJLabel::LAYER_KEYWORD)) {
			layers.emplace_back(std::move(LayerJsonBuilder::buildLayer(key, value)));
			continue;
		} 
		
		if(KeyHelper::contain(key, MJLabel::IO_KEYWORD)) {
			io = std::move(IoJsonBuilder::buildIO(key, value));
			continue;
		}
		
		CLA_ALERT("Error: There are parameters that are not assumed.");
	}

	CLA_CHECK(!layers.empty(), "Error: Not a single layer instance is generated.");
	for(const auto& layer : layers)
		CLA_CHECK(layer, "Error: A layer instance is not generated correctly.");

	CLA_CHECK(io, "Error: An io instance is not generated correctly.");

	return ClaGenerator<MultiLayerCLA>::generate(std::move(layers), std::move(io));
}


/************************************************
 * ModelJsonBuilder public functions.
 ***********************************************/

PCLA ModelJsonBuilder::buildModel(const json& config) {
	if(KeyHelper::contain(config, MJLabel::MLCLA_MODEL_LABEL)) {
		return buildMlclaModel_(config.at(MJLabel::MLCLA_MODEL_LABEL));
	}

	CLA_ALERT("Error: There is no model that can be built.");
}

PCLA ModelJsonBuilder::buildModel(
	const std::string& type, 
	const json& config
) {
	if(KeyHelper::contain(type, MJLabel::MLCLA_MODEL_LABEL)) {
		return buildMlclaModel_(config);
	}

	CLA_ALERT("Error: There is no model that can be built.");
}

} // namespace cla