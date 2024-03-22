// InvertedWrapper.hpp

/** 
 * @file
 * Definitions for the InvertedWrapper class in C++
 */

#ifndef INVERTED_WRAPPER_HPP
#define INVERTED_WRAPPER_HPP

#include <functional>

#include "cla/environment/core/CoreEnv.hpp"

namespace cla {

/**
 * InvertedWrapper implementation in C++.
 * 
 * @b Description
 * The InvertedWrapper is a the wrapper for environments. This class is 
 * multiplied by the minus for all values from the wrapped environment.
 */
class InvertedWrapper : public CoreEnv {

private:

	PEnv env_;
	inline static const std::function<Value(Value)> minus_ = [](const Value value){
		return -value;
	};

public:

	inline static const std::string name = "InvertedWrapper";

public:

	/**
	 * InvertedWrapper constructor.
	 */
	InvertedWrapper() = default;

	/**
	 * InvertedWrapper constructor with the env.
	 * 
	 * @param env The pointer of wrapped environment.
	 */
	InvertedWrapper(const PEnv& env);

	/**
	 * InvertedWrapper destructor.
	 */
	~InvertedWrapper() = default;

	/**
	 * Initialize the InvertedWrapper with the env.
	 * 
	 * @param env The environment pointer.
	 */
	void initialize(const PEnv& env);

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

#endif // INVERTED_WRAPPER_HPP