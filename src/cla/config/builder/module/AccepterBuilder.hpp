// AccepterBuilder.hpp

/** 
 * @file
 * Definitions for the AccepterBuilder class in C++
 */

#ifndef ACCEPTER_BUILDER_HPP
#define ACCEPTER_BUILDER_HPP

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

#include "cla/model/core/CoreAccepter.hpp"


namespace cla {

using json = nlohmann::json;


/**
 * AccepterJsonBuilder implementation in C++.
 * 
 * @b Description
 * The AccepterJsonBuilder is for the accepter modules. This class creates the
 * Accepter module instance according to the JSON config. The structure of
 * this class is based on the builder pattern of GOF.
 */
class AccepterJsonBuilder {

private:

	/**
	 * Build the FullAccepter module instance.
	 * 
	 * @param config The config of the FullAccepter module on the JSON.
	 * @return PAccepter The pointer of the FullAccepter module instance.
	 */
	static PAccepter buildFullAccepter_(const json& config);

	/**
	 * Build the IntensityAccepter module instance.
	 * 
	 * @param config The config of the IntensityAccepter module on the JSON.
	 * @return PAccepter The pointer of the IntensityAccepter module instance.
	 */
	static PAccepter buildIntensityAccepter_(const json& config);


public:

	/**
	 * AccepterJsonBuilder constructor.
	 */
	AccepterJsonBuilder() = default;

	/**
	 * AccepterJsonBuilder destructor.
	 */
	~AccepterJsonBuilder() = default;

	/**
	 * Build the Accepter module instance.
	 * 
	 * @param config The config of the Accepter module on the JSON.
	 * @return PAccepter The pointer of the Accepter module instance.
	 */
	static PAccepter buildAccepter(const json& config);

	/**
	 * Build the Accepter module instance.
	 * 
	 * @param type The name of the module type.
	 * @param config The config of the Accepter module on the JSON.
	 * @return PAccepter The pointer of the Accepter module instance.
	 */
	static PAccepter buildAccepter(
		const std::string& type, 
		const json& config
	);

};

} // namespace cla

#endif // ACCEPTER_BUILDER_HPP