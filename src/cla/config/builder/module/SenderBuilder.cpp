// SenderBuilder.cpp

/** 
 * @file
 * Implementation of SenderBuilder.cpp
 */

#include "cla/model/generator/SenderGenerator.hpp"
#include "cla/config/builder/module/SenderBuilder.hpp"
#include "cla/config/utils/ConfigHelpers.hpp"
#include "cla/config/utils/JsonParamDefinition.hpp"
#include "cla/utils/Checker.hpp"

namespace cla {

using SJLabel = SenderJsonLabels;


/************************************************
 * SenderJsonBuilder private functions.
 ***********************************************/

PSender SenderJsonBuilder:: buildActiveColumnSender_(const json& config) {
	return SenderGenerator<ActiveColumnSender>::generate();
}

PSender SenderJsonBuilder:: buildBurstColumnSender_(const json& config) {
	return SenderGenerator<BurstColumnSender>::generate();
}



/************************************************
 * SenderJsonBuilder public functions.
 ***********************************************/

PSender SenderJsonBuilder:: buildSender(const json& config) {
	if(KeyHelper::contain(config, SJLabel::ACTIVE_COLUMN_SENDER_LABEL)) {
		return buildActiveColumnSender_(config.at(SJLabel::ACTIVE_COLUMN_SENDER_LABEL));
	}

	if(KeyHelper::contain(config, SJLabel::BURST_COLUMN_SENDER_LABEL)) {
		return buildBurstColumnSender_(config.at(SJLabel::BURST_COLUMN_SENDER_LABEL));
	}

	CLA_ALERT("Error: There is no sender that can be built.");
}

PSender SenderJsonBuilder:: buildSender(
	const std::string& type, 
	const json& config
) {
	if(KeyHelper::contain(type, SJLabel::ACTIVE_COLUMN_SENDER_LABEL)) {
		return buildActiveColumnSender_(config);
	}

	if(KeyHelper::contain(type, SJLabel::BURST_COLUMN_SENDER_LABEL)) {
		return buildBurstColumnSender_(config);
	}

	CLA_ALERT("Error: There is no sender that can be built.");
}


} // namespace cla