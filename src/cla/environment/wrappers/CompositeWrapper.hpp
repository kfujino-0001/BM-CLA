// CompositeWrapper.hpp

/** 
 * @file
 * Definitions for the CompositeWrapper class in C++
 */

#ifndef COMPOSITE_WRAPPER_HPP
#define COMPOSITE_WRAPPER_HPP

#include <vector>
#include "cla/environment/core/CoreEnv.hpp"

namespace cla {

/**
 * CompositeWrapper implementation in C++.
 * 
 * @b Description
 * The CompositeWrapper is a the wrapper for environments. This class 
 * composites some environments.
 */
class CompositeWrapper : public CoreEnv {

private:

	std::vector<PEnv> envs_;

public:

	inline static const std::string name = "CompositeWrapper";

public:

	/**
	 * CompositeWrapper constructor.
	 */
	CompositeWrapper() = default;

	/**
	 * CompositeWrapper constructor with the environments.
	 * 
	 * @param envs The pointers of wrapped environment.
	 */
	CompositeWrapper(const std::vector<PEnv>& envs);

	/**
	 * CompositeWrapper destructor.
	 */
	~CompositeWrapper() = default;

	/**
	 * Initialize the CompositeWrapper with the envs.
	 * 
	 * @param envs The pointers of wrapped environment.
	 */
	void initialize(const std::vector<PEnv>& envs);

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

};

} // namespace cla

#endif // COMPOSITE_WRAPPER_HPP