// SenderBuilder.hpp

/** 
 * @file
 * Definitions for the SenderBuilder class in C++
 */

#ifndef SENDER_BUILDER_HPP
#define SENDER_BUILDER_HPP

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

#include "cla/model/core/CoreSender.hpp"


namespace cla {

using json = nlohmann::json;


/**
 * SenderJsonBuilder implementation in C++.
 * 
 * @b Description
 * The SenderJsonBuilder is for the Sender modules. This class creates the
 * Sender module instance according to the JSON config. The structure of
 * this class is based on the builder pattern of GOF.
 */
class SenderJsonBuilder {

private:

	/**
	 * Build the ActiveColumnSender module instance.
	 * @param config The config of the ActiveColumnSender module on the JSON.
	 * @return PSender The pointer of the ActiveColumnSender module instance.
	 */
	static PSender buildActiveColumnSender_(const json& config);
	
	/**
	 * Build the BurstColumnSender module instance.
	 * 
	 * @param config The config of the BurstColumnSender module on the JSON.
	 * @return PSender The pointer of the BurstColumnSender module instance.
	 */
	static PSender buildBurstColumnSender_(const json& config);


public:

	/**
	 * SenderJsonBuilder constructor.
	 */
	SenderJsonBuilder() = default;

	/**
	 * SenderJsonBuilder destructor.
	 */
	~SenderJsonBuilder() = default;

	/**
	 * Build the Sender module instance.
	 * 
	 * @param config The config of the Sender module on the JSON.
	 * @return PSender The pointer of the Sender module instance.
	 */
	static PSender buildSender(const json& config);

	/**
	 * Build the Sender module instance.
	 * 
	 * @param type The name of the module type.
	 * @param config The config of the Sender module on the JSON.
	 * @return PSender The pointer of the Sender module instance.
	 */
	static PSender buildSender(
		const std::string& type, 
		const json& config
	);

};

} // namespace cla

#endif // SENDER_BUILDER_HPP