// ConfigHelpers.hpp

/**
 * @file
 * Definitions for the ConfigHelpers class in C++
 */

#ifndef CONFIG_HELPERS_HPP
#define CONFIG_HELPERS_HPP

#include <string>
#include <nlohmann/json.hpp>

namespace cla {

using json = nlohmann::json;


/**
 * KeyHelper implementation in C++.
 *
 * @b Description
 * The KeyHelper is the helper functions class for the builders and aligners. This
 * class has utility function for the key values.
 */
class KeyHelper {

public:

	/**
	 * KeyHelper constructor.
	 */
	KeyHelper() = default;

	/**
	 * KeyHelper destructor.
	 */
	~KeyHelper() = default;

	/**
	 * Check whether or not the key contains the words.
	 *
	 * @param key The key volume of the config.
	 * @param words The keywords.
	 * @return true The key contains the words.
	 * @return false The key does NOT contain the words.
	 */
	static const bool contain(
		const std::string& key,
		const std::string& words
	);

	/**
	 * Check whether or note the key contains the key.
	 * 
	 * @param config The json config. 
	 * @param key The key value.
	 * @return true The config contains the key.
	 * @return false The config dees NOT contain the key.
	 */
	static const bool contain(
		const json& config,
		const std::string& key
	);

};



/**
 * ConfigHelper implementation in C++.
 * 
 * @b Description
 * The ConfigHelper is the helper functions class for the builders and aligners. This
 * class has utility function for the config values.
 */
class ConfigHelper {

public:

	/**
	 * ConfigHelper constructor.
	 */
	ConfigHelper() = default;

	/**
	 * ConfigHelper destructor.
	 */
	~ConfigHelper() = default;

	/**
	 * Assign the value for the config according to the key.
	 * 
	 * @tparam T the assigned value type.
	 * @param config The config value.
	 * @param key The key of the config whose value is assigned.
	 * @param value The assigned value.
	 */
	template<typename T>
	static void assign(
		json& config, 
		const std::string& key,
		const T& value
	) {
		if(KeyHelper::contain(config, key)) {
			config.at(key) = value;
		} else {
			config[key] = value;
		}
	}

};

} // namespace cla

#endif // CONFIG_HELPERS_HPP