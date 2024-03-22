// SaveLayerActiveSegsCallback.cpp

/** 
 * @file
 * Implementation of SaveLayerActiveSegsCallback.cpp
 */

#include <algorithm> // count_if
#include <nlohmann/json.hpp>

#include "cla/utils/Checker.hpp"
#include "cla/model/core/CoreCLA.hpp"
#include "cla/model/module/callback/SaveLayerActiveSegsCallback.hpp"

namespace cla {

using json = nlohmann::json;


/************************************************
 * ActiveSegData public functions.
 ***********************************************/

ActiveSegDataHistory::ActiveSegData::ActiveSegData(
	const htm::Segment segment,
	const htm::CellIdx cell,
	const NumSyns numSynsFromInner,
	const NumSyns numSynsFromOuter
):
	segment(segment), 
	cell(cell), 
	numSynsFromInner(numSynsFromInner),
	numSynsFromOuter(numSynsFromOuter)
{}


/************************************************
 * ActiveSegData helper functions.
 ***********************************************/

void to_json(json& j, const ActiveSegDataHistory::ActiveSegData& d) {
	j["segment"] = d.segment;
	j["cell"] = d.cell;
	j["numSynsFromInner"] = d.numSynsFromInner;
	j["numSynsFromOuter"] = d.numSynsFromOuter;
}



/************************************************
 * ActiveSegDataHistory public functions.
 ***********************************************/

ActiveSegDataHistory::ActiveSegDataHistory(
	const Step step,
	const std::vector<ActiveSegData>& activeSegs
):
	step(step),
	activeSegs(activeSegs)
{}

ActiveSegDataHistory::ActiveSegDataHistory(
	const Step step,
	const std::vector<htm::Segment>& activeSegments,
	const htm::Connections& connections,
	const htm::UInt externalCellThreshold
):
	step(step)
{
	const auto& countInnerNumSyns
		= [&](const htm::CellIdx cell){return cell < externalCellThreshold;};
	
	const auto& relations = connections.getActiveRelationsForSegments();

	activeSegs.clear();
	activeSegs.reserve(activeSegments.size());

	for(const htm::Segment activeSegment : activeSegments) {
		if(relations.count(activeSegment)) {
			const auto& cells = relations.at(activeSegment);

			const htm::UInt numSynsFromInner
				= static_cast<htm::UInt>(
					std::count_if(cells.begin(), cells.end(), countInnerNumSyns)
				);

			const htm::UInt numSynsFromOuter
				= static_cast<htm::UInt>(cells.size()) - numSynsFromInner;
		
			activeSegs.emplace_back(ActiveSegData(
				activeSegment, connections.cellForSegment(activeSegment),
				numSynsFromInner, numSynsFromOuter
			));
		}
	}
}

ActiveSegDataHistory::ActiveSegDataHistory(
	const Step step,
	const std::vector<htm::Segment>& activeSegments,
	const htm::Connections& connections
):
	step(step)
{	
	const auto& relations = connections.getActiveRelationsForSegments();

	activeSegs.clear();
	activeSegs.reserve(activeSegments.size());

	for(const htm::Segment activeSegment : activeSegments) {
		if(relations.count(activeSegment)) {
			const auto& cells = relations.at(activeSegment);
		
			activeSegs.emplace_back(ActiveSegData(
				activeSegment, connections.cellForSegment(activeSegment),
				static_cast<htm::UInt>(cells.size()), 0u
			));
		}
	}
}


/************************************************
 * ActiveSegDataHistory helper functions.
 ***********************************************/

std::ostream& operator<<(std::ostream& os, const ActiveSegDataHistory& h) {
	json j;
	j["1_step"] = h.step;
	j["2_segments"] = h.activeSegs;
	
	os << j;
	return os;
}


/************************************************
 * SaveActiveSegmentsLog public functions.
 ***********************************************/

SaveActiveSegmentsLog::SaveActiveSegmentsLog(
	const std::string& path,
	const std::string& filename,
	const Step nbHoldingSteps
) {
	initialize(path, filename, nbHoldingSteps);
}

void SaveActiveSegmentsLog::initialize(
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

void SaveActiveSegmentsLog::reset() {
	firstInput_ = true;

	hists_.clear();
	hists_.reserve(nbHoldingSteps_);
}

void SaveActiveSegmentsLog::open() {
	createDir_(path_);

	std::ofstream ofs(path_ + filename_, std::ios::out);

	CLA_CHECK(
		!ofs.fail(),
		"Cannot open the sparse json file."
	);

	ofs << "{\"data\":[\n";
	ofs.close();
}

void SaveActiveSegmentsLog::add(
	const Step step,
	const PLayer& layer
) {
	const TME& tm = layer->getTME();
	const htm::UInt externalInput = tm.externalPredictiveInputs;

	if(externalInput > 0u) {
		hists_.emplace_back(
			ActiveSegDataHistory(
				step, tm.getActiveSegments(),
				tm.connections, externalInput
			)
		);
	} else {
		hists_.emplace_back(
			ActiveSegDataHistory(
				step, tm.getActiveSegments(),
				tm.connections
			)
		);
	}
}

void SaveActiveSegmentsLog::save() {
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

void SaveActiveSegmentsLog::close() {
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
 * SaveLayerActiveSegsCallback public functions.
 ***********************************************/

SaveLayerActiveSegsCallback::SaveLayerActiveSegsCallback(
	const std::string& path,
	const std::string& filename,
	const Step nbHoldingSteps
) {
	initialize(path, filename, nbHoldingSteps);
}

void SaveLayerActiveSegsCallback::initialize(
	const std::string& path,
	const std::string& filename,
	const Step nbHoldingSteps
) {
	SaveCallback::initialize(path, filename, nbHoldingSteps);
	reset();
}

void SaveLayerActiveSegsCallback::reset() {
	for(auto&& log : logs_) log.reset();
}

void SaveLayerActiveSegsCallback::open() {
	for(auto&& log : logs_) log.open();
}

void SaveLayerActiveSegsCallback::add(
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

void SaveLayerActiveSegsCallback::save() {
	for(auto&& log : logs_) log.save();
}

void SaveLayerActiveSegsCallback::close() {
	for(auto&& log : logs_) log.close();
}

void SaveLayerActiveSegsCallback::doStartProcessing(const CoreCLA* cla) {
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