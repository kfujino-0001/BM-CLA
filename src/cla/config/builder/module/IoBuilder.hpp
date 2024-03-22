// IoBuilder.hpp

/** 
 * @file
 * Definitions for the IoBuilder class in C++
 */

#ifndef IO_BUILDER_HPP
#define IO_BUILDER_HPP

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

#include "cla/model/core/CoreIO.hpp"

namespace cla {

using json = nlohmann::json;


/**
 * IoJsonBuilder implementation in C++.
 * 
 * @b Description
 * The IoJsonBuilder is for the io modules. This class creates the
 * io module instance according to the JSON config. The structure of
 * this class is based on the builder pattern of GOF.
 */
class IoJsonBuilder {

private:

	/**
	 * Build the scalar io module instance.
	 * 
	 * @param config The config of the scalar io module on the JSON.
	 * @return PIO The pointer of the scalar io module instance.
	 */
	static PIO buildScalarIo_(const json& config);

public:

	/**
	 * IoJsonBuilder constructor.
	 */
	IoJsonBuilder() = default;

	/**
	 * IoJsonBuilder destructor.
	 */
	~IoJsonBuilder() = default;

	/**
	 * Build the io module instance.
	 * 
	 * @param config The config of the io module on the JSON.
	 * @return PIO The pointer of the io module instance.
	 */
	static PIO buildIO(const json& config);

	/**
	 * Build the io module instance.
	 * 
	 * @param type The name of the module type.
	 * @param config The config of the io module on the JSON.
	 * @return PIO The pointer of the io module instance.
	 */
	static PIO buildIO(
		const std::string& type,
		const json& config
	);

};

} // namespace cla

#endif // IO_BUILDER_HPP