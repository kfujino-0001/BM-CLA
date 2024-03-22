// SaveCallback.cpp

/** 
 * @file
 * Implementation of SaveCallback.cpp
 */

#include <filesystem> // for create_directories

#include "cla/model/module/callback/SaveCallback.hpp"
#include "cla/utils/Checker.hpp"

namespace cla {

namespace fs = std::filesystem;


/************************************************
 * CoreSaver public functions.
 ***********************************************/

void CoreSaver::createDir_(const std::string& path) const {
	if(!fs::exists(path)) 
		fs::create_directories(path);
}



/************************************************
 * SaveCallback public functions.
 ***********************************************/

SaveCallback::SaveCallback(
	const std::string& path,
	const std::string & filename,
	const Step nbHoldingSteps
) {
	initialize(path, filename, nbHoldingSteps);
}

void SaveCallback::initialize(
	const std::string& path,
	const std::string & filename,
	const Step nbHoldingSteps
) {
	CLA_ASSERT(!path.empty());
	CLA_ASSERT(!filename.empty());
	CLA_ASSERT(nbHoldingSteps > 0u);

	path_ = path;
	filename_ = filename;
	nbHoldingSteps_ = nbHoldingSteps;
}

void SaveCallback::doStartProcessing(const CoreCLA* cla) {
	reset();
	open();
}

void SaveCallback::doPostProcessing(
	const Step step,
	const Values& inputs,
	const Values& nexts,
	const Values& outputs,
	const CoreCLA* cla
) {
	add(step, inputs, nexts, outputs, cla);

	if((step + 1u) % nbHoldingSteps_ == 0u)
		save();
}

void SaveCallback::doEndProcessing(const CoreCLA* cla) {
	save(); // Save the remaining data.
	close();
}


} // namespace cla