// AdapterBuilder.hpp

/** 
 * @file
 * Definitions for the AdapterBuilder class in C++
 */

#ifndef ADAPTER_BUILDER_HPP
#define ADAPTER_BUILDER_HPP

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

#include "cla/model/core/CoreAdapter.hpp"


namespace cla {

using json = nlohmann::json;


/**
 * AdapterJsonBuilder implementation in C++.
 * 
 * @b Description
 * The AdapterJsonBuilder is for the adapter modules. This class creates the
 * adapter module instance according to the JSON config. The structure of
 * this class is based on the builder pattern of GOF.
 */
class AdapterJsonBuilder {

private:

	/**
	 * Build the DirectAdapter module instance.
	 * 
	 * @param config The config of  the DirectAdapter module on the JSON.
	 * @return PAdapter The pointer of the DirectAdapter module instance.
	 */
	static PAdapter buildDirectAdapter_(const json& config);

public:

	/**
	 * AdapterJsonBuilder constructor.
	 */
	AdapterJsonBuilder() = default;

	/**
	 * AdapterJsonBuilder destructor.
	 */
	~AdapterJsonBuilder() = default;

	/**
	 * Build the Adapter module instance.
	 * 
	 * @param config The config of the Adapter module on the JSON.
	 * @return PAdapter The pointer of the Adapter module instance.
	 */
	static PAdapter buildAdapter(const json& config);

	/**
	 * Build the Adapter module instance.
	 * 
	 * @param type The name of the module type.
	 * @param config The config of the Adapter module on the JSON.
	 * @return PAdapter The pointer of the Adapter module instance.
	 */
	static PAdapter buildAdapter(
		const std::string& type,
		const json& config
	);

};

} // namespace cla

#endif // ADAPTER_BUILDER_HPP