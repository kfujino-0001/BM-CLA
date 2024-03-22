// EvalCallback.hpp

/** 
 * @file
 * Definitions for the EvalCallback class in C++
 */

#ifndef EVAL_CALLBACK_HPP
#define EVAL_CALLBACK_HPP

#include "cla/model/core/CoreCallback.hpp"
#include "cla/environment/core/CoreEnv.hpp" // for value definitions.
#include "cla/utils/Evaluations.hpp"

namespace cla {

/**
 * EvalCallback implementation in C++.
 * 
 * @b Description
 * EvalCallback is one of the Callback-series. The class calculates the
 * prediction accuracy from the received input/output and prints it to
 * the standard output.
 */
class EvalCallback : public CoreCallback {

private:

	Dim valueDimension_;
	Step nbEvalSteps_;
	int verbose_;

	std::vector<evaluations::Evaluations> evals_;

private:

	void printLog_(const Step step);

public:

	/**
	 * EvalCallback constructor.
	 */
	EvalCallback() = default;

	/**
	 * EvalCallback constructor with the parameters.
	 * 
	 * @param valueDimension The dimension of input or output values.
	 * @param nbEvalSteps The number of steps to calculate one evaluation.
	 * @param verbose The variable to set how the learning status is
	 * displayed.
	 */
	EvalCallback(
		const Dim valueDimension,
		const Step nbEvalSteps,
		const int verbose = 1
	);

	/**
	 * EvalCallback destructor.
	 */
	~EvalCallback() = default;

	/**
	 * Initialize the EvalCallback with the parameters.
	 * 
	 * @param valueDimension The dimension of input or output values.
	 * @param nbEvalSteps The number of steps to calculate one evaluation.
	 * @param verbose The variable to set how the learning status is
	 * displayed.
	 */
	void initialize(
		const Dim valueDimension,
		const Step nbEvalSteps,
		const int verbose = 1
	);

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

};

} // namespace cla

#endif // EVAL_CALLBACK_HPP