// CompositeCallback.hpp

/** 
 * @file
 * Definitions for the CompositeCallback class in C++
 */

#ifndef COMPOSITE_CALLBACK_HPP
#define COMPOSITE_CALLBACK_HPP

#include <vector>

#include "cla/model/core/CoreCallback.hpp"

namespace cla {

/**
 * CompositeCallback implementation in C++.
 * 
 * @b Description
 * CompositeCallback is one of the Callback-series. This class merges 
 * other Callbacks and runs them simultaneously.
 */
class CompositeCallback : public CoreCallback {

private:

	std::vector<PCallback> callbacks_;

public:

	/**
	 * CompositeCallback constructor.
	 */
	CompositeCallback() = default;

	/**
	 * CompositeCallback constructor.
	 * 
	 * @param callbacks The composite callbacks.
	 */
	CompositeCallback(const std::vector<PCallback>& callbacks);

	/**
	 * CompositeCallback destructor.
	 */
	~CompositeCallback() = default;

	/**
	 * Initialize Composite callbacks.
	 * 
	 * @param callbacks The composite callbacks.
	 */
	void initialize(const std::vector<PCallback>& callbacks);

	/**
	 * Add a callback.
	 * 
	 * @param callback The callback to be registered. 
	 */
	void add(const PCallback& callback);

	/**
	 * Called the start of the processing.
	 * 
	 * @param cla A kind of cla agents. It needs to get the internal
	 * data of cla. (ex: cla->getUnits();)
	 */
	void doStartProcessing(const CoreCLA* cla) override;

	/**
	 * Called before beginning processing of a step.
	 * 
	 * @param step The step this function called.
	 * @param inputs The input values from an environment in the step.
	 * @param cla A kind of cla agents. It needs to get the internal
	 * data of cla. (ex: cla->getUnits();)
	 */
	void doPreProcessing(
		const Step step,
		const Values& inputs,
		const CoreCLA* cla
	) override;

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
	void doPostProcessing(
		const Step step,
		const Values& inputs,
		const Values& nexts,
		const Values& outputs,
		const CoreCLA* cla
	) override;

	/**
	 * Called the end of the processing.
	 * 
	 * @param cla A kind of cla agents. It needs to get the internal
	 * data of cla. (ex: cla->getUnits();)
	 */
	void doEndProcessing(const CoreCLA* cla) override;

};

} // namespace cla

#endif // COMPOSITE_CALLBACK_HPP