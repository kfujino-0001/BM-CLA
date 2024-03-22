// AdjusterFunctions.hpp

/** 
 * @file
 * Definitions for the AdjusterFunctions class in C++
 */

#ifndef ADJUSTER_FUNCTIONS_HPP
#define ADJUSTER_FUNCTIONS_HPP

#include <iostream>
#include <algorithm>
#include <type_traits>

#include <htm/types/Types.hpp>

namespace htm {

/**
 * AdjusterInterface implementation in C++.
 * 
 * @b Description
 * AdjusterInterface is the basic function that adjusts the effect on 
 * the number of synapses added or deleted in the partial prediction
 * accuracy of segment (ppas).
 */
template<class ClassType>
struct AdjusterInterface {

	/**
	 * AdjusterInterface constructor.
	 */
	AdjusterInterface() = default;

	/**
	 * AdjusterInterface destructor.
	 */
	~AdjusterInterface() = default;

	/**
	 * Adjust the impact of ppas based of the weigh.
	 * 
	 * @param ppas The partial prediction accuracy.
	 * @param weight The weight parameter for adjusting ppas.
	 * 
	 * @return The adjusted ppas.
	*/
	const Real adjust(const Real ppas, const Real weight) const {
		return static_cast<ClassType&>(this)->adjust(ppas, weight);
	}
};



namespace adjuster {

struct Pow : public AdjusterInterface<Pow> {
	const Real adjust(const Real ppas, const Real weight) const {
		return std::pow(ppas, weight);
	}
};

struct Linear : public AdjusterInterface<Linear> {
	const Real adjust(const Real ppas, const Real weight) const {
		return std::min(weight * ppas, 1.0f);
	}
};

} // namespace adjuster



/**
 * Adjuster implementation in C++.
 * 
 * @b Description
 * The adjuster class is a class that handles code changes caused by 
 * switching between various functions.
 * 
 * An example of the use.
 * 
 * Adjuster<adjusters::Pow> adjuster;
 * auto result = adjuster(ppas, weight);
 */
template<
	class ClassType, 
	bool isExtended 
		= std::is_base_of<AdjusterInterface<ClassType>, ClassType>::value
	>
class Adjuster {
	static_assert(isExtended, "ClassType is not extended interface class");
};

template<class ClassType>
class Adjuster<ClassType, true> {

private:

	ClassType obj_;

public:

	/**
	 * Adjuster constructor.
	 */
	Adjuster() = default;

	/**
	 * Adjuster destructor.
	 */
	~Adjuster() = default;

	/**
	 * Adjust the impact of ppas based of the weigh.
	 * 
	 * @param ppas The partial prediction accuracy.
	 * @param weight The weight parameter for adjusting ppas.
	 * 
	 * @return The adjusted ppas.
	*/
	const Real operator()(const Real ppas, const Real weight) const {
		return obj_.adjust(ppas, weight);
	}

};

} // namespace htm

#endif // ADJUSTER_FUNCTIONS_HPP