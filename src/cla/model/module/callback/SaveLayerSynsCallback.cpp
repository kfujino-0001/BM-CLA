// SaveLayerSynsCallback.cpp

/** 
 * @file
 * Implementation of SaveLayerSynsCallback.cpp
 */

#include <nlohmann/json.hpp>

#include "cla/utils/Checker.hpp"
#include "cla/model/core/CoreCLA.hpp"
#include "cla/model/module/callback/SaveLayerSynsCallback.hpp"

namespace cla {

using json = nlohmann::json;

/************************************************
 * SynapseHistory public functions.
 ***********************************************/

SynapseHistory::SynapseHistory(
	const Step step,
	const std::vector<SynapseConnection>& createdSynapses,
	const std::vector<htm::Synapse>& destroyedSynapses,
	const std::vector<SynapsePermanence>& updatedSynapses
):
	step(step),
	createdSynapses(createdSynapses),
	destroyedSynapses(destroyedSynapses),
	updatedSynapses(updatedSynapses)
{}


/************************************************
 * SynapseHistory helper functions.
 ***********************************************/

std::ostream& operator<<(std::ostream& os, const SynapseHistory& h) {
	json j;
	j["1_step"] = h.step;
	j["2_created"] = h.createdSynapses;
	j["3_destroyed"] = h.destroyedSynapses;
	j["4_update"] = h.updatedSynapses;

	os << j;

	return os;
}



/************************************************
 * SaveSynapseLog public functions.
 ***********************************************/

SaveSynapseLog::SaveSynapseLog(
	const std::string& path,
	const std::string& filename,
	const Step nbHoldingSteps
) {
	initialize(path, filename, nbHoldingSteps);
}

void SaveSynapseLog::initialize(
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

void SaveSynapseLog::reset() {
	firstInput_ = true;

	hists_.clear();
	hists_.reserve(nbHoldingSteps_);
}

void SaveSynapseLog::open() {
	createDir_(path_);

	std::ofstream ofs(path_ + filename_, std::ios::out);

	CLA_CHECK(
		!ofs.fail(),
		"Cannot open the sparse json file."
	);

	ofs << "{\"data\":[\n";
	ofs.close();
}

void SaveSynapseLog::add(
	const Step step,
	const PLayerProxy& layer
) {

	hists_.emplace_back(
		SynapseHistory(
			step, 
			layer->getTmCreatedSynapses(), 
			layer->getTmDestroyedSynapses(), 
			layer->getTmUpdatedSynapses()
		)
	);
}

void SaveSynapseLog::save() {
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

void SaveSynapseLog::close() {
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
 * SaveSynapseLogCallback public functions.
 ***********************************************/

SaveSynapseLogCallback::SaveSynapseLogCallback(
	const std::string& path,
	const std::string& filename,
	const Step nbHoldingSteps
) {
	initialize(path, filename, nbHoldingSteps);
}

void SaveSynapseLogCallback::initialize(
	const std::string& path,
	const std::string& filename,
	const Step nbHoldingSteps
) {
	SaveCallback::initialize(path, filename, nbHoldingSteps);
	reset();
}

void SaveSynapseLogCallback::reset() {
	for(auto&& log : logs_) log.reset();
}

void SaveSynapseLogCallback::open() {
	for(auto&& log : logs_) log.open();
}

void SaveSynapseLogCallback::add(
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

void SaveSynapseLogCallback::save() {
	for(auto&& log : logs_) log.save();
}

void SaveSynapseLogCallback::close() {
	for(auto&& log : logs_) log.close();
}

void SaveSynapseLogCallback::doStartProcessing(const CoreCLA* cla) {
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