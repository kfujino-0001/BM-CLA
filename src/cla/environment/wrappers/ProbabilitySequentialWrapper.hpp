// ProbabilitySequentialWrapper.hpp

/** 
 * @file
 * Definitions for the ProbabilitySequentialWrapper class in C++
 */

#ifndef PROBABILITY_SEQUENTIAL_WRAPPER_HPP
#define PROBABILITY_SEQUENTIAL_WRAPPER_HPP

#include <iostream>
#include <vector>
#include <random>

#include "cla/environment/core/CoreEnv.hpp"
#include "cla/environment/wrappers/SequentialWrapper.hpp"

namespace cla {

/**
 * ProbabilitySequentialWrapper implementation in C++.
 * 
 * @b Description
 * The ProbabilitySequentialWrapper is a wrapper for the environment. This class 
 * probabilistically switches the environment that outputs the values at 
 * each constant step.
 */
class ProbabilitySequentialWrapper : public CoreEnv {

private: // definitions.

	using Idx = std::size_t;

private:

	std::vector<PEnv> envs_;
	std::vector<Step> sections_;

	Idx selected_;
	Step subStep_;

	std::mt19937 mt_;
	int seed_;

public:

	inline static const std::string name = "ProbabilitySequentialWrapper";

private:

	const Idx select_();

public:

	/**
	 * ProbabilitySequentialWrapper constructor.
	 */
	ProbabilitySequentialWrapper() = default;

	/**
	 * ProbabilitySequentialWrapper constructor with the parameters.
	 * 
	 * @param sections The sections, which represents the environment
	 * and step.
	 * @param seed The seed value.
	 */
	ProbabilitySequentialWrapper(
		const std::vector<EnvSection>& sections,
		const int seed = 0
	);

	/**
	 * ProbabilitySequentialWrapper destructor.
	 */
	~ProbabilitySequentialWrapper() = default;

	/**
	 * Initialize the CompositeWrapper with the sections, which represents
	 * the environment and step.
	 * 
	 * @param sections The sections, which represents the environment
	 * and step.
	 * @param seed The seed value.
	 */
	void initialize(
		const std::vector<EnvSection>& sections,
		const int seed = 0
	);

	/**
	 * Reset the environment.
	 */
	void reset() override;

	/**
	 * Get the values of the environment.
	 * 
	 * @return The values of the environment.
	 */
	const Values getValues() const override;

	/**
	 * Get json config of the environment.
	 * 
	 * @return The json instance.
	 */
	const json getJsonConfig() const override;

	/**
	 * Increment the Step of the environment.
	 */
	void increment() override;

	/**
	 * Get the environment pointers.
	 * 
	 * @returns The pointers of wrapped environment.
	 */
	const std::vector<PEnv>& getEnvs() const;

	/**
	 * Get the steps each section in this wrapper.
	 * 
	 * @returns The vector value of steps each section.
	 */
	const std::vector<Step>& getSectionSteps() const;

};

} // namespace cla

#endif // PROBABILITY_SEQUENTIAL_WRAPPER_HPP