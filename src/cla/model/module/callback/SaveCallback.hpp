// SaveCallback.hpp

/** 
 * @file
 * Definitions for the SaveCallback class in C++
 */

#ifndef SAVE_CALLBACK_HPP
#define SAVE_CALLBACK_HPP

#include <string>

#include "cla/model/core/CoreCallback.hpp"

namespace cla {

/**
 * CoreSaver implementation in C++.
 * 
 * @b Description
 * The CoreSaver is a class that defines functions necessary for Callback 
 * to save data and functions that support the saving process.
 */
class CoreSaver {

protected:

	/**
	 * Create dir according to the path.
	 * 
	 * @param path The path specifying the dir to be created.
	 */
	void createDir_(const std::string& path) const;

public:

	/**
	 * CoreSaver constructor.
	 */
	CoreSaver() = default;

	/**
	 * CoreSaver destructor.
	 */
	virtual ~CoreSaver() = default;

	/**
	 * Reset the SaveCallback.
	 */
	virtual void reset() = 0;

	/**
	 * Open the file where the data will be saved.
	 */
	virtual void open() = 0;

	/**
	 * Save the data on the container.
	 */
	virtual void save() = 0;

	/**
	 * Close the file where the data is saved.
	 */
	virtual void close() = 0;

};



/**
 * SaveCallback implementation in C++.
 * 
 * @b Description
 * SaveCallback is one of the Callback-series. This class is an abstract 
 * class that can be used to define a Callback that saves CLA information.
 */
class SaveCallback : 
	public CoreCallback,
	public CoreSaver
{

private:

	std::string path_;
	std::string filename_;
	Step nbHoldingSteps_;

public:

	/**
	 * SaveCallback constructor.
	 */
	SaveCallback() = default;

	/**
	 * SaveCallback constructor with the parameters.
	 * 
	 * @param path The file path where the data will be saved.
	 * @param filename The filename where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in
	 * the real memory. Adjusting this value reduces the time required
	 * to access io and speeds up the process.
	 */
	SaveCallback(
		const std::string& path,
		const std::string & filename,
		const Step nbHoldingSteps
	);

	/**
	 * SaveCallback destructor.
	 */
	virtual ~SaveCallback() = default;

	/**
	 * Initialize the SaveCallback constructor the parameters.
	 * 
	 * @param path The file path where the data will be saved.
	 * @param filename The filename where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in
	 * the real memory. Adjusting this value reduces the time required
	 * to access io and speeds up the process.
	 */
	void initialize(
		const std::string& path,
		const std::string & filename,
		const Step nbHoldingSteps
	);

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
	virtual void add(
		const Step step,
		const Values& inputs,
		const Values& nexts,
		const Values& outputs,
		const CoreCLA* cla
	) = 0;

	/**
	 * Called the start of the processing.
	 * 
	 * @param cla A kind of cla agents. It needs to get the internal
	 * data of cla. (ex: cla->getUnits();)
	 */
	void doStartProcessing(const CoreCLA* cla) override;

	/**
	 * Called before beginning processing of a step.
	 * 
	 * @param step The step this function called.
	 * @param inputs The input values from an environment in the step.
	 * @param cla A kind of cla agents. It needs to get the internal
	 * data of cla. (ex: cla->getUnits();)
	 */
	void doPreProcessing(
		const Step step,
		const Values& inputs,
		const CoreCLA* cla
	) override {}

	/**
	 * Called after beginning processing of a step.
	 * 
	 * @param step The step this function called.
	 * @param inputs The input values from an environment in the step.
	 * @param nexts The input values form the environemnt in the next step.
	 * @param outputs The output values of CLA in the step.
	 * @param cla A kind of cla agents. It needs to get the internal
	 * data of cla. (ex: cla->getUnits();)
	 */
	void doPostProcessing(
		const Step step,
		const Values& inputs,
		const Values& nexts,
		const Values& outputs,
		const CoreCLA* cla
	) override;

	/**
	 * Called the end of the processing.
	 * 
	 * @param cla A kind of cla agents. It needs to get the internal
	 * data of cla. (ex: cla->getUnits();)
	 */
	void doEndProcessing(const CoreCLA* cla) override;

	/**
	 * Set the path where data will be saved.
	 * 
	 * @param path The string value that represents the file path.
	 */
	void setPath(const std::string& path) {
		path_ = path;
	}

	/**
	 * Set the filename at which data will be saved.
	 * 
	 * @param filename The string value that represents the file name.
	 */
	void setFilename(const std::string& filename) {
		filename_ = filename;
	}

	/**
	 * Set the number of holding steps.
	 * 
	 * @param nbHoldingSteps The step value that represents holding steps.
	 */
	void setNbHoldingSteps(const Step nbHoldingSteps) {
		nbHoldingSteps_ = nbHoldingSteps;
	}

	/**
	 * Get the path where data will be saved.
	 * 
	 * @return The string value that represents the file path.
	 */
	const std::string& getPath() const {
		return path_;
	}

	/**
	 * Get the filename at which data will be saved.
	 * 
	 * @return The string value that represents the file name.
	 */
	const std::string& getFilename() const {
		return filename_;
	}

	/**
	 * Get the number of holding steps.
	 * 
	 * @return The step value that represents the holding steps.
	 */
	const Step getNbHoldingSteps() const {
		return nbHoldingSteps_;
	}

};

} // namespace cla

#endif // SAVE_CALLBACK_HPP