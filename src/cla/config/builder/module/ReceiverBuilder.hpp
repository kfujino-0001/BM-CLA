// ReceiverBuilder.hpp

/** 
 * @file
 * Definitions for the ReceiverBuilder class in C++
 */

#ifndef RECEIVER_BUILDER_HPP
#define RECEIVER_BUILDER_HPP

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

#include "cla/model/core/CoreReceiver.hpp"


namespace cla {

using json = nlohmann::json;


/**
 * ReceiverJsonBuilder implementation in C++.
 * 
 * @b Description
 * The ReceiverJsonBuilder is for the Receiver modules. This class creates the
 * Receiver module instance according to the JSON config. The structure of
 * this class is based on the builder pattern of GOF.
 */
class ReceiverJsonBuilder {

private:

	/**
	 * Build the ActiveCellReceiver module instance.
	 * 
	 * @param config The config of the ActiveCellReceiver module on the JSON.
	 * @return PReceiver The pointer of the ActiveCellReceiver module instance.
	 */
	static PReceiver buildActiveCellReceiver_(const json& config);

	/**
	 * Build the VolatileActiveCellReceiver module instance.
	 * 
	 * @param config The config of the VolatileActiveCellReceiver module on the JSON.
	 * @return PReceiver The pointer of the VolatileActiveCellReceiver module instance.
	 */
	static PReceiver buildVolatileActiveCellReceiver_(const json& config);

public:

	/**
	 * ReceiverJsonBuilder constructor.
	 */
	ReceiverJsonBuilder() = default;

	/**
	 * ReceiverJsonBuilder destructor.
	 */
	~ReceiverJsonBuilder() = default;

	/**
	 * Build the Receiver module instance.
	 * 
	 * @param config The config of the Receiver module on the JSON.
	 * @return PReceiver The pointer of the Receiver module instance.
	 */
	static PReceiver buildReceiver(const json& config);

	/**
	 * Build the Receiver module instance.
	 * 
	 * @param type The name of the module type.
	 * @param config The config of the Receiver module on the JSON.
	 * @return PReceiver The pointer of the Receiver module instance.
	 */
	static PReceiver buildReceiver(
		const std::string& type, 
		const json& config
	);

};

} // namespace cla

#endif // RECEIVER_BUILDER_HPP