// SaveNumSynsCallback.hpp

/**
 * @file
 * Definitions for the SaveNumSynsCallback class in C++
 */

#ifndef SAVE_NUM_SYNS_CALLBACK_HPP
#define SAVE_NUM_SYNS_CALLBACK_HPP

#include <tuple>
#include <type_traits>

#include "cla/utils/Checker.hpp"
#include "cla/utils/Csv.hpp"

#include "cla/extension/algorithms/TemporalMemoryExtension.hpp"
#include "htm/types/Sdr.hpp"

#include "cla/model/module/callback/SaveCallback.hpp"
#include "cla/model/module/helper/LayerProxy.hpp"

namespace cla {

using NumSyn = htm::UInt;

/**
 * NumSynsData implementation in C++.
 *
 * @b Description
 *
 * @param potentialSyns
 * The potentialSyns is the number of potential synapses.
 *
 * @param connectedSyns
 * The connectedSyns is the number of connected synapses.
 *
 * @param activeSyns
 * The activeSyns is the number of active synapses.
 *
 * @param contributiveSyns
 * The contributiveSyns is the number of contributive synapses.
 */
struct NumSynsData {

	NumSyn potentialSyns = 0u;
	NumSyn connectedSyns = 0u;
	NumSyn activeSyns = 0u;
	NumSyn contributiveSyns = 0u;

	/**
	 * NumSynsData constructor.
	 */
	NumSynsData() = default;

	/**
	 * NumSynsData constructor with the parameters.
	 */
	NumSynsData(
		const NumSyn potentialSyns,
		const NumSyn connectedSyns,
		const NumSyn activeSyns,
		const NumSyn contributiveSyns
	);

	/**
	 * NumSynsData destructor.
	 */
	~NumSynsData() = default;

public:

	friend std::ostream& operator<<(
		std::ostream& os,
		const NumSynsData& rnsd
	);
};

std::ostream& operator<<(std::ostream& os, const NumSynsData& nsd);


using RegionNumSynsData = std::vector<NumSynsData>;
using LayerNumSynsData = std::vector<RegionNumSynsData>;
using Header = csv::Header;



/**
 * LayerData implementation in C++.
 *
 * @b Description
 * LayerData is a structure for holding layer shape.
 *
 * @param numRegions
 * The number of regions in the layer.
 *
 * @param numColumns
 * The number of columns in the layer.
 *
 * @param numCells
 * The number of cells in the layer.
 */
struct LayerData {
	htm::UInt numRegions;
	htm::UInt numColumns;
	htm::UInt numCells;
};


/**
 * LayerLogArgs implementation in C++.
 *
 * @b Description
 * The LayerLogArgs has the arguments for the log. This class is used to
 * absorb processing differences between the upper and middle layers.
 */
struct LayerLogArgs {

	LayerData selfLayerData;
	LayerData upperLayerData;
	bool isExistUpperLayer;

	/**
	 * LayerLogArgs constructor.
	 */
	LayerLogArgs() = default;

	/**
	 * LayerLogArgs constructor with the parameters.
	 *
	 * @param selfLayer The layer proxy corresponding to this layer log
	 * args.
	 */
	LayerLogArgs(const PLayerProxy& selfLayer);

	/**
	 * LayerLogArgs constructor with the parameters.
	 *
	 * @param selfLayer The layer proxy corresponding to this layer log
	 * args.
	 * @param upperLayer The upper layer proxy corresponding to this layer
	 * log args.
	 */
	LayerLogArgs(
		const PLayerProxy& selfLayer,
		const PLayerProxy& upperLayer
	);

	/**
	 * LayerLogArgs destructor.
	 */
	~LayerLogArgs() = default;
};



/************************************************
 * Histories
 ***********************************************/

/**
 * TopNumSynsHistory implementation in C++.
 *
 * @b Description
 * The TopNumSynsHistory is a data structure. It contains the data
 * representing the number of synapses in the top layer. This class saves
 * the data at the json file.
 *
 * @param step
 * The step of the TopNumSynsHistory history.
 * 
 * @param nums
 * The numbers of the synapses of the top layer.
 */
struct TopNumSynsHistory {

	Step step;
	RegionNumSynsData nums;

public:

	static inline const std::string stepHeader = "step";
	static inline const std::string potentialHeader = "_potential";
	static inline const std::string connectedHeader = "_connected";
	static inline const std::string activeHeader = "_active";
	static inline const std::string contributiveHeader = "_contributive";

public:

	/**
	 * TopNumSynsHistory constructor.
	 */
	TopNumSynsHistory() = default;

	/**
	 * TopNumSynsHistory constructor.
	 */
	TopNumSynsHistory(const Step step, const RegionNumSynsData& nums);

	/**
	 * TopNumSynsHistory destructor.
	 */
	~TopNumSynsHistory() = default;

	/**
	 * Get the header of the TopNumSynsHistory.
	 *
	 * @param layerId the id of this layer.
	 * @param numRegions The number of regions.
	 *
	 * @returns The header of the TopNumSynsHistory.
	 */
	static Header getHeader(
		const htm::UInt layerId,
		const htm::UInt numRegions
	);

public:

	friend std::ostream& operator<<(
		std::ostream& os,
		const TopNumSynsHistory& h
	);
};

std::ostream& operator<<(std::ostream& os, const TopNumSynsHistory& h);



/**
 * MiddleNumSynsHistory implementation in C++.
 *
 * @b Description
 * The MiddleNumSynsHistory is a data structure. It contains the data
 * representing the number of synapses in the middle layer. This class saves
 * the data at the json file.
 *
 * @param step
 * The step of the MiddleNumSynsHistory history.
 * 
 * @param selfNums
 * The numbers of the synapses of the middle layer from the self layer.
 * 
 * @param upperNums
 * The numbers of the synapses of the middle layer from the upper layer.
 */
struct MiddleNumSynsHistory {

	Step step;

	RegionNumSynsData selfNums;
	RegionNumSynsData upperNums;

public:

	static inline const std::string stepHeader = "step";
	static inline const std::string potentialHeader = "_potential";
	static inline const std::string connectedHeader = "_connected";
	static inline const std::string activeHeader = "_active";
	static inline const std::string contributiveHeader = "_contributive";

public:

	/**
	 * MiddleNumSynsHistory constructor.
	 */
	MiddleNumSynsHistory() = default;

	/**
	 * MiddleNumSynsHistory constructor.
	 */
	MiddleNumSynsHistory(
		const Step step,
		const RegionNumSynsData& selfNums,
		const RegionNumSynsData& upperNums
	);

	/**
	 * MiddleNumSynsHistory destructor.
	 */
	~MiddleNumSynsHistory() = default;

	/**
	 * Get the header of the MiddleNumSynsHistory.
	 *
	 * @param layerId the id of this layer.
	 * @param numRegions The number of regions.
	 *
	 * @returns The header of the MiddleNumSynsHistory.
	 */
	static Header getHeader(
		const htm::UInt layerId,
		const htm::UInt numRegions
	);

public:

	friend std::ostream& operator<<(
		std::ostream& os,
		const MiddleNumSynsHistory& h
	);
};

std::ostream& operator<<(std::ostream& os, const MiddleNumSynsHistory& h);



/************************************************
 * RegionLogs
 ***********************************************/

/**
 * SaveRegionNumSynsLog implementation in C++.
 *
 * @b Description
 * SaveRegionNumSynsLog is save class for the number of the synapse on the
 * region. This class is responsible for the save process of a single
 * region.
 */
template <typename HistoryType>
class SaveRegionNumSynsLog : public CoreSaver {

private:

	std::string path_;
	std::string filename_;
	Step nbHoldingSteps_;

	htm::UInt layerId_;
	htm::UInt numRegions_;

	std::vector<HistoryType> hists_;
	csv::Csv csv_;

public:

	/**
	 * SaveRegionNumSynsLog constructor.
	 */
	SaveRegionNumSynsLog() = default;

	/**
	 * SaveRegionNumSynsLog constructor with the parameters.
	 *
	 * @param path
	 * @param filename
	 * @param nbHoldingSteps
	 * @param layerId
	 * @param regionId
	 * @param args
	 */
	SaveRegionNumSynsLog(
		const std::string& path,
		const std::string& filename,
		const Step nbHoldingSteps,
		const htm::UInt layerId,
		const htm::UInt regionId,
		const LayerLogArgs& args
	) {
		initialize(
			path, filename, nbHoldingSteps, layerId, regionId, args
		);
	}

	/**
	 * SaveRegionNumSynsLog destructor.
	 */
	~SaveRegionNumSynsLog() = default;

	/**
	 * Initialize SaveRegionNumSynsLog with the parameters.
	 *
	 * @param path
	 * @param filename
	 * @param nbHoldingSteps
	 * @param layerId
	 * @param regionId
	 * @param args
	 */
	void initialize(
		const std::string& path,
		const std::string& filename,
		const Step nbHoldingSteps,
		const htm::UInt layerId,
		const htm::UInt regionId,
		const LayerLogArgs& args
	) {
		CLA_ASSERT(!path.empty());
		CLA_ASSERT(!filename.empty());
		CLA_ASSERT(nbHoldingSteps > 0u);

		path_ = path;
		filename_ = filename;
		nbHoldingSteps_ = nbHoldingSteps;

		layerId_ = layerId;
		numRegions_ = args.selfLayerData.numRegions;

		reset();
	}

	/**
	 * Reset the SaveCallback.
	 */
	void reset() override {
		hists_.clear();
		hists_.reserve(nbHoldingSteps_);
	}

	/**
	 * Open the file where the data will be saved.
	 */
	void open() override {
		createDir_(path_);

		csv_.initialize(path_ + filename_, true);
		csv_.write(
			std::ios::out, HistoryType::getHeader(layerId_, numRegions_)
		);
	}

	/**
	 * Add the data to container.
	 *
	 * @param hist The added history.
	 */
	void add(const HistoryType& hist) {
		hists_.emplace_back(hist);
	}

	/**
	 * Save the data on the container.
	 */
	void save() override {
		csv_.write(std::ios::app, hists_);

		hists_.clear();
		hists_.reserve(nbHoldingSteps_);
	}

	/**
	 * Close the file where the data is saved.
	 */
	void close() override {}
};



/************************************************
 * Analyzer
 ***********************************************/

/**
 * AnalyzerFunc implementation in C++.
 *
 * @b Description
 * AnalyzerFunc is a class of functions commonly used in analyzers. Each
 * function is static-qualified.
 */
class AnalyzerFunc {

public:

	/**
	 * AnalyzerFunc constructor.
	 */
	AnalyzerFunc() = default;

	/**
	 * AnalyzerFunc destructor.
	 */
	~AnalyzerFunc() = default;

	/**
	 * Get the boolean value of whether the input cell is in the upper
	 * layer.
	 *
	 * @param cell The index of the cell.
	 * @param args The args of the layer.
	 *
	 * @return the boolean value of whether the input cell is in the upper
	 * layer.
	 */
	static bool inUpperLayer(
		const htm::CellIdx cell,
		const LayerLogArgs& args
	);

	/**
	 * Convert the cell index to the region index.
	 * 
	 * @param cell The cell index.
	 * @param layerData The layer data corresponding to the layer.
	 * 
	 * @return The region index of the cell index.
	 */
	static const htm::UInt toRegion(
		const htm::CellIdx cell,
		const LayerData& layerData
	);

	/**
	 * Get the relation between cells.
	 * 
	 * @param synapse The synapse index.
	 * @param connections The connection of the layer.
	 * @param args The argument of this layer.
	 * 
	 * @return The relation between cells.
	 */
	static const std::tuple<htm::UInt, htm::UInt, bool> toRelation(
		const htm::Synapse synapse,
		const htm::Connections& connections,
		const LayerLogArgs& args
	);
};



/**
 * AnalyzerInterface implementation in C++.
 *
 * @b Description
 * AnalyzerInterface is an interface class. This interface defines the
 * main function of the Analyzer. The class uses static polymorphism.
 */
template <typename AnalyzerType>
class AnalyzerInterface {

public:

	/**
	 * AnalyzerInterface constructor.
	 */
	AnalyzerInterface() = default;

	/**
	 * AnalyzerInterface destructor.
	 */
	~AnalyzerInterface() = default;

	/**
	 * Initialize Analyzer with the parameters.
	 *
	 * @param args The args of the layer.
	 */
	void initialize(const LayerLogArgs& args) {
		static_cast<AnalyzerType&>(this)->initialize(args);
	}

	/**
	 * Compile synapses from layers to the number data. This function
	 * called in a top layer log.
	 *
	 * @param layer The pointer of the layer.
	 */
	void compile(const PLayerProxy layer) {
		static_cast<AnalyzerType&>(this)->compile(layer);
	}

	/**
	 * Write various data to logs.
	 *
	 * @param step The step this function called.
	 * @param logs logs to be written the various data.
	 */
	template <typename LogType>
	void write(const Step step, std::vector<LogType>& logs) {
		static_cast<AnalyzerType&>(this)->write<LogType>(logs);
	}
};



/**
 * TopLayerAnalyzer implementation in C++.
 *
 * @b Description
 * The TopLayerAnalyzer is the class that analyzes the synapse kinds by status.
 */
class TopLayerAnalyzer : public AnalyzerInterface<TopLayerAnalyzer> {

public:

	using HistoryType = TopNumSynsHistory;

private:

	LayerLogArgs args_;
	LayerNumSynsData database_;

public:

	/**
	 * TopLayerAnalyzer constructor.
	 */
	TopLayerAnalyzer() = default;

	/**
	 * TopLayerAnalyzer constructor with the parameters.
	 *
	 * @param args The args of the layer.
	 */
	TopLayerAnalyzer(const LayerLogArgs& args);

	/**
	 * TopLayerAnalyzer destructor.
	 */
	~TopLayerAnalyzer() = default;

	/**
	 * Initialize Analyzer with the parameters.
	 *
	 * @param args The args of the layer.
	 */
	void initialize(const LayerLogArgs& args);

	/**
	 * Compile synapses from layers to the number data. This function
	 * called in a top layer log.
	 *
	 * @param layer The pointer of the layer.
	 */
	void compile(const PLayerProxy& layer);

	/**
	 * Write various data to logs.
	 *
	 * @param step The step this function called.
	 * @param logs logs to be written the various data.
	 */
	template <typename LogType>
	void write(const Step step, std::vector<LogType>& logs) {
		CLA_ASSERT(logs.size() == args_.selfLayerData.numRegions);

		for(std::size_t i = 0u, size = logs.size(); i < size; ++i) {
			logs.at(i).add({step, database_.at(i)});
		}
	}
};



/**
 * MiddleLayerAnalyzer implementation in C++.
 *
 * @b Description
 * The MiddleLayerAnalyzer is the class that analyzes the synapse kinds by status.
 */
class MiddleLayerAnalyzer : public AnalyzerInterface<MiddleLayerAnalyzer> {

public:

	using HistoryType = MiddleNumSynsHistory;

private:

	LayerLogArgs args_;

	LayerNumSynsData selfLayerDatabase_;
	LayerNumSynsData upperLayerDatabase_;

private:

	const std::pair<htm::UInt, bool> toRegion_(const htm::CellIdx cell
	) const;

public:

	/**
	 * MiddleLayerAnalyzer constructor.
	 */
	MiddleLayerAnalyzer() = default;

	/**
	 * MiddleLayerAnalyzer constructor with the parameters.
	 *
	 * @param args The args of the layer.
	 */
	MiddleLayerAnalyzer(const LayerLogArgs& args);

	/**
	 * MiddleLayerAnalyzer destructor.
	 */
	~MiddleLayerAnalyzer() = default;

	/**
	 * Initialize Analyzer with the parameters.
	 *
	 * @param args The args of the layer.
	 */
	void initialize(const LayerLogArgs& args);

	/**
	 * Compile synapses from layers to the number data. This function
	 * called in a top layer log.
	 *
	 * @param layer The pointer of the layer.
	 */
	void compile(const PLayerProxy& layer);

	/**
	 * Write various data to logs.
	 *
	 * @param step The step this function called.
	 * @param logs logs to be written the various data.
	 */
	template <typename LogType>
	void write(const Step step, std::vector<LogType>& logs) {
		CLA_ASSERT(logs.size() == args_.selfLayerData.numRegions);

		for(std::size_t i = 0u, size = logs.size(); i < size; ++i) {
			logs.at(i).add(
				{step, selfLayerDatabase_.at(i), upperLayerDatabase_.at(i)}
			);
		}
	}
};



/************************************************
 * LayerLogs
 ***********************************************/

/**
 * SaveLayerNumSynsLog implementation in C++.
 *
 * @b Description
 * The SaveLayerNumSynsLog is save class for the number of layer synapses.
 * This class is responsible for the save process of a single layer.
 */
template <
	typename AnalyzerType,
	typename HistoryType = typename AnalyzerType::HistoryType,
	bool isExtended = std::
		is_base_of<AnalyzerInterface<AnalyzerType>, AnalyzerType>::value>
class SaveLayerNumSynsLog : public CoreSaver {
	static_assert(
		isExtended,
		"AnalyzerType is not extended interface class"
	);
};


template <typename AnalyzerType, typename HistoryType>
class SaveLayerNumSynsLog<AnalyzerType, HistoryType, true> :
	public CoreSaver {

private:

	AnalyzerType analyzer_;
	std::vector<SaveRegionNumSynsLog<HistoryType>> logs_;

public:

	/**
	 * SaveLayerNumSynsLog constructor.
	 */
	SaveLayerNumSynsLog() = default;

	/**
	 * SaveLayerNumSynsLog constructor with the parameters.
	 *
	 * @param path The file path where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in the
	 * real memory. Adjusting this value reduces the time required to
	 * access io and speeds up the process.
	 * @param layerId The id of the layer.
	 * @param args The args of the layer.
	 */
	SaveLayerNumSynsLog(
		const std::string& path,
		const Step nbHoldingSteps,
		const htm::UInt layerId,
		const LayerLogArgs& args
	) {
		initialize(path, nbHoldingSteps, layerId, args);
	}

	/**
	 * SaveLayerNumSynsLog destructor.
	 */
	~SaveLayerNumSynsLog() = default;

	/**
	 * Initialize SaveLayerNumSynsLog with the parameters.
	 *
	 * @param path The file path where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in the
	 * real memory. Adjusting this value reduces the time required to
	 * access io and speeds up the process.
	 * @param layerId The id of the layer.
	 * @param args The args of the layer.
	 */
	void initialize(
		const std::string& path,
		const Step nbHoldingSteps,
		const htm::UInt layerId,
		const LayerLogArgs& args
	) {
		const htm::UInt numRegions = args.selfLayerData.numRegions;

		logs_.clear();
		logs_.resize(numRegions);
		for(std::size_t i = 0u; i < numRegions; i++) {
			logs_.at(i).initialize(
				path + "numSyns\\",
				"numSyns_L" + std::to_string(layerId) + "R"
					+ std::to_string(i) + ".csv",
				nbHoldingSteps, layerId, static_cast<htm::UInt>(i), args
			);
		}

		analyzer_.initialize(args);

		reset();
	}

	/**
	 * Reset the SaveCallback.
	 */
	void reset() override {
		for(auto&& log : logs_)
			log.reset();
	}

	/**
	 * Open the file where the data will be saved.
	 */
	void open() override {
		for(auto&& log : logs_)
			log.open();
	}

	/**
	 * Add the data to container.
	 *
	 * @param step The step this function called.
	 * @param layer The pointer of the layer. It needs to get
	 * the internal layer data.
	 */
	void add(const Step step, const PLayerProxy layer) {
		analyzer_.compile(layer);
		analyzer_.write(step, logs_);
	}

	/**
	 * Save the data on the container.
	 */
	void save() override {
		for(auto&& log : logs_)
			log.save();
	}

	/**
	 * Close the file where the data is saved.
	 */
	void close() override {
		for(auto&& log : logs_)
			log.close();
	}
};



/**
 * SaveNumSynsCallback implementation in C++.
 *
 * @b Description
 * SaveNumSynsCallback is one of the Callback-series. This calls saves
 * data of the number of synapses for each region.
 */
class SaveNumSynsCallback : public SaveCallback {

private:

	SaveLayerNumSynsLog<TopLayerAnalyzer> topLayerProxyLog_;
	std::vector<SaveLayerNumSynsLog<MiddleLayerAnalyzer>> middleLayerLogs_;

public:

	/**
	 * SaveNumSynsCallback constructor.
	 */
	SaveNumSynsCallback() = default;

	/**
	 * SaveNumSynsCallback constructor with the parameters.
	 *
	 * @param path The file path where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in the
	 * real memory. Adjusting this value reduces the time required to
	 * access io and speeds up the process.
	 */
	SaveNumSynsCallback(
		const std::string& path,
		const Step nbHoldingSteps
	);

	/**
	 * SaveNumSynsCallback destructor.
	 */
	~SaveNumSynsCallback() = default;

	/**
	 * Initialize SaveSynapseLogCallback with the parameters.
	 *
	 * @param path The file path where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in the
	 * real memory. Adjusting this value reduces the time required to
	 * access io and speeds up the process.
	 */
	void initialize(const std::string& path, const Step nbHoldingSteps);

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
	void close() override;

	/**
	 * Called the start of the processing.
	 *
	 * @param cla A kind of cla agents. It needs to get the internal
	 * data of cla. (ex: cla->getUnits();)
	 */
	void doStartProcessing(const CoreCLA* cla) override;
};


} // namespace cla

#endif // SAVE_NUM_SYNS_CALLBACK_HPP