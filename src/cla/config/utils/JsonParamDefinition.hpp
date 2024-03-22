// JsonParamDefinition.hpp

/**
 * @file
 * Definitions for the JsonParamDefinition class in C++
 */

#ifndef JSON_PARAM_DEFINITION_HPP
#define JSON_PARAM_DEFINITION_HPP

#include <string>

namespace cla {

using Label = const std::string;


/**
 * SenderJsonLabels implementation in C++.
 *
 * @b Description
 * SenderJsonLabels defines the labels for the json parameters of the
 * sender module.
 */
struct SenderJsonLabels {

	inline static Label ACTIVE_COLUMN_SENDER_LABEL = "ActiveColumnSender";
	inline static Label BURST_COLUMN_SENDER_LABEL = "BurstColumnSender";
};



/**
 * ReceiverJsonLabels implementation in C++.
 *
 * @b Description
 * ReceiverJsonLabels defines the labels for the json parameters of the
 * receiver module.
 */
struct ReceiverJsonLabels {

	inline static Label ACTIVE_CELL_RECEIVER_LABEL = "ActiveCellReceiver";
	inline static Label VOLATILE_ACTIVE_CELL_RECEIVER_LABEL = "VolatileActCellReceiver";

	inline static Label PARAM_EXTERNAL_PREDICTIVE_INPUTS = "externalPredictiveInputs";
	inline static Label PARAM_VOLATILE_RATE = "volatileRate";
	inline static Label PARAM_VOLATILE_THRESHOLD = "volatileThreshold";
};



/**
 * SpatialPoolerJsonLabels implementation in C++.
 *
 * @b Description
 * SpatialPoolerJsonLabels defines the labels for the json parameters of
 * the spatial pooler module.
 */
struct SpatialPoolerJsonLabels {

	inline static Label HTM_SPATIAL_POOLER_LABEL = "HtmSpatialPooler";

	inline static Label PARAM_INPUT_DIMENSIONS = "inputDimensions";
	inline static Label PARAM_COLUMN_DIMENSIONS = "columnDimensions";
	inline static Label PARAM_POTENTIAL_RADIUS = "potentialRadius";
	inline static Label PARAM_POTENTIAL_PCT = "potentialPct";
	inline static Label PARAM_GLOBAL_INHIBITION = "globalInhibition";
	inline static Label PARAM_LOCAL_AREA_DENSITY = "localAreaDensity";
	inline static Label PARAM_STIMULUS_THRESHOLD = "stimulusThreshold";
	inline static Label PARAM_SYN_PERM_INACTIVE_DEC = "synPermInactiveDec";
	inline static Label PARAM_SYN_PERM_ACTIVE_INC = "symPermActiveInc";
	inline static Label PARAM_SYN_PERM_CONNECTED = "symPermConnected";
	inline static Label PARAM_SYN_INIT_PERMANENCE = "synInitPermanence";
	inline static Label PARAM_MIN_PCT_OVERLAP_DUTY_CYCLES = "minPctOverlapDutyCycles";
	inline static Label PARAM_DUTY_CYCLE_PERIOD = "dutyCyclePeriod";
	inline static Label PARAM_BOOST_STRENGTH = "boostStrength";
	inline static Label PARAM_SEED = "seed";
	inline static Label PARAM_SP_VERBOSITY = "spVerbosity";	
	inline static Label PARAM_WRAP_AROUND = "wrapAround";
	inline static Label PARAM_CONST_SYN_INIT_PERMANENCE = "constSynInitPermanence";
};



/**
 * TemporalMemoryJsonLabels implementation in C++.
 *
 * @b Description
 * TemporalMemoryJsonLabels defines the labels for the json parameters of
 * the temporal memory module.
 */
struct TemporalMemoryJsonLabels {

	inline static Label HTM_TEMPORAL_MEMORY_LABEL = "HtmTemporalMemory";

	inline static Label PARAM_NUM_REGIONS = "numRegions";
	inline static Label PARAM_COLUMN_DIMENSIONS = "columnDimensions";
	inline static Label PARAM_CELLS_PER_COLUMN = "cellsPerColumn";
	inline static Label PARAM_ACTIVATION_THRESHOLD = "activationThreshold";
	inline static Label PARAM_INITIAL_PERMANENCE = "initialPermanence";
	inline static Label PARAM_CONNECTED_PERMANENCE = "connectedPermanence";
	inline static Label PARAM_MIN_THRESHOLD = "minThreshold";
	inline static Label PARAM_MAX_NEW_SYNAPSE_COUNT = "maxNewSynapseCount";
	inline static Label PARAM_PERMANENCE_INCREMENT = "permanenceIncrement";
	inline static Label PARAM_PERMANENCE_DECREMENT = "permanenceDecrement";
	inline static Label PARAM_PREDICTED_SEGMENT_DECREMENT = "predictedSegmentDecrement";
	inline static Label PARAM_SEED = "seed";
	inline static Label PARAM_MAX_SEGMENTS_PER_CELL = "maxSegmentsPerCell";
	inline static Label PARAM_MAX_SYNAPSES_PER_SEGMENT = "maxSynapsesPerSegment";
	inline static Label PARAM_CHECK_INPUTS = "checkInputs";
	inline static Label PARAM_EXCEPTION_HANDLING = "exceptionHandling";
	inline static Label PARAM_EXTERNAL_PREDICTIVE_INPUTS = "externalPredictiveInputs";
	inline static Label PARAM_SYNAPSE_DESTINATION_WEIGHT = "synapseDestinationWeight";
	inline static Label PARAM_CREATE_SYN_WEIGHT = "createSynWeight";
	inline static Label PARAM_DESTROY_SYN_WEIGHT = "destroySynWeight";
	inline static Label PARAM_ACTIVATE_WEIGHT = "activateWeight";
	inline static Label PARAM_CAPACITY_OF_NB_ACTIVE_SEGMENTS = "capacityOfNbActiveSegments";
	inline static Label PARAM_CAPACITY_OF_NB_MATCHING_SEGMENTS = "capacityOfNbMatchingSegments";
	inline static Label PARAM_INNER_SEGMENT_SELECTOR_MODE = "innerSegmentSelectorMode";
	inline static Label PARAM_OUTER_SEGMENT_SELECTOR_MODE = "outerSegmentSelectorMode";
	inline static Label PARAM_ANOMALY_MODE = "anomalyMode";
};



/**
 * AccepterJsonLabels implementation in C++.
 *
 * @b Description
 * AccepterJsonLabels defines the labels for the json parameters of
 * the accepter module.
 */
struct AccepterJsonLabels {

	inline static Label FULL_ACCEPTER_LABEL = "FullAccepter";
	inline static Label INTENSITY_ACCEPTER_LABEL = "IntensityAccepter";

	inline static Label PARAM_INTENSITY_THRESHOLD = "intensityThreshold";
};



/**
 * AdapterJsonLabels implementation in C++.
 *
 * @b Description
 * AdapterJsonLabels defines the labels for the json parameters of
 * the adapter module.
 */
struct AdapterJsonLabels {

	inline static Label DIRECT_ADAPTER_LABEL = "DirectAdapter";
};



/**
 * LayerJsonLabels implementation in C++.
 *
 * @b Description
 * LayerJsonLabels defines the labels for the json parameters of the layer
 * module.
 */
struct LayerJsonLabels {

	inline static Label HTM_LAYER_LABEL = "HtmLayer";

	inline static Label PARAM_INPUT_DIMENSION_LABEL = "inputDimensions";
	inline static Label PARAM_COLUMN_DIMENSION_LABEL = "columnDimensions";
	inline static Label PARAM_NB_CELLS_FOR_COLUMNS_LABEL = "nbCellsForColumns";
	inline static Label PARAM_NB_REGIONS_LABEL = "nbRegions";

	inline static Label ACCEPTER_KEYWORD = "Accepter";
	inline static Label ADAPTER_KEYWORD = "Adapter";
	inline static Label SPATIAL_POOLER_KEYWORD = "SpatialPooler";
	inline static Label TEMPORAL_MEMORY_KEYWORD = "TemporalMemory";
	inline static Label SENDER_KEYWORD = "Sender";
	inline static Label RECEIVER_KEYWORD = "Receiver";
};



/**
 * IoJsonLabels implementation in C++.
 *
 * @b Description
 * IoJsonLabels defines the labels for the json parameters of the io
 * module.
 */
struct IoJsonLabels {

	inline static Label SCALAR_IO_LABEL = "ScalarIO";

	inline static Label PARAM_NB_INPUTS = "nbInputs";
	inline static Label PARAM_INPUT_DIMENSIONS = "dimensions";
	inline static Label PARAM_NB_ACTIVE_BITS = "nbActiveBits";
	inline static Label PARAM_MINS = "mins";
	inline static Label PARAM_MAXS = "maxs";
	inline static Label PARAM_SEED = "seed";
};



/**
 * ModelJsonLabels implementation in C++.
 *
 * @b Description
 * ModelJsonLabels defines the labels for the json parameters of the model.
 */
struct ModelJsonLabels {

	inline static Label MLCLA_MODEL_LABEL = "MLCLA";

	inline static Label LAYER_KEYWORD = "Layer";
	inline static Label IO_KEYWORD = "IO";
};

} // namespace cla

#endif // JSON_PARAM_DEFINITION_HPP