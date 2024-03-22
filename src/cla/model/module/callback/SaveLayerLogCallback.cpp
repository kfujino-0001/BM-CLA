// SaveLayerLogCallback.cpp

/** 
 * @file
 * Implementation of SaveLayerLogCallback.cpp
 */

#include "cla/utils/Checker.hpp"
#include "cla/model/core/CoreCLA.hpp"
#include "cla/model/module/callback/SaveLayerLogCallback.hpp"

namespace cla {

/************************************************
 * LayerHistory helper functions.
 ***********************************************/

namespace {

// Urgent implementation to avoid conflicts // FIXME
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
	if(vec.empty()) return os;

	for(std::size_t i = 0u, size = vec.size() - 1u; i < size; ++i)
		os << vec.at(i) << ",";

	os << vec.back();
	return os;
}

} // namespace for inner linker.


/************************************************
 * LayerHistory public functions
 ***********************************************/

LayerHistory::LayerHistory(
	const Step step,
	const std::size_t predictiveCellsNum,
	const std::size_t predictiveColumnsNum,
	const double averageNumPredCellsPerColumn,
	const std::size_t burstColumnsNum,
	const double anomaly,
	const std::size_t numTmSynapses,
	const std::size_t numTmSegments,
	const Status status
):
	step(step),
	predictiveCellsNum(predictiveCellsNum),
	predictiveColumnsNum(predictiveColumnsNum),
	averageNumPredCellsPerColumn(averageNumPredCellsPerColumn),
	burstColumnsNum(burstColumnsNum),
	anomaly(anomaly),
	numTmSynapses(numTmSynapses),
	numTmSegments(numTmSegments),
	status(status)
{}


/************************************************
 * LayerHistory helper functions
 ***********************************************/

std::ostream& operator<<(std::ostream& os, const LayerHistory& h) {
	os	<< h.step << ","
		<< h.predictiveCellsNum << ","
		<< h.predictiveColumnsNum << ","
		<< h.averageNumPredCellsPerColumn << ","
		<< h.burstColumnsNum <<","
		<< h.anomaly << ","
		<< h.numTmSynapses << ","
		<< h.numTmSegments << ","
		<< StatusFuncs::toName(h.status);

	return os;
}



/************************************************
 * SaveLayerLog public functions.
 ***********************************************/

SaveLayerLog::SaveLayerLog(
	const std::string& path,
	const std::string& filename,
	const Step nbHoldingSteps
) {
	initialize(path, filename, nbHoldingSteps);
}

void SaveLayerLog::initialize(
	const std::string& path,
	const std::string& filename,
	const Step nbHoldingSteps
) {
	CLA_ASSERT(!path.empty());
	CLA_ASSERT(!filename.empty());
	CLA_ASSERT(nbHoldingSteps > 0u);

	path_ = path;
	filename_ = filename;
	nbHoldingSteps_ = nbHoldingSteps;

	reset();
}

void SaveLayerLog::reset() {
	hists_.clear();
	hists_.reserve(nbHoldingSteps_);
}

void SaveLayerLog::open() {
	createDir_(path_);

	csv_.initialize(path_ + filename_, true);
	csv_.write(std::ios::out, LayerHistory::header);
}

void SaveLayerLog::add(
	const Step step,
	const PLayerProxy& layer
) {
	const auto& pcells = layer->getPredictiveCells();
	const auto& pcolumns = layer->getPredictiveColumnsWithNbPCells();
	const auto& bcolumns = layer->getBurstColumns();

	hists_.emplace_back(LayerHistory(
		step, pcells.getSparse().size(),
		pcolumns.getSparse().size(),
		pcolumns.getExDataMean(),
		bcolumns.getSparse().size(),
		layer->getTmAnomaly(), 
		layer->getNbTmSynapses(),
		layer->getNbTmSegments(),
		layer->getStatus()
	));
}

void SaveLayerLog::save() {
	csv_.write(std::ios::app, hists_);

	hists_.clear();
	hists_.reserve(nbHoldingSteps_);
}



/************************************************
 * SaveLayerLogCallback public functions.
 ***********************************************/

SaveLayerLogCallback::SaveLayerLogCallback(
	const std::string& path,
	const std::string& filename,
	const Step nbHoldingSteps
) {
	initialize(path, filename, nbHoldingSteps);
}

void SaveLayerLogCallback::initialize(
	const std::string& path,
	const std::string& filename,
	const Step nbHoldingSteps
) {
	SaveCallback::initialize(path, filename, nbHoldingSteps);
	reset();
}

void SaveLayerLogCallback::reset() {
	for(auto&& log : logs_) log.reset();
}

void SaveLayerLogCallback::open() {
	for(auto&& log : logs_) log.open();
}

void SaveLayerLogCallback::add(
	const Step step,
	const Values& inputs,
	const Values& nexts,
	const Values& outputs,
	const CoreCLA* cla
) {
	const auto& layers = cla->getLayers();

	for(std::size_t i = 0u, size = layers.size(); i < size; ++i) {
		logs_.at(i).add(step, layers.at(i));
	}
}

void SaveLayerLogCallback::save() {
	for(auto&& log : logs_) log.save();
}

void SaveLayerLogCallback::close() {
	for(auto&& log : logs_) log.close();
}

void SaveLayerLogCallback::doStartProcessing(const CoreCLA* cla) {
	const auto& layers = cla->getLayers();
	logs_.resize(layers.size());

	for(std::size_t i = 0u, size = layers.size(); i < size; ++i) {
		logs_.at(i).initialize(
			getPath() + "layer" + std::to_string(i) + "\\",
			getFilename(), getNbHoldingSteps()
		);
	}

	SaveCallback::doStartProcessing(cla);
}


} // namespace cla