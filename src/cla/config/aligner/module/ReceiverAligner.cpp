// ReceiverAligner.cpp

/** 
 * @file
 * Implementation of ReceiverAligner.cpp
 */

#include "cla/config/aligner/module/ReceiverAligner.hpp"
#include "cla/config/utils/ConfigHelpers.hpp"
#include "cla/config/utils/JsonParamDefinition.hpp"
#include "cla/utils/Checker.hpp"

namespace cla {

using RJLabel = ReceiverJsonLabels;


/************************************************
 * ReceiverJsonAligner private functions.
 ***********************************************/

void ReceiverJsonAligner::initializeByActiveCellReceiver_(
	const std::string& type,
	const json& config
) {
	type_ = type;
	config_ = config;

	isExternalPredictiveInputsUpdated_ = false;
}

void ReceiverJsonAligner::initializeByVolatileActiveCellReceiver_(
	const std::string& type,
	const json& config
) {
	type_ = type;
	config_ = config;

	isExternalPredictiveInputsUpdated_ = false;
}

void ReceiverJsonAligner::alignActiveCellReceiverConfig_() {}

void ReceiverJsonAligner::alignVolatileActiveCellReceiverConfig_() {
	if(isExternalPredictiveInputsUpdated_)
		ConfigHelper::assign(config_, RJLabel::PARAM_EXTERNAL_PREDICTIVE_INPUTS, externalPredictiveInputs_);
}


/************************************************
 * ReceiverJsonAligner public functions.
 ***********************************************/

ReceiverJsonAligner::ReceiverJsonAligner():
	isExternalPredictiveInputsUpdated_(false)
{

}

ReceiverJsonAligner::ReceiverJsonAligner(const json& config) {
	initialize(config);
}

ReceiverJsonAligner::ReceiverJsonAligner(
	const std::string& type,
	const json& config
) {
	initialize(type, config);
}

void ReceiverJsonAligner::initialize(const json& config) {
	if(KeyHelper::contain(config, RJLabel::ACTIVE_CELL_RECEIVER_LABEL)) {
		initializeByActiveCellReceiver_(
			RJLabel::ACTIVE_CELL_RECEIVER_LABEL,
			config.at(RJLabel::ACTIVE_CELL_RECEIVER_LABEL)
		);
		return;
	}

	if(KeyHelper::contain(config, RJLabel::VOLATILE_ACTIVE_CELL_RECEIVER_LABEL)) {
		initializeByVolatileActiveCellReceiver_(
			RJLabel::VOLATILE_ACTIVE_CELL_RECEIVER_LABEL,
			config.at(RJLabel::VOLATILE_ACTIVE_CELL_RECEIVER_LABEL)
		);
		return;
	}

	CLA_ALERT("Error: There is no model that can be aligned in ReceiverJsonAligner.");
}

void ReceiverJsonAligner::initialize(
	const std::string& type,
	const json& config
) {
	if(KeyHelper::contain(type, RJLabel::ACTIVE_CELL_RECEIVER_LABEL)) {
		initializeByActiveCellReceiver_(type, config);
		return;
	}

	if(KeyHelper::contain(type, RJLabel::VOLATILE_ACTIVE_CELL_RECEIVER_LABEL)) {
		initializeByVolatileActiveCellReceiver_(type, config);
		return;
	}

	CLA_ALERT("Error: There is no model that can be aligned in ReceiverJsonAligner.");
}

void ReceiverJsonAligner::align() {
	if(KeyHelper::contain(type_, RJLabel::ACTIVE_CELL_RECEIVER_LABEL)) {
		alignActiveCellReceiverConfig_();
		return;
	}

	if(KeyHelper::contain(type_, RJLabel::VOLATILE_ACTIVE_CELL_RECEIVER_LABEL)) {
		alignVolatileActiveCellReceiverConfig_();
		return;
	}

	CLA_ALERT("Error: There is no model that can be aligned in ReceiverJsonAligner.");
}

const std::string& ReceiverJsonAligner::getType() const {
	return type_;
}

const json& ReceiverJsonAligner::getConfig() const {
	return config_;
}


/************************************************
 * ReceiverJsonAligner public setters.
 ***********************************************/

void ReceiverJsonAligner::setExternalPredictiveInputs(const htm::UInt externalPredictiveInputs) {
	externalPredictiveInputs_ = externalPredictiveInputs;
	isExternalPredictiveInputsUpdated_ = true;
}

} // namespace cla