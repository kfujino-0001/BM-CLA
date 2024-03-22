// ConfigHelpers.cpp

/**
 * @file
 * Implementation of ConfigHelpers.cpp
 */

#include "cla/config/utils/ConfigHelpers.hpp"

namespace cla {

const bool KeyHelper::contain(
	const std::string& key,
	const std::string& words
) {
	return key.find(words) != std::string::npos;
}

const bool KeyHelper::contain(
	const json& config,
	const std::string& key
) {
	return config.find(key) != config.end();
}

} // namespace cla