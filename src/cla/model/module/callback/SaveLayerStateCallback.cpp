// SaveLayerStateCallback.cpp

/** 
 * @file
 * Implementation of SaveLayerStateCallback.cpp
 */

#include <fstream>
#include <nlohmann/json.hpp>

#include "cla/utils/Checker.hpp"
#include "cla/model/core/CoreCLA.hpp"
#include "cla/model/module/callback/SaveLayerStateCallback.hpp"

namespace cla {

using json = nlohmann::json;


/************************************************
 * SparseHistory public functions.
 ***********************************************/

SparseHistory::SparseHistory(
	const Step step,
	const htm::SDR_sparse_t& sparse
):
	step(step),
	sparse(sparse)
{}


/************************************************
 * SparseHistory helper functions.
 ***********************************************/

std::ostream& operator<<(std::ostream& os, const SparseHistory& h) {
	json j;
	j["1_step"] = h.step;
	j["2_sparse"] = h.sparse;

	os << j;

	return os;
}



/************************************************
 * SaveSparseLog public functions.
 ***********************************************/

SaveSparseLog::SaveSparseLog(
	const std::string& path,
	const std::string& filename,
	const Step nbHoldingSteps
) {
	initialize(path, filename, nbHoldingSteps);
}

void SaveSparseLog::initialize(
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

void SaveSparseLog::reset() {
	firstInput_ = true;

	hists_.clear();
	hists_.reserve(nbHoldingSteps_);
}

void SaveSparseLog::open() {
	createDir_(path_);

	std::ofstream ofs(path_ + filename_, std::ios::out);

	CLA_CHECK(
		!ofs.fail(),
		"Cannot open the sparse json file."
	);

	ofs << "{\"data\":[\n";
	ofs.close();
}

void SaveSparseLog::add(
	const Step step,
	const htm::SDR_sparse_t& sparse
) {
	hists_.emplace_back(SparseHistory(step, sparse));
}

void SaveSparseLog::save() {
	std::ofstream ofs(path_ + filename_, std::ios::app);

	CLA_CHECK(
		!ofs.fail(),
		"Cannot open the sparse json file."
	)

	for(const auto& hist : hists_){
		if(firstInput_){
			ofs << hist;
			firstInput_ = false;
			continue;
		}

		ofs << ",\n" << hist;
	}

	ofs.close();

	hists_.clear();
	hists_.reserve(nbHoldingSteps_);
}

void SaveSparseLog::close() {
	std::ofstream ofs(path_ + filename_, std::ios::app);
	
	CLA_CHECK(
		!ofs.fail(),
		"Cannot open the sparse json file."
	);

	ofs << "\n]}";
	ofs.close();

	firstInput_ = true;
}



/************************************************
 * SaveLayerStateCallback public functions.
 ***********************************************/

SaveLayerStateCallback::SaveLayerStateCallback(
	const std::string& path,
	const Step nbHoldingSteps
) {
	initialize(
		path, 
		{
			"inputBitsSparse.json",
			"activeColumnsSparse.json",
			"activeCellsSparse.json",
			"predictiveCellsSparse.json",
			"predictiveColumnsSparse.json"
		},
		nbHoldingSteps
	);
}

SaveLayerStateCallback::SaveLayerStateCallback(
	const std::string& path,
	const std::vector<std::string>& filenames,
	const Step nbHoldingSteps
) {
	initialize(path, filenames, nbHoldingSteps);
}

void SaveLayerStateCallback::initialize(
	const std::string& path,
	const std::vector<std::string>& filenames,
	const Step nbHoldingSteps
) {
	// 5 is the number of logs.
	CLA_ASSERT(filenames.size() == 5u);

	filenames_ = filenames;
	SaveCallback::initialize(path, "dummy", nbHoldingSteps);
	
	reset();
}

void SaveLayerStateCallback::reset() {
	for(auto&& log : inputBitLogs_) log.reset();
	for(auto&& log : activeColumnLogs_) log.reset();
	for(auto&& log : activeCellLogs_) log.reset();
	for(auto&& log : predictiveCellLogs_) log.reset();
	for(auto&& log : predictiveColumnLogs_) log.reset();
}

void SaveLayerStateCallback::open() {
	for(auto&& log : inputBitLogs_) log.open();
	for(auto&& log : activeColumnLogs_) log.open();
	for(auto&& log : activeCellLogs_) log.open();
	for(auto&& log : predictiveCellLogs_) log.open();
	for(auto&& log : predictiveColumnLogs_) log.open();
}

void SaveLayerStateCallback::add(
	const Step step,
	const Values& inputs,
	const Values& nexts,
	const Values& outputs,
	const CoreCLA* cla
) {
	const auto& layers = cla->getLayers();

	for(std::size_t i = 0u, size = layers.size(); i < size; ++i) {
		inputBitLogs_.at(i).add(step, layers.at(i)->getActiveBits().getSparse());
		activeColumnLogs_.at(i).add(step, layers.at(i)->getActiveColumns().getSparse());
		activeCellLogs_.at(i).add(step,layers.at(i)->getActiveCells().getSparse());
		predictiveCellLogs_.at(i).add(step, layers.at(i)->getPredictiveCells().getSparse());
		predictiveColumnLogs_.at(i).add(step, layers.at(i)->getPredictiveColumns().getSparse());
	}
}

void SaveLayerStateCallback::save() {
	for(auto&& log : inputBitLogs_) log.save();
	for(auto&& log : activeColumnLogs_) log.save();
	for(auto&& log : activeCellLogs_) log.save();
	for(auto&& log : predictiveCellLogs_) log.save();
	for(auto&& log : predictiveColumnLogs_) log.save();
}

void SaveLayerStateCallback::close() {
	for(auto&& log : inputBitLogs_) log.close();
	for(auto&& log : activeColumnLogs_) log.close();
	for(auto&& log : activeCellLogs_) log.close();
	for(auto&& log : predictiveCellLogs_) log.close();
	for(auto&& log : predictiveColumnLogs_) log.close();
}

void SaveLayerStateCallback::doStartProcessing(const CoreCLA* cla) {
	const auto& layers = cla->getLayers();
	const std::size_t layerSize = layers.size();

	inputBitLogs_.resize(layerSize);
	activeColumnLogs_.resize(layerSize);
	activeCellLogs_.resize(layerSize);
	predictiveCellLogs_.resize(layerSize);
	predictiveColumnLogs_.resize(layerSize);

	for(std::size_t i = 0u; i < layerSize; ++i) {
		inputBitLogs_.at(i).initialize(
			getPath() + "layer" + std::to_string(i) + "\\",
			filenames_.at(0), getNbHoldingSteps()
		);

		activeColumnLogs_.at(i).initialize(
			getPath() + "layer" + std::to_string(i) + "\\",
			filenames_.at(1), getNbHoldingSteps()
		);

		activeCellLogs_.at(i).initialize(
			getPath() + "layer" + std::to_string(i) + "\\",
			filenames_.at(2), getNbHoldingSteps()
		);

		predictiveCellLogs_.at(i).initialize(
			getPath() + "layer" + std::to_string(i) + "\\",
			filenames_.at(3), getNbHoldingSteps()
		);

		predictiveColumnLogs_.at(i).initialize(
			getPath() + "layer" + std::to_string(i) + "\\",
			filenames_.at(4), getNbHoldingSteps()
		);
	}

	SaveCallback::doStartProcessing(cla);
}



/************************************************
 * SaveLayerExStateCallback public functions.
 ***********************************************/

SaveLayerExStateCallback::SaveLayerExStateCallback(
	const std::string& path,
	const Step nbHoldingSteps
) {
	initialize(
		path, 
		{
			"externalActiveSparse.json",
			"externalWinnerSparse.json"
		},
		nbHoldingSteps
	);
}

SaveLayerExStateCallback::SaveLayerExStateCallback(
	const std::string& path,
	const std::vector<std::string>& filenames,
	const Step nbHoldingSteps
) {
	initialize(path, filenames, nbHoldingSteps);
}

void SaveLayerExStateCallback::initialize(
	const std::string& path,
	const std::vector<std::string>& filenames,
	const Step nbHoldingSteps
) {
	// 2 is the number of logs.
	CLA_ASSERT(filenames.size() == 2u);

	filenames_ = filenames;
	SaveCallback::initialize(path, "dummy", nbHoldingSteps);
	
	reset();
}

void SaveLayerExStateCallback::reset() {
	for(auto&& log : externalActiveSdrLogs_) log.reset();
	for(auto&& log : externalWinnerSdrLogs_) log.reset();
}

void SaveLayerExStateCallback::open() {
	for(auto&& log : externalActiveSdrLogs_) log.open();
	for(auto&& log : externalWinnerSdrLogs_) log.open();
}

void SaveLayerExStateCallback::add(
	const Step step,
	const Values& inputs,
	const Values& nexts,
	const Values& outputs,
	const CoreCLA* cla
) {
	const auto& layers = cla->getLayers();

	for(std::size_t i = 0u, size = layers.size(); i < size; ++i) {
		externalActiveSdrLogs_.at(i).add(step, layers.at(i)->getExternalActives().getSparse());
		externalWinnerSdrLogs_.at(i).add(step, layers.at(i)->getExternalWinners().getSparse());
	}
}

void SaveLayerExStateCallback::save() {
	for(auto&& log : externalActiveSdrLogs_) log.save();
	for(auto&& log : externalWinnerSdrLogs_) log.save();
}

void SaveLayerExStateCallback::close() {
	for(auto&& log : externalActiveSdrLogs_) log.close();
	for(auto&& log : externalWinnerSdrLogs_) log.close();
}

void SaveLayerExStateCallback::doStartProcessing(const CoreCLA* cla) {
	const auto& layers = cla->getLayers();
	const std::size_t layerSize = layers.size();

	externalActiveSdrLogs_.resize(layerSize);
	externalWinnerSdrLogs_.resize(layerSize);

	for(std::size_t i = 0u; i < layerSize; ++i) {
		externalActiveSdrLogs_.at(i).initialize(
			getPath() + "layer" + std::to_string(i) + "\\",
			filenames_.at(0), getNbHoldingSteps()
		);

		externalWinnerSdrLogs_.at(i).initialize(
			getPath() + "layer" + std::to_string(i) + "\\",
			filenames_.at(1), getNbHoldingSteps()
		);
	}

	SaveCallback::doStartProcessing(cla);
}

} // namespace cla