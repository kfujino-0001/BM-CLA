// TemporalMemoryAligner.cpp

/** 
 * @file
 * Implementation of TemporalMemoryAligner.cpp
 */

#include "cla/config/aligner/module/TemporalMemoryAligner.hpp"
#include "cla/config/utils/ConfigHelpers.hpp"
#include "cla/config/utils/JsonParamDefinition.hpp"
#include "cla/utils/Checker.hpp"

namespace cla {

using TMJLabel = TemporalMemoryJsonLabels;


/************************************************
 * TemporalMemoryJsonAligner private functions.
 ***********************************************/

void TemporalMemoryJsonAligner::initializeByHtmTemporalMemory_(
	const std::string& type,
	const json& config
) {
	type_ = type;
	config_ = config;

	isSeedUpdated_ = false;
	isNumRegionsUpdated_ = false;
	isColumnDimensionsUpdated_ = false;
	isCellsPerColumnUpdated_ = false;
	isExternalPredictiveInputsUpdated_ = false;
}

void TemporalMemoryJsonAligner::alignHtmTemporalMemoryConfig_() {
	if(isSeedUpdated_)
		ConfigHelper::assign(config_, TMJLabel::PARAM_SEED, seed_);

	if(isNumRegionsUpdated_)
		ConfigHelper::assign(config_, TMJLabel::PARAM_NUM_REGIONS, numRegions_);

	if(isColumnDimensionsUpdated_)
		ConfigHelper::assign(config_, TMJLabel::PARAM_COLUMN_DIMENSIONS, columnDimensions_);

	if(isCellsPerColumnUpdated_)
		ConfigHelper::assign(config_, TMJLabel::PARAM_CELLS_PER_COLUMN, cellsPerColumn_);

	if(isExternalPredictiveInputsUpdated_)
		ConfigHelper::assign(config_, TMJLabel::PARAM_EXTERNAL_PREDICTIVE_INPUTS, externalPredictiveInputs_);
}


/************************************************
 * TemporalMemoryJsonAligner public functions.
 ***********************************************/

TemporalMemoryJsonAligner::TemporalMemoryJsonAligner():
	isSeedUpdated_(false),
	isNumRegionsUpdated_(false),
	isColumnDimensionsUpdated_(false),
	isCellsPerColumnUpdated_(false),
	isExternalPredictiveInputsUpdated_(false)
{

}

TemporalMemoryJsonAligner::TemporalMemoryJsonAligner(const json& config) {
	initialize(config);
}

TemporalMemoryJsonAligner::TemporalMemoryJsonAligner(
	const std::string& type,
	const json& config
) {
	initialize(type, config);
}

void TemporalMemoryJsonAligner::initialize(const json& config) {
	if(KeyHelper::contain(config, TMJLabel::HTM_TEMPORAL_MEMORY_LABEL)) {
		initializeByHtmTemporalMemory_(
			TMJLabel::HTM_TEMPORAL_MEMORY_LABEL,
			config.at(TMJLabel::HTM_TEMPORAL_MEMORY_LABEL)
		);
		return;
	}

	CLA_ALERT("Error: There is no model that can be aligned in TemporalMemoryJsonAligner.");
}

void TemporalMemoryJsonAligner::initialize(
	const std::string& type,
	const json& config
) {
	if(KeyHelper::contain(type, TMJLabel::HTM_TEMPORAL_MEMORY_LABEL)) {
		initializeByHtmTemporalMemory_(type, config);
		return;
	}

	CLA_ALERT("Error: There is no model that can be aligned in TemporalMemoryJsonAligner.");
}

void TemporalMemoryJsonAligner::align() {
	if(KeyHelper::contain(type_, TMJLabel::HTM_TEMPORAL_MEMORY_LABEL)) {
		alignHtmTemporalMemoryConfig_();
		return;
	}

	CLA_ALERT("Error: There is no model that can be aligned in TemporalMemoryJsonAligner.");
}

const std::string& TemporalMemoryJsonAligner::getType() const {
	return type_;
}

const json& TemporalMemoryJsonAligner::getConfig() const {
	return config_;
}


/************************************************
 * TemporalMemoryJsonALigner public setters.
 ***********************************************/

void TemporalMemoryJsonAligner::setSeed(const int seed) {
	seed_ = seed;
	isSeedUpdated_ = true;
}

void TemporalMemoryJsonAligner::setNumRegions(const htm::UInt numRegions) {
	numRegions_ = numRegions;
	isNumRegionsUpdated_ = true;
}

void TemporalMemoryJsonAligner::setColumnDimensions(const std::vector<htm::UInt>& dimensions) {
	columnDimensions_ = dimensions;
	isColumnDimensionsUpdated_ = true;
}

void TemporalMemoryJsonAligner::setCellsPerColumn(const htm::UInt cellsPerColumn) {
	cellsPerColumn_ = cellsPerColumn;
	isCellsPerColumnUpdated_ = true;
}

void TemporalMemoryJsonAligner::setExternalPredictiveInputs(const htm::UInt externalPredictiveInputs) {
	externalPredictiveInputs_ = externalPredictiveInputs;
	isExternalPredictiveInputsUpdated_ = true;
}

} // namespace cla