// SequentialWrapper.hpp

/** 
 * @file
 * Definitions for the SequentialWrapper class in C++
 */

#ifndef SEQUENTIAL_WRAPPER_HPP
#define SEQUENTIAL_WRAPPER_HPP

#include <utility>

#include "cla/environment/core/CoreEnv.hpp"

namespace cla {

using EnvSection =  std::pair<PEnv, Step>;

/**
 * SequentialWrapper implementation in C++.
 * 
 * @b Description
 * The SequentialWrapper is a wrapper for the environment. This class 
 * sequentially switches the environment that outputs the values at 
 * each constant step.
 */
class SequentialWrapper : public CoreEnv {

private: // definitions.

	using Idx = std::size_t;

private:

	std::vector<PEnv> envs_;
	std::vector<Step> sections_;

	Idx selected_; // selected environment index.
	Step wavelength_; // sum of steps in all sections, wavelength of this class.

public:

	inline static const std::string name = "SequentialWrapper";

public:

	/**
	 * SequentialWrapper constructor.
	 */
	SequentialWrapper() = default;

	/**
	 * SequentialWrapper constructor with the sections, which represents
	 * the environment and step.
	 * 
	 * @param sections The sections, which represents the environment
	 * and step.
	 */
	SequentialWrapper(const std::vector<EnvSection>& sections);

	/**
	 * SequentialWrapper destructor.
	 */
	~SequentialWrapper() = default;

	/**
	 * Initialize the SequentialWrapper with the sections, which represents
	 * the environment and step.
	 * 
	 * @param sections The sections, which represents the environment
	 * and step.
	 */
	void initialize(const std::vector<EnvSection>& sections);

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
	const std::vector<Step> getSectionSteps() const;

};

} // namespace cla

#endif // SEQUENTIAL_WRAPPER_HPP