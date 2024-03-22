// ModelAligner.cpp

/** 
 * @file
 * Implementation of ModelAligner.cpp
 */

#include "cla/config/aligner/ModelAligner.hpp"
#include "cla/config/utils/ConfigHelpers.hpp"
#include "cla/config/utils/JsonParamDefinition.hpp"
#include "cla/utils/Checker.hpp"

namespace cla {

using MJLabel = ModelJsonLabels;


/************************************************
 * ModelJsonAligner private functions.
 ***********************************************/

void ModelJsonAligner::initializeByMLCLA_(
	const std::string& type,
	const json& config
) {
	type_ = type;
	config_ = config;
	
	for(const auto& [key, value] : config.items()) {
		if(KeyHelper::contain(key, MJLabel::LAYER_KEYWORD)) {
			layers_.emplace_back(key, value);
			continue;
		}

		if(KeyHelper::contain(key, MJLabel::IO_KEYWORD)) {
			io_.initialize(key, value);
			continue;
		}
	}
}

void ModelJsonAligner::alignMLCLAConfig_() {

	int size = static_cast<int>(layers_.size());
	int idx = size - 1;
	layers_.at(idx).setInputDimensions(io_.getDimensions());

	for(idx -= 1; idx >= 0; --idx) {
		layers_.at(idx).setInputDimensions(
			layers_.at(idx + 1).getDimensionsForUpperLayer()
		);
	}

	for(idx = 0; idx < size - 1; ++idx) {
		layers_.at(idx + 1).setExternalDimensions(
			layers_.at(idx).getDimensionsForLowerLayer()
		);
	}


	// register process.
	for(auto&& layer : layers_) {
		layer.align();
		config_.at(layer.getType()) = layer.getConfig();
	} 
		
	io_.align();
	config_.at(io_.getType()) = io_.getConfig();
}


/************************************************
 * ModelJsonAligner public functions.
 ***********************************************/

ModelJsonAligner::ModelJsonAligner(const json& config) {
	initialize(config);
}

ModelJsonAligner::ModelJsonAligner(
	const std::string& type,
	const json& config
) {
	initialize(type, config);
}

void ModelJsonAligner::initialize(const json& config) {
	if(KeyHelper::contain(config, MJLabel::MLCLA_MODEL_LABEL)) {
		initializeByMLCLA_(MJLabel::MLCLA_MODEL_LABEL, config.at(MJLabel::MLCLA_MODEL_LABEL));
		return;
	}

	CLA_ALERT("Error: There is no model that can be aligned in ModelJsonAligner.");
}

void ModelJsonAligner::initialize(
	const std::string& type,
	const json& config
) {
	if(KeyHelper::contain(type, MJLabel::MLCLA_MODEL_LABEL)) {
		initializeByMLCLA_(type, config);
		return;
	}

	CLA_ALERT("Error: There is no model that can be aligned in ModelJsonAligner.");
}

void ModelJsonAligner::align() {
	if(KeyHelper::contain(type_, MJLabel::MLCLA_MODEL_LABEL)) {
		alignMLCLAConfig_();
		return;
	}

	CLA_ALERT("Error: There is no model that can be aligned in ModelJsonAligner.");
}

const std::string& ModelJsonAligner::getType() const {
	return type_;
}

const json& ModelJsonAligner::getConfig() const {
	return config_;
}

std::vector<LayerJsonAligner>& ModelJsonAligner::getLayers() {
	return layers_;
}
	
IoJsonAligner& ModelJsonAligner::getIO() {
	return io_;
}


/************************************************
 * ModelJsonAligner public setters. 
 ***********************************************/

void ModelJsonAligner::setSeed(const int seed) {
	for(auto&& layer : layers_)
		layer.setSeed(seed);
		
	io_.setSeed(seed);
}


} // namespace cla