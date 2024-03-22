// AdapterBuilder.cpp

/** 
 * @file
 * Implementation of AdapterBuilder.cpp
 */

#include "cla/model/generator/AdapterGenerator.hpp"
#include "cla/config/builder/module/AdapterBuilder.hpp"
#include "cla/config/utils/ConfigHelpers.hpp"
#include "cla/config/utils/JsonParamDefinition.hpp"
#include "cla/utils/Checker.hpp"

namespace cla {

using AJLabel = AdapterJsonLabels;


/************************************************
 * AdapterBuilder private functions.
 ***********************************************/

PAdapter AdapterJsonBuilder::buildDirectAdapter_(const json& config) {
	return AdapterGenerator<DirectAdapter>::generate();
}


/************************************************
 * AdapterBuilder public functions.
 ***********************************************/

PAdapter AdapterJsonBuilder::buildAdapter(const json& config) {
	if(KeyHelper::contain(config, AJLabel::DIRECT_ADAPTER_LABEL)) {
		return buildDirectAdapter_(config.at(AJLabel::DIRECT_ADAPTER_LABEL));
	}

	CLA_ALERT("Error: There is no adapter that can be built.");
}

PAdapter AdapterJsonBuilder::buildAdapter(
	const std::string& type,
	const json& config
) {
	if(KeyHelper::contain(type, AJLabel::DIRECT_ADAPTER_LABEL)) {
		return buildDirectAdapter_(config);
	}

	CLA_ALERT("Error: There is no adapter that can be built.");
}

} // namespace cla