// LayerBuilder.cpp

/**
 * @file
 * Implementation of LayerBuilder.cpp
 */

#include "cla/model/generator/LayerGenerator.hpp"
#include "cla/config/builder/module/LayerBuilder.hpp"
#include "cla/config/builder/module/AccepterBuilder.hpp"
#include "cla/config/builder/module/AdapterBuilder.hpp"
#include "cla/config/builder/module/SenderBuilder.hpp"
#include "cla/config/builder/module/ReceiverBuilder.hpp"
#include "cla/config/builder/module/SpatialPoolerBuilder.hpp"
#include "cla/config/builder/module/TemporalMemoryBuilder.hpp"
#include "cla/config/utils/ConfigHelpers.hpp"
#include "cla/config/utils/JsonParamDefinition.hpp"
#include "cla/utils/Checker.hpp"

namespace cla {

using LJLabel = LayerJsonLabels;


/************************************************
 * LayerJsonBuilder private functions.
 ***********************************************/

PLayer LayerJsonBuilder::buildHtmLayer_(const json& config) {
	std::vector<htm::UInt> inputDimensions = {};
	std::vector<htm::UInt> columnDimensions = {};
	htm::UInt nbCellsForColumn = 0u;
	htm::UInt nbRegions = 0u;

	// The values to initialize sps later.
	std::string tmpSPType;
	json tmpSPParams;

	PAccepter accepter = nullptr;
	PAdapter adapter = nullptr;
	PSender sender = nullptr;
	PReceiver receiver = nullptr;
	std::vector<PSpatialPooler> sps;
	PTemporalMemory tm = nullptr;

	for(const auto& [key, value] : config.items()) {
		if(KeyHelper::contain(key, LJLabel::PARAM_INPUT_DIMENSION_LABEL)) {
			value.get_to(inputDimensions);
			continue;
		} 
		
		if(KeyHelper::contain(key, LJLabel::PARAM_COLUMN_DIMENSION_LABEL)) {
			value.get_to(columnDimensions);
			continue;
		}
		
		if(KeyHelper::contain(key, LJLabel::PARAM_NB_CELLS_FOR_COLUMNS_LABEL)) {
			value.get_to(nbCellsForColumn);
			continue;
		}

		if(KeyHelper::contain(key, LJLabel::PARAM_NB_REGIONS_LABEL)) {
			value.get_to(nbRegions);
			continue;
		}

		if(KeyHelper::contain(key, LJLabel::ACCEPTER_KEYWORD)) {
			accepter = std::move(AccepterJsonBuilder::buildAccepter(key, value));
			continue;
		}

		if(KeyHelper::contain(key, LJLabel::ADAPTER_KEYWORD)) {
			adapter = std::move(AdapterJsonBuilder::buildAdapter(key, value));
			continue;;
		}

		if(KeyHelper::contain(key, LJLabel::SENDER_KEYWORD)) {
			sender = std::move(SenderJsonBuilder::buildSender(key, value));
			continue;
		}

		if(KeyHelper::contain(key, LJLabel::RECEIVER_KEYWORD)) {
			receiver = std::move(ReceiverJsonBuilder::buildReceiver(key, value));
			continue;
		}

		if(KeyHelper::contain(key, LJLabel::SPATIAL_POOLER_KEYWORD)) {
			tmpSPType = key;
			tmpSPParams = value;
			continue;
		}

		if(KeyHelper::contain(key, LJLabel::TEMPORAL_MEMORY_KEYWORD)) {
			tm = std::move(TemporalMemoryJsonBuilder::buildTemporalMemory(key, value));
			continue;
		}

		CLA_ALERT("Error: There are parameters that are not assumed.");
	}

	CLA_CHECK(!inputDimensions.empty(), "Error: An inputDimensions is not initialized.");
	for(const auto& dim : inputDimensions)
		CLA_CHECK(dim > 0u, "Error: An inputDimension includes the not allowed value.");

	CLA_CHECK(!columnDimensions.empty(), "Error A columnDimensions is not initialized.");
	for(const auto& dim : columnDimensions)
		CLA_CHECK(dim > 0u, "Error: A columnDimension includes the not allowed value.");

	CLA_CHECK(nbCellsForColumn > 0, "Error: A nbCellsForColumn value is not allowed.");
	CLA_CHECK(nbRegions, "Error: A nbRegions value is not allowed.");

	// Late initializing.
	for(htm::UInt i = 0u; i < nbRegions; ++i) {
		sps.emplace_back(std::move(SpatialPoolerJsonBuilder::buildSpatialPooler(tmpSPType, tmpSPParams)));
	}

	CLA_CHECK(accepter, "Error: An accepter instance is not generated correctly.");
	CLA_CHECK(adapter, "Error: An adapter instance is not generated correctly.");
	CLA_CHECK(sender, "Error: A sender instance is not generated correctly.");
	CLA_CHECK(receiver, "Error: A receiver instance is not generated correctly.");
	
	CLA_CHECK(!sps.empty(), "Error: Not a single spatial pooler instance is generated.");
	for(const auto& sp : sps)
		CLA_CHECK(sp, "Error: A spatial pooler instance is not generated correctly.");

	CLA_CHECK(tm, "Error: A temporal memory instance is not generated correctly.");


	return LayerGenerator<HtmLayer>::generate(
		inputDimensions, columnDimensions, 
		nbCellsForColumn, nbRegions,
		std::move(accepter), 
		std::move(adapter), 
		std::move(sender),
		std::move(receiver),
		std::move(sps), std::move(tm)
	);
};


/************************************************
 * LayerJsonBuilder public functions.
 ***********************************************/

PLayer LayerJsonBuilder::buildLayer(const json& config) {
	if(KeyHelper::contain(config, LJLabel::HTM_LAYER_LABEL)) {
		return buildHtmLayer_(config.at(LJLabel::HTM_LAYER_LABEL));
	}

	CLA_ALERT("Error: There is no layer that can be built.");
}


PLayer LayerJsonBuilder::buildLayer(
	const std::string& type,
	const json& config
) {
	if(KeyHelper::contain(type, LJLabel::HTM_LAYER_LABEL)) {
		return buildHtmLayer_(config);
	}

	CLA_ALERT("Error: There is no layer that can be built.");
}

} // namespace cla