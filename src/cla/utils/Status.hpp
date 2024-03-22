// Status.hpp

/**
 * @file
 * Definitions for the Status class in C++
 */

#ifndef STATUS_HPP
#define STATUS_HPP

#include <string>
#include <unordered_map>

namespace cla {

/**
 * CLA statuses definitions in C++.
 */
enum class Status {
	READY,
	SLEEP,
	RUN
};


/**
 * StatusFuncs implementation in C++.
 *
 * @b Description
 * The StatusFuncs define the functions for the cla statuses.
 */
class StatusFuncs {

private:

	inline static std::unordered_map<std::string, Status> _map = {
		{"Ready", Status::READY},
		{"Sleep", Status::SLEEP},
		{"Run", Status::RUN}
	};

public:

	/**
	 * StatusFuncs constructor.
	 */
	StatusFuncs() = default;

	/**
	 * StatusFuncs destructor.
	 */
	~StatusFuncs() = default;

	/**
	 * Convert a status name to the status.
	 * 
	 * @param name An inputted status name
	 * @return const Status 
	 */
	static const Status toStatus(const std::string& name);

	/**
	 * Convert a status to the status name.
	 * 
	 * @param status An inputted status
	 * @return const std::string The status name.  
	 */
	static const std::string toName(const Status& status);

	/**
	 * Check the status name in statuses.
	 * 
	 * @param name An inputted status name.
	 * @return true 
	 * @return false 
	 */
	static const bool isExist(const std::string& name);

};

} // namespace cla

#endif // STATUS_HPP