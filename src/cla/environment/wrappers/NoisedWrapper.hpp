// NoisedWrapper.hpp

/** 
 * @file
 * Definitions for the NoisedWrapper class in C++
 */

#ifndef NOISED_WRAPPER_HPP
#define NOISED_WRAPPER_HPP

#include <random>

#include "cla/environment/core/CoreEnv.hpp"

namespace cla {

/**
 * NoisedWrapper implementation in C++.
 * 
 * @b Description
 * The NoisedWrapper is a wrapper for the environment. This class adds 
 * the Gaussian noise for all values from the wrapped environment.
 */
class NoisedWrapper : public CoreEnv {

private:

	mutable std::mt19937 mt_;
	Value sigma_;
	int seed_;

	PEnv env_;

public:

	inline static const std::string name = "NoisedWrapper";

public:

	/**
	 * NoisedWrapper constructor.
	 */
	NoisedWrapper() = default;

	/**
	 * NoisedWrapper constructor with the env and the parameters.
	 * 
	 * @param env The pointer of wrapped environment.
	 * @param sigma The variance value of the noise to be added.
	 * @param seed The seed value.
	 */
	NoisedWrapper(
		const PEnv& env, 
		const Value sigma = 0.01,
		const int seed = 1
	);

	/**
	 * NoisedWrapper destructor.
	 */
	~NoisedWrapper() = default;

	/**
	 * Initialize the NoisedWrapper with the env and the parameters.
	 * 
	 * @param env The pointer of wrapped environment.
	 * @param sigma The variance value of the noise to be added.
	 * @param seed The seed value.
	 */
	void initialize(
		const PEnv& env,
		const Value sigma = 0.01,
		const int seed = 1
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
	 * Get the environment pointer.
	 * 
	 * @returns The environment pointer.
	 */
	const PEnv& getEnv() const;


};

} // namespace cla

#endif // NOISED_WRAPPER_HPP