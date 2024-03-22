// CoreEnv.hpp

/** 
 * @file
 * Definitions for the CoreEnv class in C++
 */

#ifndef CORE_ENV_HPP
#define CORE_ENV_HPP

#include <iostream>
#include <vector>
#include <tuple>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>


namespace cla{

using Step = std::size_t;
using Dim = std::size_t;

using Value = double;
using Values = std::vector<Value>;

using json = nlohmann::json;


/**
 * CoreEnv implementation in C++
 * 
 * @b Description
 * The CoreEnv is an abstract class for the environment in which CLA is
 * trained. When the users creates a new environment, they have to override
 * the getValue function.
 */
class CoreEnv {

private:

	Step t_ = 0u;
	Dim dimension_ = 0u;

	Values maxs_;
	Values mins_;

protected:

	void _setDimension(const Dim dimension);

	void _setMaxs(const Values& maxs);

	void _setMins(const Values& mins);

public:

	/**
	 * CoreEnv constructor
	 */
	CoreEnv() = default;

	/**
	 * CoreEnv destructor
	 */
	virtual ~CoreEnv() = default;

	/**
	 * Reset the environment.
	 */
	virtual void reset();

	/**
	 * Get the values of the environment.
	 * 
	 * @return The values of the environment.
	 */
	virtual const Values getValues() const = 0;

	/**
	 * Get the inner Step of the environment.
	 * 
	 * @return The Step.
	 */
	virtual const Step& getStep() const;

	/**
	 * Increment the Step of the environment.
	 */
	virtual void increment();

	/**
	 * Get json config of the environment.
	 * 
	 * @return The json instance.
	 */
	virtual const json getJsonConfig() const;

	/**
	 * Get the dimension of values in the environment.
	 * 
	 * @return The dimensions.
	 */
	const Dim getDimension() const;
	/**
	 * Get the maximum values in each dimension.
	 * 
	 * @return The maximum values in each dimension.
	 */
	const Values& getMaxs() const;

	/**
	 * Get the minimum values in each dimension.
	 * 
	 * @return The minimum values in each dimension.
	 */
	const Values& getMins() const;

	/**
	 * Summary the environment.
	 * 
	 * @param os Tht output stream. default is std::cout.
	 */
	void summary(std::ostream& os = std::cout) const;

};

using PEnv = std::shared_ptr<CoreEnv>;


std::ostream& operator<<(std::ostream& os, const CoreEnv& env);
std::ostream& operator<<(std::ostream& os, const PEnv& penv);


} // namespace cla

#endif // CORE_ENV_HPP