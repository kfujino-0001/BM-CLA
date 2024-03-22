// SaveModelLogCallback.hpp

/** 
 * @file
 * Definitions for the SaveModelLogCallback class in C++
 */

#ifndef SAVE_MODEL_LOG_CALLBACKS_HPP
#define SAVE_MODEL_LOG_CALLBACKS_HPP

#include "cla/utils/Csv.hpp"
#include "cla/model/module/callback/SaveCallback.hpp"

namespace cla {

using Header = csv::Header;

/**
 * ModelHistory implementation in C++.
 * 
 * @b Description
 * The ModelHistory is a data structure. It contains the input, output and
 * inner information at the step.
 * 
 * @param step
 * The step of this history.
 * 
 * @param inputs
 * The input values from the environment.
 * 
 * @param nexts
 * The next input values from the environment.
 * 
 * @param predictions
 * The predictive values from the cla. (The smaller the difference between
 * the predictions and the next input values are the better.)
 */
struct ModelHistory {

	Step step;
	Values inputs;
	Values nexts;
	Values predictions;

public:

	static inline const std::string stepHeader = "step";
	static inline const std::string inputHeader = "input_";
	static inline const std::string nextInputHeader = "next_input_";
	static inline const std::string predictionHeader = "prediction_";

public:

	/**
	 * ModelHistory constructor.
	 */
	ModelHistory() = default;

	/**
	 * ModelHistory constructor.
	 */
	ModelHistory(
		const Step step,
		const Values& inputs,
		const Values& nexts,
		const Values& predictions
	);

	/**
	 * ModelHistory destructor.
	 */
	~ModelHistory() = default;

	/**
	 * Get the header of the cla history.
	 * 
	 * @param nbValues The number of the values.
	 * 
	 * @returns The header of the cla history.
	 */
	static Header getHeader(const std::size_t nbValues);

public:

	friend std::ostream& operator<<(std::ostream& os, const ModelHistory& h);

};

std::ostream& operator<<(std::ostream& os, const ModelHistory& h);



/**
 * SaveModelLogCallback implementation in C++.
 * 
 * @b Description
 * SaveModelLogCallback is one of the Callback-series. This class saves
 * the input/output of the model to a csv file.
 */
class SaveModelLogCallback : public SaveCallback {

private:

	std::vector<ModelHistory> hists_;
	csv::Csv csv_;

	Dim valueDimension_;

public:

	/**
	 * SaveModelLogCallback constructor.
	 */
	SaveModelLogCallback() = default;

	/**
	 * SaveModelLogCallback constructor with the parameters.
	 * 
	 * @param path The file path where the data will be saved.
	 * @param filename The filename where the data will be saved.
	 * @param valueDimension The dimension of input or output values.
	 * @param nbHoldingSteps The number of steps to hold the data in
	 * the real memory. Adjusting this value reduces the time required
	 * to access io and speeds up the process.
	 */
	SaveModelLogCallback(
		const std::string& path,
		const std::string & filename,
		const Dim valueDimension,
		const Step nbHoldingSteps
	);

	/**
	 * SaveModelLogCallback destructor.
	 */
	~SaveModelLogCallback() = default;

	/**
	 * Initialize SaveModelLogCallback with the parameters.
	 * 
	 * @param path The file path where the data will be saved.
	 * @param filename The filename where the data will be saved.
	 * @param valueDimension The dimension of input or output values.
	 * @param nbHoldingSteps The number of steps to hold the data in
	 * the real memory. Adjusting this value reduces the time required
	 * to access io and speeds up the process.
	 */
	void initialize(
		const std::string& path,
		const std::string & filename,
		const Dim valueDimension,
		const Step nbHoldingSteps
	);

	/**
	 * Reset the SaveCallback.
	 */
	void reset() override;

	/**
	 * Open the file where the data will be saved.
	 */
	void open() override;

	/**
	 * Add the data to container.
	 * 
	 * @param step The step this function called.
	 * @param inputs The input values from an environment in the step.
	 * @param nexts The input values form the environemnt in the next step.
	 * @param outputs The output values of CLA in the step.
	 * @param cla A kind of cla agents. It needs to get the internal
	 * data of cla. (ex: cla->getUnits();)
	 */
	void add(
		const Step step,
		const Values& inputs,
		const Values& nexts,
		const Values& outputs,
		const CoreCLA* cla
	) override;

	/**
	 * Save the data on the container.
	 */
	void save() override;

	/**
	 * Close the file where the data is saved.
	 */
	void close() override {}
	
};

} // namespace cla

#endif // SAVE_MODEL_LOG_CALLBACKS_HPP