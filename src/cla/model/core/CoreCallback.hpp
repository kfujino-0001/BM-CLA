// CoreCallback.hpp

/**
 * @file
 * Definitions for the CoreCallback class in C++
 */

#ifndef CORE_CALLBACK_HPP
#define CORE_CALLBACK_HPP

#include <memory>

#include "cla/environment/core/CoreEnv.hpp"

namespace cla {

class CoreCLA; // for cross-referencing.


/**
 * CoreCallback implementation in C++.
 *
 * @b Description
 * The CoreCallback is the base (interface) class for callbacks. This
 * class uses template methods designed.
 */
class CoreCallback {

public:

	/**
	 * CoreCallback constructor.
	 */
	CoreCallback() = default;

	/**
	 * CoreCallback destructor.
	 */
	virtual ~CoreCallback() = default;

	/**
	 * Called the start of the processing.
	 *
	 * @param cla A kind of cla agents. It needs to get the internal
	 * data of cla. (ex: cla->getUnits();)
	 */
	virtual void doStartProcessing(const CoreCLA* cla) {}

	/**
	 * Called before beginning processing of a step.
	 *
	 * @param step The step this function called.
	 * @param inputs The input values from an environment in the step.
	 * @param cla A kind of cla agents. It needs to get the internal
	 * data of cla. (ex: cla->getUnits();)
	 */
	virtual void doPreProcessing(
		const Step step,
		const Values& inputs,
		const CoreCLA* cla
	) {}

	/**
	 * Called after beginning processing of a step.
	 *
	 * @param step The step this function called.
	 * @param inputs The input values from an environment in the step.
	 * @param nexts The input values form the environemnt in the next step.
	 * @param outputs The output values of CLA in the step.
	 * @param cla A kind of cla agents. It needs to get the internal
	 * data of cla. (ex: cla->getUnits();)
	 */
	virtual void doPostProcessing(
		const Step step,
		const Values& inputs,
		const Values& nexts,
		const Values& outputs,
		const CoreCLA* cla
	) {}

	/**
	 * Called the end of the processing.
	 *
	 * @param cla A kind of cla agents. It needs to get the internal
	 * data of cla. (ex: cla->getUnits();)
	 */
	virtual void doEndProcessing(const CoreCLA* cla) {}
};

using PCallback = std::shared_ptr<CoreCallback>;


} // namespace cla

#endif // CORE_CALLBACK_HPP