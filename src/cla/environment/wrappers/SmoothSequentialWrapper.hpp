// SmoothSequentialWrapper.hpp

/** 
 * @file
 * Definitions for the SmoothSequentialWrapper class in C++
 */

#ifndef SMOOTH_SEQUENTIAL_WRAPPER_HPP
#define SMOOTH_SEQUENTIAL_WRAPPER_HPP

#include <utility>
#include <vector>

#include "cla/environment/core/CoreEnv.hpp"
#include "cla/environment/wrappers/SequentialWrapper.hpp"

namespace cla {

/**
 * SmoothSequentialWrapper implementation in C++.
 * 
 * @b Description
 * The SmoothSequentialWrapper is a wrapper for the environment. This class
 * smooth switches the environment that outputs the values at 
 * each constant step.
 */
class SmoothSequentialWrapper : public CoreEnv {

private: // definitions.

	using Idx = std::size_t;

private:

	inline static const Value sigc_ = 10.0; // coefficient of the sig function.

	std::vector<PEnv> envs_;
	std::vector<Step> lengths_;
	std::vector<Step> sections_;

	Step wavelength_; // sum of steps in all sections, wavelength of this class.
	Step overlappedStep_;

public:

	inline static const std::string name = "SmoothSequentialWrapper";

private:

	const Value sig_(const Value val) const;

	const Value theta_(
		const Step length,
		const Step step
	) const;

	const Value delta_(
		const Step length,
		const Step step,
		const Step section,
		const Step fullLength
	) const;

public:

	/**
	 * SmoothSequentialWrapper constructor.
	 */
	SmoothSequentialWrapper() = default;

	/**
	 * SmoothSequentialWrapper constructor with the parameters.
	 * 
	 * @param sections The sections, which represents the environment
	 * and step.
	 * @param overlappedStep The overlapped step. This value is required 
	 * 2 * overlappedStep <= a min environment step.
	 */
	SmoothSequentialWrapper(
		const std::vector<EnvSection>& sections,
		const Step overlappedStep
	);

	/**
	 * SmoothSequentialWrapper destructor.
	 */
	~SmoothSequentialWrapper() = default;

	/**
	 * Initialize the SmoothSequentialWrapper with the sections, which represents
	 * the environment and step.
	 * 
	 * @param sections The sections, which represents the environment
	 * and step.
	 * @param overlappedStep The overlapped step. This value is required 
	 * 2 * overlappedStep <= a min environment step.
	 */
	void initialize(
		const std::vector<EnvSection>& sections,
		const Step overlappedStep
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
	const std::vector<Step> getSectionSteps() const;

};

} // namespace cla

#endif // SMOOTH_SEQUENTIAL_WRAPPER_HPP