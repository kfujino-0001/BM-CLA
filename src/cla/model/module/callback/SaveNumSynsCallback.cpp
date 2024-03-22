// SaveNumSynsCallback.cpp

/** 
 * @file
 * Implementation of SaveNumSynsCallback.cpp
 */

#include "cla/utils/Checker.hpp"
#include "cla/model/core/CoreCLA.hpp"
#include "cla/model/module/callback/SaveNumSynsCallback.hpp"

namespace cla {


/************************************************
 * NumSynsData public functions.
 ***********************************************/

NumSynsData::NumSynsData(
	const NumSyn potentialSyns,
	const NumSyn connectedSyns,
	const NumSyn activeSyns,
	const NumSyn contributiveSyns
):
	potentialSyns(potentialSyns),
	connectedSyns(connectedSyns),
	activeSyns(activeSyns),
	contributiveSyns(contributiveSyns)
{}


/************************************************
 * NumSynsData helper functions.
 ***********************************************/

std::ostream& operator<<(std::ostream& os, const NumSynsData& nsd) {
	os	<< nsd.potentialSyns << ","
		<< nsd.connectedSyns << ","
		<< nsd.activeSyns << ","
		<< nsd.contributiveSyns;

	return os;
}



/************************************************
 * LayerLogArgs public functions.
 ***********************************************/

LayerLogArgs::LayerLogArgs(
	const PLayerProxy& selfLayer
) {
	isExistUpperLayer = false;

	selfLayerData.numRegions = static_cast<htm::UInt>(selfLayer->getSPEs().size());
	selfLayerData.numColumns = static_cast<htm::UInt>(selfLayer->getTME().numberOfColumns());
	selfLayerData.numCells = static_cast<htm::UInt>(selfLayer->getTME().numberOfCells());

	CLA_ASSERT(selfLayerData.numRegions > 0u);
	CLA_ASSERT(selfLayerData.numColumns > 0u);
	CLA_ASSERT(selfLayerData.numCells > 0u);
}

LayerLogArgs::LayerLogArgs(
	const PLayerProxy& selfLayer,
	const PLayerProxy& upperLayer
) {
	isExistUpperLayer = true;

	selfLayerData.numRegions = static_cast<htm::UInt>(selfLayer->getSPEs().size());
	selfLayerData.numColumns = static_cast<htm::UInt>(selfLayer->getTME().numberOfColumns());
	selfLayerData.numCells = static_cast<htm::UInt>(selfLayer->getTME().numberOfCells());

	CLA_ASSERT(selfLayerData.numRegions > 0u);
	CLA_ASSERT(selfLayerData.numColumns > 0u);
	CLA_ASSERT(selfLayerData.numCells > 0u);


	upperLayerData.numRegions = static_cast<htm::UInt>(upperLayer->getSPEs().size());
	upperLayerData.numColumns = static_cast<htm::UInt>(upperLayer->getTME().numberOfColumns());
	upperLayerData.numCells = static_cast<htm::UInt>(upperLayer->getTME().numberOfCells());

	CLA_ASSERT(upperLayerData.numRegions > 0u);
	CLA_ASSERT(upperLayerData.numColumns > 0u);
	CLA_ASSERT(upperLayerData.numCells > 0u);
}



/************************************************
 * TopNumSynsHistory public functions.
 ***********************************************/

TopNumSynsHistory::TopNumSynsHistory(
	const Step step,
	const RegionNumSynsData& nums
):
	step(step), nums(nums)
{}

Header TopNumSynsHistory::getHeader(
	const htm::UInt layerId,
	const htm::UInt numRegions
) {
	Header header = {stepHeader};

	// The headers for the self layer.
	for(std::size_t i = 0u; i < numRegions; ++i) {
		const std::string idStr
			= "L" + std::to_string(layerId) + "R" + std::to_string(i);
		
		header.emplace_back(idStr + potentialHeader);
		header.emplace_back(idStr + connectedHeader);
		header.emplace_back(idStr + activeHeader);
		header.emplace_back(idStr + contributiveHeader);
	}

	return header;
}


/************************************************
 * TopNumSynsHistory helper functions.
 ***********************************************/

std::ostream& operator<<(std::ostream& os, const TopNumSynsHistory& h) {
	os << h.step;
	for(auto&& line : h.nums) os << "," << line;
	
	return os;
}



/************************************************
 * MiddleNumSynsHistory public functions.
 ***********************************************/

MiddleNumSynsHistory::MiddleNumSynsHistory(
	const Step step,
	const RegionNumSynsData& selfNums,
	const RegionNumSynsData& upperNums
): 
	step(step), selfNums(selfNums), upperNums(upperNums) 
{}

Header MiddleNumSynsHistory::getHeader(
	const htm::UInt layerId,
	const htm::UInt numRegions
) {
	
	Header header = {stepHeader};

	// The headers for the upper layer.
	for(std::size_t i = 0u; i < numRegions; ++i) {
		const std::string idStr
			= "L" + std::to_string(layerId + 1u) + "R" + std::to_string(i);
		
		header.emplace_back(idStr + potentialHeader);
		header.emplace_back(idStr + connectedHeader);
		header.emplace_back(idStr + activeHeader);
		header.emplace_back(idStr + contributiveHeader);
	}

	// The headers for the self layer.
	for(std::size_t i = 0u; i < numRegions; ++i) {
		const std::string idStr
			= "L" + std::to_string(layerId) + "R" + std::to_string(i);
		
		header.emplace_back(idStr + potentialHeader);
		header.emplace_back(idStr + connectedHeader);
		header.emplace_back(idStr + activeHeader);
		header.emplace_back(idStr + contributiveHeader);
	}

	return header;
}


/************************************************
 * MiddleNumSynsHistory helper functions.
 ***********************************************/

std::ostream& operator<<(std::ostream& os, const MiddleNumSynsHistory& h) {
	os << h.step;

	for(auto&& line : h.upperNums) os << "," << line;
	for(auto&& line : h.selfNums) os << "," << line;
	
	return os;
}



/************************************************
 * AnalyzerFunc static functions.
 ***********************************************/

bool AnalyzerFunc::inUpperLayer(
	const htm::CellIdx cell,
	const LayerLogArgs& args
) {
	return cell >= args.selfLayerData.numCells;
}

const htm::UInt AnalyzerFunc::toRegion(
	const htm::CellIdx cell,
	const LayerData& layerData
) {
	return (layerData.numRegions * static_cast<htm::CellIdx>(cell)) / layerData.numCells;
}

const std::tuple<htm::UInt, htm::UInt, bool> AnalyzerFunc::toRelation(
	const htm::Synapse synapse,
	const htm::Connections& connections,
	const LayerLogArgs& args
) {
	const htm::SynapseData& synData = connections.dataForSynapse(synapse);
	const htm::CellIdx scell = synData.presynapticCell;
	const htm::CellIdx rcell = connections.cellForSegment(synData.segment);

	const bool isSenderCellInUpper = inUpperLayer(scell, args);

	htm::UInt sender;

	if(isSenderCellInUpper) {
		sender = toRegion(scell - args.selfLayerData.numCells, args.upperLayerData);
		CLA_ASSERT(sender < args.upperLayerData.numRegions);
	} else {
		sender = toRegion(scell, args.selfLayerData);
		CLA_ASSERT(sender < args.selfLayerData.numRegions);
	}

	htm::UInt receiver = toRegion(rcell, args.selfLayerData);
	CLA_ASSERT(receiver < args.selfLayerData.numRegions);

	return std::make_tuple(sender, receiver, isSenderCellInUpper);
}



/************************************************
 * TopLayerAnalyzer public functions.
 ***********************************************/

TopLayerAnalyzer::TopLayerAnalyzer(const LayerLogArgs& args) {
	initialize(args);
}

void TopLayerAnalyzer::initialize(
	const LayerLogArgs& args
) {
	args_ = args;

	const htm::UInt numRegions = args_.selfLayerData.numRegions;

	// initialize the database of num synapses.
	database_.clear();
	database_.resize(numRegions);

	for(htm::UInt i = 0u; i < numRegions; ++i){
		database_.at(i).resize(numRegions, {0u, 0u, 0u, 0u});
	}
}

void TopLayerAnalyzer::compile(const PLayerProxy& layer) {

	const auto& handler = layer->getTME().getSynapseHandler();
	const auto& connections = layer->getTM().connections;
	const htm::Permanence connectedThreshold = connections.getConnectedThreshold();
	const bool isInitConnected 
		= layer->getTME().getInitialPermanence() >= connectedThreshold;


	/************************************************
	 * 
	 ***********************************************/

	// This handles changes in the number of synapses due to synapse generation.
	for(const htm::Synapse createdSyn : handler.getCreatedSynapses()) {
		const auto& [sender, receiver, isUpper]
			= AnalyzerFunc::toRelation(createdSyn, connections, args_);

		CLA_ASSERT(!isUpper);
		
		auto&& data = database_.at(receiver).at(sender);

		data.potentialSyns++;
		if(isInitConnected) data.connectedSyns++;
	}

	// This handles changes in the number of synapses due to permanence update.
	for(const auto& updatedSynData : handler.getUpdatePermanences()) {
		const auto& [updatedSyn, perm] = updatedSynData;
		const auto& [sender, receiver, isUpper]
			= AnalyzerFunc::toRelation(updatedSyn, connections, args_);
		
		CLA_ASSERT(!isUpper);

		auto&& data = database_.at(receiver).at(sender);

		if(perm >= connectedThreshold) data.connectedSyns++;
		else data.connectedSyns--;
	}

	// This handles changes in the number of synapses due to delete of synapses.
	for(const htm::Synapse deletedSyn : handler.getDestroyedSynapses()) {
		const htm::Permanence perm = connections.dataForSynapse(deletedSyn).permanence;
		const auto& [sender, receiver, isUpper]
			= AnalyzerFunc::toRelation(deletedSyn, connections, args_);
	
		CLA_ASSERT(!isUpper);

		auto&& data = database_.at(receiver).at(sender);

		data.potentialSyns--;
		if(perm >= connectedThreshold) data.connectedSyns--;
	}


	/************************************************
	 * 
	 ***********************************************/

	const htm::UInt numRegions = args_.selfLayerData.numRegions;
	const auto& activeRelations = connections.getActiveRelationsForCells();
	const auto& psparse = layer->getPredictiveCells().getSparse();

	// initialize database.
	for(auto&& line : database_) {
		for(auto&& data : line) {
			data.activeSyns = 0u;
			data.contributiveSyns = 0u;
		}
	}

	for(const auto& relation : activeRelations) {
		const auto& [receivedCell, senderCells] = relation;
		const htm::UInt receiver 
			= AnalyzerFunc::toRegion(receivedCell, args_.selfLayerData);
		CLA_ASSERT(receiver < numRegions);

		const bool isPredictive 
			= std::binary_search(psparse.begin(), psparse.end(), receivedCell);

		for(const htm::CellIdx senderCell : senderCells) {
			const htm::UInt sender 
				= AnalyzerFunc::toRegion(senderCell, args_.selfLayerData);
			CLA_ASSERT(sender < numRegions);
		
			auto&& data = database_.at(receiver).at(sender);

			data.activeSyns++;
			if(isPredictive) data.contributiveSyns++;
		}
	}
}



/************************************************
 * MiddleLayerAnalyzer public functions.
 ***********************************************/

MiddleLayerAnalyzer::MiddleLayerAnalyzer(const LayerLogArgs& args) {
	initialize(args);
}

void MiddleLayerAnalyzer::initialize(
	const LayerLogArgs& args
) {
	CLA_ASSERT(args.isExistUpperLayer)

	args_ = args;

	// initialize the self layer database of num synapses.
	const htm::UInt selfNumRegions = args_.selfLayerData.numRegions;

	selfLayerDatabase_.clear();
	selfLayerDatabase_.resize(selfNumRegions);

	for(htm::UInt i = 0u; i < selfNumRegions; ++i) {
		selfLayerDatabase_.at(i).resize(selfNumRegions, {0u, 0u, 0u, 0u});
	}

	// initialize the upper layer database of num synapses.
	const htm::UInt upperNumRegions = args_.upperLayerData.numRegions;

	upperLayerDatabase_.clear();
	upperLayerDatabase_.resize(upperNumRegions);

	for(htm::UInt i = 0u; i < upperNumRegions; ++i) {
		upperLayerDatabase_.at(i).resize(upperNumRegions, {0u, 0u, 0u, 0u});
	}
}

void MiddleLayerAnalyzer::compile(const PLayerProxy& layer) {

	const auto& handler = layer->getTME().getSynapseHandler();
	const auto& connections = layer->getTME().connections;
	const htm::Permanence connectedThreshold = connections.getConnectedThreshold();
	const bool isInitConnected 
		= layer->getTME().getInitialPermanence() >= connectedThreshold;


	/************************************************
	 * 
	 ***********************************************/

	// This handles changes in the number of synapses due to synapse generation.
	for(const htm::Synapse createdSyn : handler.getCreatedSynapses()) {
		const auto& [sender, receiver, isUpper]
			= AnalyzerFunc::toRelation(createdSyn, connections, args_);
	
		auto&& data = (isUpper) ? 
						upperLayerDatabase_.at(receiver).at(sender):
						selfLayerDatabase_.at(receiver).at(sender);

		data.potentialSyns++;
		if(isInitConnected) data.connectedSyns++;
	}

	// This handles changes in the number of synapses due to permanence update.
	for(const auto& updatedSynData : handler.getUpdatePermanences()) {
		const auto& [updatedSyn, perm] = updatedSynData;
		const auto& [sender, receiver, isUpper]
			= AnalyzerFunc::toRelation(updatedSyn, connections, args_);

		auto&& data = (isUpper) ? 
						upperLayerDatabase_.at(receiver).at(sender):
						selfLayerDatabase_.at(receiver).at(sender);

		if(perm >= connectedThreshold) data.connectedSyns++;
		else data.connectedSyns--;
	}

	// This handles changes in the number of synapses due to delete of synapses.
	for(const htm::Synapse deletedSyn : handler.getDestroyedSynapses()) {
		const htm::Permanence perm = connections.dataForSynapse(deletedSyn).permanence;
		const auto& [sender, receiver, isUpper]
			= AnalyzerFunc::toRelation(deletedSyn, connections, args_);

		auto&& data = (isUpper) ? 
						upperLayerDatabase_.at(receiver).at(sender):
						selfLayerDatabase_.at(receiver).at(sender);

		data.potentialSyns--;
		if(perm >= connectedThreshold) data.connectedSyns--;
	}


	/************************************************
	 * 
	 ***********************************************/

	const htm::UInt selfNumRegions = args_.selfLayerData.numRegions;
	const htm::UInt upperNumRegions = args_.upperLayerData.numRegions;
	const auto& activeRelations = connections.getActiveRelationsForCells();
	const auto& psparse = layer->getPredictiveCells().getSparse();

	// initialize database.
	for(auto&& line : selfLayerDatabase_) {
		for(auto&& data : line) {
			data.activeSyns = 0u;
			data.contributiveSyns = 0u;
		}
	}

	for(auto&& line : upperLayerDatabase_) {
		for(auto&& data : line) {
			data.activeSyns = 0u;
			data.contributiveSyns = 0u;
		}
	}

	for(const auto& relation : activeRelations) {
		const auto& [receivedCell, senderCells] = relation;
		const htm::UInt receiver 
			= AnalyzerFunc::toRegion(receivedCell, args_.selfLayerData);
		CLA_ASSERT(receiver < selfNumRegions);

		const bool isPredictive 
			= std::binary_search(psparse.begin(), psparse.end(), receivedCell);
		
		for(const htm::CellIdx senderCell : senderCells) {
			const bool isUpper = AnalyzerFunc::inUpperLayer(senderCell, args_);
			htm::UInt sender;
			
			if(isUpper) {
				sender = AnalyzerFunc::toRegion(
							senderCell - args_.selfLayerData.numCells, 
							args_.upperLayerData
						);
				CLA_ASSERT(sender < upperNumRegions);
			} else {
				sender = AnalyzerFunc::toRegion(senderCell, args_.selfLayerData);
				CLA_ASSERT(sender < selfNumRegions);
			}

			auto&& data = (isUpper) ? 
							upperLayerDatabase_.at(receiver).at(sender):
							selfLayerDatabase_.at(receiver).at(sender);

			data.activeSyns++;
			if(isPredictive) data.contributiveSyns++;
		}
	}
}



/************************************************
 * SaveNumSynsCallback public functions.
 ***********************************************/

SaveNumSynsCallback::SaveNumSynsCallback(
	const std::string& path,
	const Step nbHoldingSteps
) {
	initialize(path, nbHoldingSteps);
}

void SaveNumSynsCallback::initialize(
	const std::string& path,
	const Step nbHoldingSteps
) {
	SaveCallback::initialize(path, "dummy", nbHoldingSteps);
	reset();
}

void SaveNumSynsCallback::reset() {
	topLayerProxyLog_.reset();
	for(auto&& log : middleLayerLogs_) log.reset();
}

void SaveNumSynsCallback::open() {
	topLayerProxyLog_.open();
	for(auto&& log : middleLayerLogs_) log.open();
}

void SaveNumSynsCallback::add(
	const Step step,
	const Values& inputs,
	const Values& nexts,
	const Values& outputs,
	const CoreCLA* cla
) {
	const auto& layers = cla->getLayers();
	CLA_ASSERT(layers.size() > 0u);
	CLA_ASSERT(middleLayerLogs_.size() == layers.size() - 1u);

	topLayerProxyLog_.add(step, layers.back());

	for(std::size_t i = 0u, size = layers.size(); i < size - 1u; i++) {
		middleLayerLogs_.at(i).add(step, layers.at(i));
	}
}

void SaveNumSynsCallback::save() {
	topLayerProxyLog_.save();
	for(auto&& log : middleLayerLogs_) log.save();
}

void SaveNumSynsCallback::close() {
	topLayerProxyLog_.close();
	for(auto&& log : middleLayerLogs_) log.close();
}

void SaveNumSynsCallback::doStartProcessing(const CoreCLA* cla) {
	const auto& layers = cla->getLayers();
	CLA_ASSERT(layers.size() > 0u);

	const htm::UInt numLayers = static_cast<htm::UInt>(layers.size());

	topLayerProxyLog_.initialize(
		getPath() + "layer" + std::to_string(numLayers - 1u) + "\\",
		getNbHoldingSteps(),
		numLayers - 1u, {layers.back()}
	);

	middleLayerLogs_.clear();
	middleLayerLogs_.resize(layers.size() - 1u);

	for(htm::UInt i = 0u; i < numLayers - 1u; ++i) {
		middleLayerLogs_.at(i).initialize(
			getPath() + "layer" + std::to_string(i) + "\\",
			getNbHoldingSteps(),
			i, {layers.at(i), layers.at(i + 1u)}
		);
	}

	SaveCallback::doStartProcessing(cla);
}


} // namespace cla