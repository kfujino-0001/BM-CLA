// LayerAligner.cpp

/** 
 * @file
 * Implementation of LayerAligner.cpp
 */

#include "cla/config/aligner/module/LayerAligner.hpp"
#include "cla/config/utils/ConfigHelpers.hpp"
#include "cla/config/utils/JsonParamDefinition.hpp"
#include "cla/utils/Checker.hpp"

namespace cla {

using LJLabel = LayerJsonLabels;


/************************************************
 * LayerJsonAligner private functions.
 ***********************************************/

void LayerJsonAligner::initializeByHtmLayer_(
	const std::string& type,
	const json& config
) {
	isInputDimensionsUpdated_ = false;

	type_ = type;
	config_ = config;

	for(const auto& [key, value] : config.items()) {
		if(KeyHelper::contain(key, LJLabel::RECEIVER_KEYWORD)) {
			receiver_.initialize(key, value);
			continue;
		}

		if(KeyHelper::contain(key, LJLabel::SPATIAL_POOLER_KEYWORD)) {
			sp_.initialize(key, value);
			continue;
		}

		if(KeyHelper::contain(key, LJLabel::TEMPORAL_MEMORY_KEYWORD)) {
			tm_.initialize(key, value);
			continue;
		}
	}
}

void LayerJsonAligner::alignHtmLayerConfig_() {

	if(isInputDimensionsUpdated_)
		ConfigHelper::assign(config_, LJLabel::PARAM_INPUT_DIMENSION_LABEL, inputDimensions_);

	sp_.setColumnDimensions(config_.at(LJLabel::PARAM_COLUMN_DIMENSION_LABEL));
	tm_.setColumnDimensions(config_.at(LJLabel::PARAM_COLUMN_DIMENSION_LABEL));
	tm_.setNumRegions(config_.at(LJLabel::PARAM_NB_REGIONS_LABEL));
	tm_.setCellsPerColumn(config_.at(LJLabel::PARAM_NB_CELLS_FOR_COLUMNS_LABEL));

	receiver_.align();
	config_.at(receiver_.getType()) = receiver_.getConfig();

	sp_.align();
	config_.at(sp_.getType()) = sp_.getConfig();

	tm_.align();
	config_.at(tm_.getType()) = tm_.getConfig();
}


/************************************************
 * LayerJsonAligner public functions.
 ***********************************************/

LayerJsonAligner::LayerJsonAligner(const json& config) {
	initialize(config);
}

LayerJsonAligner::LayerJsonAligner(
	const std::string& type,
	const json& config
) {
	initialize(type, config);
}

void LayerJsonAligner::initialize(const json& config) {
	if(KeyHelper::contain(config, LJLabel::HTM_LAYER_LABEL)) {
		initializeByHtmLayer_(LJLabel::HTM_LAYER_LABEL, config.at(LJLabel::HTM_LAYER_LABEL));
		return;
	}

	CLA_ALERT("Error: There is no model that can be aligned in LayerJsonAligner.");
}

void LayerJsonAligner::initialize(
	const std::string& type,
	const json& config
) {
	if(KeyHelper::contain(type, LJLabel::HTM_LAYER_LABEL)) {
		initializeByHtmLayer_(type, config);
		return;
	}

	CLA_ALERT("Error: There is no model that can be aligned in LayerJsonAligner.");
}

void LayerJsonAligner::align() {
	if(KeyHelper::contain(type_, LJLabel::HTM_LAYER_LABEL)) {
		alignHtmLayerConfig_();
		return;
	}

	CLA_ALERT("Error: There is no model that can be aligned in LayerJsonAligner.");
}

const std::string& LayerJsonAligner::getType() const {
	return type_;
}

const json& LayerJsonAligner::getConfig() const {
	return config_;
}

ReceiverJsonAligner& LayerJsonAligner::getReceiver() {
	return receiver_;
}

SpatialPoolerJsonAligner& LayerJsonAligner::getSP() {
	return sp_;
}

TemporalMemoryJsonAligner& LayerJsonAligner::getTM() {
	return tm_;
}


/************************************************
 * LayerJsonAligner public setters.
 ***********************************************/

void LayerJsonAligner::setSeed(const int seed) {
	sp_.setSeed(seed);
	tm_.setSeed(seed);
}

void LayerJsonAligner::setInputDimensions(const std::vector<htm::UInt>& dimensions) {
	sp_.setInputDimensions(dimensions);

	inputDimensions_ = dimensions;
	isInputDimensionsUpdated_ = true;
}

void LayerJsonAligner::setExternalDimensions(const std::vector<htm::UInt>& dimensions) {
	const htm::UInt externalInputs = htm::SDR(dimensions).size;
	
	tm_.setExternalPredictiveInputs(externalInputs);
	receiver_.setExternalPredictiveInputs(externalInputs);
}


/************************************************
 * LayerJsonAligner public getters.
 ***********************************************/

const std::vector<htm::UInt> LayerJsonAligner::getDimensionsForUpperLayer() const {
	
	// TODO temporal implementation for the columns sender.
	// If the cell senders are implemented, change the this implementation.
	std::vector<htm::UInt> dimension;

	config_.at(LJLabel::PARAM_COLUMN_DIMENSION_LABEL).get_to(dimension);
	return dimension;
}

const std::vector<htm::UInt> LayerJsonAligner::getDimensionsForLowerLayer() const {
	// TODO temporal implementation for the columns sender.
	// If the cell senders are implemented, change the this implementation.
	
	std::vector<htm::UInt> dimension;

	config_.at(LJLabel::PARAM_COLUMN_DIMENSION_LABEL).get_to(dimension);
	dimension.emplace_back(config_.at(LJLabel::PARAM_NB_CELLS_FOR_COLUMNS_LABEL));

	return dimension;
}

} // namespace cla