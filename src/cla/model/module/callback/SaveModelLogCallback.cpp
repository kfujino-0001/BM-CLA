// SaveModelLogCallback.cpp

/** 
 * @file
 * Implementation of SaveModelLogCallback.cpp
 */

#include "cla/utils/Checker.hpp"
#include "cla/model/module/callback/SaveModelLogCallback.hpp"

#include "cla/model/core/CoreCLA.hpp" // for cross-referencing

namespace cla {

/************************************************
 * ModelHistory helper functions.
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
 * ModelHistory public functions.
 ***********************************************/

ModelHistory::ModelHistory(
	const Step step,
	const Values& inputs,
	const Values& nexts,
	const Values& predictions
):
	step(step),
	inputs(inputs),
	nexts(nexts),
	predictions(predictions)
{}

Header ModelHistory::getHeader(const std::size_t nbValues) {
	Header header = {stepHeader};

	for(std::size_t i = 0u; i < nbValues; ++i) 
		header.emplace_back(inputHeader + std::to_string(i));

	for(std::size_t i = 0u; i < nbValues; ++i) 
		header.emplace_back(nextInputHeader + std::to_string(i));
	
	for(std::size_t i = 0u; i < nbValues; ++i) 
		header.emplace_back(predictionHeader + std::to_string(i));

	return header;
}


/************************************************
 * ModelHistory helper functions.
 ***********************************************/

std::ostream& operator<<(std::ostream& os, const ModelHistory& h) {
	os	<< h.step << ","
		<< h.inputs << ","
		<< h.nexts << ","
		<< h.predictions;

	return os;
}



/************************************************
 * SaveModelLogCallback public functions.
 ***********************************************/

SaveModelLogCallback::SaveModelLogCallback(
	const std::string& path,
	const std::string & filename,
	const Dim valueDimension,
	const Step nbHoldingSteps
) {
	initialize(path, filename, valueDimension, nbHoldingSteps);
}

void SaveModelLogCallback::initialize(
	const std::string& path,
	const std::string & filename,
	const Dim valueDimension,
	const Step nbHoldingSteps
) {
	CLA_ASSERT(valueDimension > 0u);

	SaveCallback::initialize(
		path, filename, nbHoldingSteps
	);
	
	valueDimension_ = valueDimension;

	reset();
}

void SaveModelLogCallback::reset() {
	hists_.clear();
	hists_.reserve(getNbHoldingSteps());
}

void SaveModelLogCallback::open() {
	createDir_(getPath());

	csv_.initialize(getPath() + getFilename(), true);
	csv_.write(std::ios::out, ModelHistory::getHeader(valueDimension_));
}

void SaveModelLogCallback::add(
	const Step step,
	const Values& inputs,
	const Values& nexts,
	const Values& outputs,
	const CoreCLA* cla
) {
	hists_.emplace_back(ModelHistory(step, inputs, nexts, outputs));
}

void SaveModelLogCallback::save() {
	csv_.write(std::ios::app, hists_);

	hists_.clear();
	hists_.reserve(getNbHoldingSteps());
}

} // namespace cla