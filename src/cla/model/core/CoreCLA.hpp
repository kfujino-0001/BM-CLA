// CoreCLA.hpp

/**
 * @file
 * Definitions for the CoreCLA class in C++
 */

#ifndef CORE_CLA_HPP
#define CORE_CLA_HPP

#include <iostream>
#include <memory>

#include "cla/environment/core/CoreEnv.hpp"
#include "cla/model/core/CoreLayer.hpp"
#include "cla/model/core/CoreIO.hpp"

namespace cla {

class CoreCallback; // for cross-referencing
using PCallback = std::shared_ptr<CoreCallback>;


/**
 * CoreCLA implementation in C++.
 *
 * @b Description
 * The CoreCLA is the base (interface) class for cla models. This class
 * defines the fitting and testing of the CLA models.
 */
class CoreCLA {

private:

	/**
	 * Compile the given environment.
	 *
	 * @param nbStep The learning step (learning iteration).
	 * @param verbose The variable to set how the learning status is
	 * displayed.
	 * @param learn The cla mode whether learning or testing.
	 * @param env The envrionment which CLA model learns. (The environment
	 * class needs to extend the CoreEnv).
	 * @param callback The callback which defines additional processing
	 * during fitting to be determined externally.
	 */
	void compile_(
		const Step nbStep,
		const int verbose,
		const bool learn,
		PEnv& env,
		PCallback& callback
	);

	/**
	 * Print the next values and prediction values on each step.
	 *
	 * @param step The value of step.
	 * @param nexts The next values from the given environment.
	 * @param predictions The prediction values from the cla model.
	 */
	void printLog_(
		const Step step,
		const Values& nexts,
		const Values& predictions
	) const;

public:

	/**
	 * CoreCLA constructor.
	 */
	CoreCLA() = default;

	/**
	 * CoreCLA destructor.
	 */
	virtual ~CoreCLA() = default;


	/************************************************
	 * Virtual functions which needs override.
	 ***********************************************/

	/**
	 * Reset the cla model.
	 */
	virtual void reset() = 0;

	/**
	 * Summarize the cla model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	virtual void summary(std::ostream& os = std::cout) const = 0;

	/**
	 * Show the detail of the cla model.
	 * 
	 * @param os The output stream. The default os is std::cout.
	 */
	virtual void detail(std::ostream& os = std::cout) const = 0;

	/**
	 * Feedforward input values to the prediction values. This function is
	 * uses in the fit and test.
	 *
	 * @param inputs The input values which is vector of float.
	 * @param learn The boolean value whether learning mode.
	 * @return const Values: The prediction values.
	 */
	virtual const Values feedforward(
		const Values& inputs,
		const bool learn
	) = 0;

	/**
	 * Feedback the next values to the cla model.
	 * This function performs additional learning from input values from
	 * the environment at a given next time step.
	 *
	 * @param nexts The input values at a given next time step.
	 * @param learn The boolean value whether learning model.
	 */
	virtual void feedback(const Values& nexts, const bool learn) = 0;

	/**
	 * Get the layer proxies of the cla model.
	 * 
	 * @return const std::vector<PLayerProxy>: The layer proxies of the cla model.
	 */
	virtual const std::vector<PLayerProxy> getLayers() const = 0;

	/**
	 * Get the pointer of the io module.
	 * 
	 * @return const PIO& The pointer io module.
	 */
	virtual const PIO& getIO() const = 0;


	/************************************************
	 * public functions.
	 ***********************************************/

	/**
	 * Fit the cla model to the given environment.
	 *
	 * @param nbStep The learning step (learning iteration).
	 * @param verbose The variable to set how the learning status is
	 * displayed.
	 * @param env The envrionment which CLA model learns. (The environment
	 * class needs to extend the CoreEnv).
	 */
	void fit(const Step nbStep, const int verbose, PEnv& env);

	/**
	 * Fit the cla model to the given environment.
	 *
	 * @param nbStep The learning step (learning iteration).
	 * @param verbose The variable to set how the learning status is
	 * displayed.
	 * @param env The envrionment which CLA model learns. (The environment
	 * class needs to extend the CoreEnv).
	 * @param callback The callback which defines additional processing
	 * during fitting to be determined externally.
	 */
	void fit(
		const Step nbStep,
		const int verbose,
		PEnv& env,
		PCallback& callback
	);

	/**
	 * Test the cla model on the given environment.
	 *
	 * @param nbStep The learning step (learning iteration).
	 * @param verbose The variable to set how the learning status is
	 * displayed.
	 * @param env The envrionment which CLA model learns. (The environment
	 * class needs to extend the CoreEnv).
	 */
	void test(const Step nbStep, const int verbose, PEnv& env);

	/**
	 * Test the cla model on the given environment.
	 *
	 * @param nbStep The learning step (learning iteration).
	 * @param verbose The variable to set how the learning status is
	 * displayed.
	 * @param env The envrionment which CLA model learns. (The environment
	 * class needs to extend the CoreEnv).
	 * @param callback The callback which defines additional processing
	 * during fitting to be determined externally.
	 */
	void test(
		const Step nbStep,
		const int verbose,
		PEnv& env,
		PCallback& callback
	);
	
};

using PCLA = std::unique_ptr<CoreCLA>;


std::ostream& operator<<(std::ostream& os, const CoreCLA& cla);
std::ostream& operator<<(std::ostream& os, const PCLA& pcla);


} // namespace cla

#endif // CORE_CLA_HPP