// CompositeCallback.cpp

/** 
 * @file
 * Implementation of CompositeCallback.cpp
 */

#include "cla/model/module/callback/CompositeCallback.hpp"

namespace cla {

/************************************************
 * CompositeCallback public functions.
 ***********************************************/

CompositeCallback::CompositeCallback(const std::vector<PCallback>& callbacks) {
	initialize(callbacks);
}

void CompositeCallback::initialize(const std::vector<PCallback>& callbacks) {
	callbacks_ = callbacks;
}

void CompositeCallback::add(const PCallback& callback) {
	callbacks_.emplace_back(callback);
}

void CompositeCallback::doStartProcessing(const CoreCLA* cla) {
	for(PCallback& pcallback : callbacks_)
		pcallback->doStartProcessing(cla);
}

void CompositeCallback::doPreProcessing(
	const Step step,
	const Values& inputs,
	const CoreCLA* cla
) {
	for(PCallback& pcallback : callbacks_)
		pcallback->doPreProcessing(step, inputs, cla);
}

void CompositeCallback::doPostProcessing(
	const Step step,
	const Values& inputs,
	const Values& nexts,
	const Values& outputs,
	const CoreCLA* cla
) {
	for(PCallback& pcallback : callbacks_)
		pcallback->doPostProcessing(step, inputs, nexts, outputs, cla);
}

void CompositeCallback::doEndProcessing(const CoreCLA* cla) {
	for(PCallback& pcallback : callbacks_)
		pcallback->doEndProcessing(cla);
}

} // namespace cla