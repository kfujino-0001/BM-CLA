// SpatialPoolerAligner.cpp

/** 
 * @file
 * Implementation of SpatialPoolerAligner.cpp
 */

#include "cla/config/aligner/module/SpatialPoolerAligner.hpp"
#include "cla/config/utils/ConfigHelpers.hpp"
#include "cla/config/utils/JsonParamDefinition.hpp"
#include "cla/utils/Checker.hpp"

namespace cla {

using SPJLabel = SpatialPoolerJsonLabels;


/************************************************
 * SpatialPoolerJsonAligner private functions.
 ***********************************************/

void SpatialPoolerJsonAligner::initializeByHtmSpatialPooler_(
	const std::string& type,
	const json& config
) {
	type_ = type;
	config_ = config;

	isSeedUpdated_ = false;
	isInputDimensionsUpdated_ = false;
	isColumnDimensionsUpdated_ = false;
}

void SpatialPoolerJsonAligner::alignHtmSpatialPoolerConfig_() {
	if(isSeedUpdated_)
		ConfigHelper::assign(config_, SPJLabel::PARAM_SEED, seed_);

	if(isInputDimensionsUpdated_)
		ConfigHelper::assign(config_, SPJLabel::PARAM_INPUT_DIMENSIONS, inputDimensions_);

	if(isColumnDimensionsUpdated_)
		ConfigHelper::assign(config_, SPJLabel::PARAM_COLUMN_DIMENSIONS, columnDimensions_);

}


/************************************************
 * SpatialPoolerJsonAligner public functions.
 ***********************************************/

SpatialPoolerJsonAligner::SpatialPoolerJsonAligner():
	isSeedUpdated_(false),
	isInputDimensionsUpdated_(false),
	isColumnDimensionsUpdated_(false)
{

}

SpatialPoolerJsonAligner::SpatialPoolerJsonAligner(const json& config) {
	initialize(config);
}

SpatialPoolerJsonAligner::SpatialPoolerJsonAligner(
	const std::string& type,
	const json& config
) {
	initialize(type, config);
}

void SpatialPoolerJsonAligner::initialize(const json& config) {
	if(KeyHelper::contain(config, SPJLabel::HTM_SPATIAL_POOLER_LABEL)) {
		initializeByHtmSpatialPooler_(
			SPJLabel::HTM_SPATIAL_POOLER_LABEL, 
			config.at(SPJLabel::HTM_SPATIAL_POOLER_LABEL)
		);
		return;
	}

	CLA_ALERT("Error: There is no model that can be aligned in SpatialPoolerJsonAligner.");
}

void SpatialPoolerJsonAligner::initialize(
	const std::string& type,
	const json& config
) {
	if(KeyHelper::contain(type, SPJLabel::HTM_SPATIAL_POOLER_LABEL)) {
		initializeByHtmSpatialPooler_(type, config);
		return;
	}

	CLA_ALERT("Error: There is no model that can be aligned in SpatialPoolerJsonAligner.");
}

void SpatialPoolerJsonAligner::align() {
	if(KeyHelper::contain(type_, SPJLabel::HTM_SPATIAL_POOLER_LABEL)) {
		alignHtmSpatialPoolerConfig_();
		return;
	}

	CLA_ALERT("Error: There is no model that can be aligned in SpatialPoolerJsonAligner.");
}

const std::string& SpatialPoolerJsonAligner::getType() const {
	return type_;
}

const json& SpatialPoolerJsonAligner::getConfig() const {
	return config_;
}

/************************************************
 * SpatialPoolerJsonAligner public setters.
 ***********************************************/

void SpatialPoolerJsonAligner::setSeed(const int seed) {
	seed_ = seed;
	isSeedUpdated_ = true;
}

void SpatialPoolerJsonAligner::setInputDimensions(const std::vector<htm::UInt>& dimensions) {
	inputDimensions_ = dimensions;
	isInputDimensionsUpdated_ = true;
}

void SpatialPoolerJsonAligner::setColumnDimensions(const std::vector<htm::UInt>& dimensions) {
	columnDimensions_ = dimensions;
	isColumnDimensionsUpdated_ = true;
}


} // namespace cla