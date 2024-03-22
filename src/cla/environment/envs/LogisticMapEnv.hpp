// LogisticMapEnv.hpp

/** 
 * @file
 * Definitions for the LogisticMapEnv class in C++
 */

#ifndef LOGISTIC_MAP_ENV_HPP
#define LOGISTIC_MAP_ENV_HPP

#include "cla/environment/core/CoreEnv.hpp"

namespace cla {

/**
 * LogisticMapEnv implementation in C++.
 * 
 * @b Description
 * The TriEnv is the environment of logistic map.
 */
class LogisticMapEnv : public CoreEnv {

private:

	Value alpha_ = 4.0f;
	mutable Value previous_ = 0.5;

public:

	inline static const std::string name = "LogisticMapEnv";

public:

	/**
	 * LogisticMapEnv constructor.
	 * 
	 * @param alpha The parameter of the logistic map.
	 * @param init The initial value of the logistic map.
	 */
	LogisticMapEnv(
		const Value alpha = 4.0f, 
		const Value init = 0.5f
	);

	/**
	 * LogisticMapEnv destructor.
	 */
	~LogisticMapEnv() = default;

	/**
	 * Get the value of the logistic map env.
	 * 
	 * @return The value of the logistic map env.
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

#endif // LOGISTIC_MAP_ENV_HPP