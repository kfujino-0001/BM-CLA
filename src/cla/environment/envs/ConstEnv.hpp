// ConstEnv.hpp

/** 
 * @file
 * Definitions for the ConstEnv class in C++
 */

#ifndef CONST_ENV_HPP
#define CONST_ENV_HPP

#include "cla/environment/core/CoreEnv.hpp"

namespace cla {

/**
 * ConstEnv implementation in C++.
 * 
 * @b Description
 * The ConstEnv is the environment of the const value.
 */
class ConstEnv : public CoreEnv {

private:

	Value value_;

public:

	inline static const std::string name = "ConstEnv";

public:

	/**
	 * ConstEnv constructor.
	 * 
	 * @param value The const value
	 */
	ConstEnv(const Value value);

	/**
	 * ConstEnv constructor.
	 * 
	 * @param value The const value
	 * @param min The min value.
	 * @param max The max value.
	 */
	ConstEnv(
		const Value value,
		const Value min,
		const Value max
	);

	/**
	 * ConstEnv destructor.
	 */
	~ConstEnv() = default;

	/**
	 * Get the value of sin env.
	 * 
	 * @return The const value.
	 */
	const Values getValues() const override;

	/**
	 * Get json config of the environment.
	 * 
	 * @return The json instance.
	 */
	const json getJsonConfig() const override;

};

} // namespace cla

#endif // CONST_ENV_HPP