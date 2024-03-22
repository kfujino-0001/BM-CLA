// ReceiverBuilder.cpp

/**
 * @file
 * Implementation of ReceiverBuilder.cpp
 */

#include "cla/model/generator/ReceiverGenerator.hpp"
#include "cla/config/builder/module/ReceiverBuilder.hpp"
#include "cla/config/utils/ConfigHelpers.hpp"
#include "cla/config/utils/JsonParamDefinition.hpp"
#include "cla/utils/Checker.hpp"

namespace cla {

using RJLabel = ReceiverJsonLabels;


/************************************************
 * ReceiverJsonBuilder private functions.
 ***********************************************/

PReceiver ReceiverJsonBuilder::buildActiveCellReceiver_(const json& config) {
	return ReceiverGenerator<ActiveCellReceiver>::generate();
}

PReceiver ReceiverJsonBuilder::buildVolatileActiveCellReceiver_(
	const json& config
) {
	htm::UInt externalPredictiveInputs;
	htm::Real volatileRate;
	htm::Real volatileThreshold;

	for(const auto& [key, value] : config.items()) {
		if(KeyHelper::contain(key, RJLabel::PARAM_EXTERNAL_PREDICTIVE_INPUTS)) {
			value.get_to(externalPredictiveInputs);
			continue;
		}

		if(KeyHelper::contain(key, RJLabel::PARAM_VOLATILE_RATE)) {
			value.get_to(volatileRate);
			continue;
		}

		if(KeyHelper::contain(key, RJLabel::PARAM_VOLATILE_THRESHOLD)) {
			value.get_to(volatileThreshold);
			continue;
		}

		CLA_ALERT("Error: There are parameters that are not assumed.");
	}

	return ReceiverGenerator<VolatileActiveCellReceiver>::generate(
		externalPredictiveInputs, volatileRate, volatileThreshold
	);
}


/************************************************
 * ReceiverJsonBuilder public functions.
 ***********************************************/

PReceiver ReceiverJsonBuilder::buildReceiver(const json& config) {
	if(KeyHelper::contain(config, RJLabel::ACTIVE_CELL_RECEIVER_LABEL)) {
		return buildActiveCellReceiver_(
			config.at(RJLabel::ACTIVE_CELL_RECEIVER_LABEL)
		);
	}

	if(KeyHelper::contain(config, RJLabel::VOLATILE_ACTIVE_CELL_RECEIVER_LABEL)) {
		return buildVolatileActiveCellReceiver_(
			config.at(RJLabel::VOLATILE_ACTIVE_CELL_RECEIVER_LABEL)
		);
	}

	CLA_ALERT("Error: There is no receiver that can be built.");
}

PReceiver ReceiverJsonBuilder::buildReceiver(
	const std::string& type,
	const json& config
) {
	if(KeyHelper::contain(type, RJLabel::ACTIVE_CELL_RECEIVER_LABEL)) {
		return buildActiveCellReceiver_(config);
	}

	if(KeyHelper::contain(type, RJLabel::VOLATILE_ACTIVE_CELL_RECEIVER_LABEL)) {
		return buildVolatileActiveCellReceiver_(config);
	}

	CLA_ALERT("Error: There is no receiver that can be built.");
}


} // namespace cla