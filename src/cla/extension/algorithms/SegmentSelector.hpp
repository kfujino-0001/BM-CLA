// SegmentSelector.hpp

/** 
 * @file
 * Definitions for the SegmentSelector class in C++
 */

#ifndef SEGMENT_SELECTOR_HPP
#define SEGMENT_SELECTOR_HPP

#include <iostream>
#include <memory>
#include <unordered_map>
#include "htm/algorithms/Connections.hpp"


namespace htm {

// The number of synapses.
using NumSyns = SynapseIdx;


// prototype definition for cross reference.
class TemporalMemoryExtension;



/**
 * SelectorArgs implementation in C++.
 * 
 * @b Description
 * The selectorArgs is a class for select args. When you add args
 * in the select functions of the selectors, you have to define
 * that in this class.
 * 
 * @param capacity The capacity of selected segments. This value is
 * used on the adaptive selectors.
 * @param threshold The threshold value of selected segments. This
 * value is used on the threshold selectors.
 */
struct SelectorArgs {

	UInt capacity = 0u;
	NumSyns threshold = 0u;

	/**
	 * SelectorArgs constructor.
	 */
	SelectorArgs() = default;

	/**
	 * SelectorArgs constructor with the parameters.
	 */
	SelectorArgs(
		const UInt capacity,
		const SynapseIdx threshold
	);

	/**
	 * SelectorArgs destructor.
	 */
	~SelectorArgs() = default;

};



/**
 * CoreSelector implementation in C++.
 * 
 * @b Description
 * The CoreSelector is an abstract class of segment selectors. This 
 * class defines the basic functions as a selector. This class also
 * has common parameters to all selectors.
 */
class CoreSelector {

private:

	UInt nbCells_ = 1u;
	UInt nbRegions_ = 1u;

	const Connections* connections_;

protected:

	const bool compareSegments_(
		const Segment a, 
		const Segment b
	) const;

	const SegmentData& getSegmentData_(const Segment segment) const;

	const CellIdx getCellIdx_(const Segment segment) const;

public:

	/**
	 * CoreSelector constructor.
	 */
	CoreSelector() = default;

	/**
	 * CoreSelector constructor with the parameters.
	 * 
	 * @param connections The connection pointer for using the compare functions.
	 * @param nbCells The number of cells on the layer.
	 * @param nbRegions The number of regions on the layer.
	 */
	CoreSelector(
		const Connections* connections,
		const UInt nbCells,
		const UInt nbRegions
	);

	/**
	 * CoreSelector destructor.
	 */
	virtual ~CoreSelector() = default;

	/**
	 * Summarize the selector.
	 * 
	 * @param os The output stream (The default is std::cout)
	 */
	virtual void summary(std::ostream& os = std::cout) const;

	/**
	 * Select the segments that will be activated.
	 * 
	 * @param numSynsForSegment The number of synapses on each segment.
	 * @param relations The relations segments and cells.
	 * @param args The arguments of the select.
	 * @param activateSegs The index vector of the selected segment. 
	 * (This is return vector of this function.)
	 */
	virtual void select(
		const std::vector<SynapseIdx>& numSynsForSegment,
		const Relations<Segment, CellIdx>& relations,
		const SelectorArgs& args,
		std::vector<Segment>& activateSegs
	) const = 0;


	/************************************************
	 * getter and setter.
	 ***********************************************/

	/**
	 * Get the number of cells on the layer.
	 * 
	 * @return The number of cells on the layer.
	 */
	const UInt getNbCells() const;

	/**
	 * Get the number of regions on the layer.
	 * 
	 * @return The number of regions on the layer.
	 */
	const UInt getNbRegions() const;
	
	/**
	 * Set the number of the cells.
	 * 
	 * @param nbCells The number of cells on the layer.
	 */
	void setNbCells(const UInt nbCells);

	/**
	 * Set the number of regions on the layer.
	 * 
	 * @param nbRegions The number of regions on the layer.
	 */
	void setNbRegions(const UInt nbRegions);

};

using PSelector = std::shared_ptr<CoreSelector>;

std::ostream& operator<<(std::ostream& os, const CoreSelector& selector);
std::ostream& operator<<(std::ostream& os, const PSelector& pselector);



/**
 * ThresholdSelector implementation in C++.
 * 
 * @b Description
 * The ThresholdSelector is one of the selectors. This class selects
 * segments based on whether the number of synapses in the segment 
 * is greater than or equal to a threshold. This selector is the same 
 * as the conventional CLA selection method.
 */
class ThresholdSelector : public CoreSelector {

public:

	/**
	 * ThresholdSelector constructor.
	 */
	ThresholdSelector() = default;

	/**
	 * CoreSelector constructor with the parameters.
	 * 
	 * @param connections The connection pointer for using the compare functions.
	 * @param nbCells The number of cells on the layer.
	 * @param nbRegions The number of regions on the layer.
	 */
	ThresholdSelector(
		const Connections* connections,
		const UInt nbCells,
		const UInt nbRegions
	);

	/**
	 * ThresholdSelector destructor.
	 */
	~ThresholdSelector() = default;

	/**
	 * Summarize the selector.
	 * 
	 * @param os The output stream (The default is std::cout)
	 */
	void summary(std::ostream& os = std::cout) const override;

	/**
	 * Select the segments that will be activated.
	 * 
	 * @param numSynsForSegment The number of synapses on each segment.
	 * @param relations The relations segments and cells.
	 * @param args The arguments of the select.
	 * @param activateSegs The index vector of the selected segment. 
	 * (This is return vector of this function.)
	 */
	void select(
		const std::vector<SynapseIdx>& numSynsForSegment,
		const Relations<Segment, CellIdx>& relations,
		const SelectorArgs& args,
		std::vector<Segment>& activateSegs
	) const override;

};



/**
 * ToggleThresholdSelector implementation in C++.
 * 
 * @b Description
 * The ToggleThresholdSelector is one of the selectors. This class
 * selects segments based on whether the number of synapses in the segment 
 * is greater than or equal to a threshold. Additionally, it is needed 
 * that the segment has at least one synapse from the inner/outer cell.
 */
class ToggleThresholdSelector : public CoreSelector {

public:

	/**
	 * Trigger mode.
	 * 
	 * At least one synapse is needed to activate a segment, which 
	 * represents whether the cell is inner or outer.
	 */
	enum class Trigger {
		INNER, OUTER
	};

private:

	inline static const std::unordered_map<Trigger, std::string> _triggerMap = {
		{Trigger::INNER, "inner"},
		{Trigger::OUTER, "outer"}
	};

private:

	Trigger trigger_;

private:

	const bool hasInnerCell_(
		const SDR_sparse_t& cells,
		const std::size_t threshold
	) const;

	const bool hasOuterCell_(
		const SDR_sparse_t& cells,
		const std::size_t threshold
	) const;

public:

	/**
	 * ToggleThresholdSelector constructor.
	 */
	ToggleThresholdSelector() = default;
	
	/**
	 * ToggleThresholdSelector constructor with the parameters.
	 * 
	 * @param connections The connection pointer for using the compare functions.
	 * @param nbCells The number of cells on the layer.
	 * @param nbRegions The number of regions on the layer.
	 * @param trigger Trigger which side needed to the activate segment.
	 */
	ToggleThresholdSelector(
		const Connections* connections,
		const UInt nbCells,
		const UInt nbRegions,
		const Trigger trigger
	);

	/**
	 * ToggleThresholdSelector destructor.
	 */
	~ToggleThresholdSelector() = default;

	/**
	 * Summarize the selector.
	 * 
	 * @param os The output stream (The default is std::cout)
	 */
	void summary(std::ostream& os = std::cout) const override;

	/**
	 * Select the segments that will be activated.
	 * 
	 * @param numSynsForSegment The number of synapses on each segment.
	 * @param relations The relations segments and cells.
	 * @param args The arguments of the select.
	 * @param activateSegs The index vector of the selected segment. 
	 * (This is return vector of this function.)
	 */
	void select(
		const std::vector<SynapseIdx>& numSynsForSegment,
		const Relations<Segment, CellIdx>& relations,
		const SelectorArgs& args,
		std::vector<Segment>& activateSegs
	) const override;

};



/**
 * AdaptiveSelector implementation in C++.
 * 
 * @b Description
 * The AdaptiveSelector is one of the selectors. This class selects
 * the top segments based on the number of synapses in the segment.
 * This selector prevents a significant drop in the number of segments 
 * selected.
 */
class AdaptiveSelector : public CoreSelector {

public:

	/**
	 * AdaptiveSelector constructor.
	 */
	AdaptiveSelector() = default;

	/**
	 * AdaptiveSelector constructor with the parameters.
	 * 
	 * @param connections The connection pointer for using the compare functions.
	 * @param nbCells The number of cells on the layer.
	 * @param nbRegions The number of regions on the layer.
	 */
	AdaptiveSelector(
		const Connections* connections,
		const UInt nbCells,
		const UInt nbRegions
	);

	/**
	 * AdaptiveSelector destructor.
	 */
	~AdaptiveSelector() = default;

	/**
	 * Summarize the selector.
	 * 
	 * @param os The output stream (The default is std::cout)
	 */
	void summary(std::ostream& os = std::cout) const override;

	/**
	 * Select the segments that will be activated.
	 * 
	 * @param numSynsForSegment The number of synapses on each segment.
	 * @param relations The relations segments and cells.
	 * @param args The arguments of the select.
	 * @param activateSegs The index vector of the selected segment. 
	 * (This is return vector of this function.)
	 */
	void select(
		const std::vector<SynapseIdx>& numSynsForSegment,
		const Relations<Segment, CellIdx>& relations,
		const SelectorArgs& args,
		std::vector<Segment>& activateSegs
	) const override;

};



/**
 * SeparateAdaptiveSelector implementation in C++.
 * 
 * @b Description
 * The SeparateAdaptiveSelector is one of the selectors. This class 
 * selects the top segments in each region based on the number of
 * synapses in the segment. This selector prevents a significant 
 * drop in the number of segments selected on all regions.
 */
class SeparateAdaptiveSelector : public CoreSelector {

public:

	/**
	 * SeparateAdaptiveSelector constructor.
	 */
	SeparateAdaptiveSelector() = default;

	/**
	 * SeparateAdaptiveSelector constructor with the parameters.
	 * 
	 * @param connections The connection pointer for using the compare functions.
	 * @param nbCells The number of cells on the layer.
	 * @param nbRegions The number of regions on the layer.
	 */
	SeparateAdaptiveSelector(
		const Connections* connections,
		const UInt nbCells,
		const UInt nbRegions
	);

	/**
	 * SeparateAdaptiveSelector destructor.
	 */
	~SeparateAdaptiveSelector() = default;

	/**
	 * Summarize the selector.
	 * 
	 * @param os The output stream (The default is std::cout)
	 */
	void summary(std::ostream& os = std::cout) const override;

	/**
	 * Select the segments that will be activated.
	 * 
	 * @param numSynsForSegment The number of synapses on each segment.
	 * @param relations The relations segments and cells.
	 * @param args The arguments of the select.
	 * @param activateSegs The index vector of the selected segment. 
	 * (This is return vector of this function.)
	 */
	void select(
		const std::vector<SynapseIdx>& numSynsForSegment,
		const Relations<Segment, CellIdx>& relations,
		const SelectorArgs& args,
		std::vector<Segment>& activateSegs
	) const override;

};



/**
 * SegmentSelector implementation in C++.
 * 
 * @b Description
 * This class creates an instance of the selector.
 * 
 * An example of the use.
 * 
 * SelectorParams params;
 * PSelector selector = SegmentSelector<ThresholdSelector>::make(params);
 */
template<typename SelectorType>
struct SegmentSelector {
	template<typename... Args>
	static PSelector make(Args... args) {
		return std::make_shared<SelectorType>(args...);
	}
};



/**
 * Segment selector mode.
 * 
 * The mode of the selector for active segments and matching segments.
 * THRESHOLD: The selector based on the threshold.
 * TOGGLE_THRESHOLD: The toggle selector based on the threshold. 
 * ADAPTIVE: The selector based on the number of connected synapses.
 * SEPARATE_ADAPTIVE: The selector based on the number of connected synapse
 * in each region.
 */
enum class SegmentSelectorMode {
	THRESHOLD,
	INNER_TOGGLE_THRESHOLD,
	OUTER_TOGGLE_THRESHOLD,
	ADAPTIVE,
	SEPARATE_ADAPTIVE
};

using SSMode = SegmentSelectorMode;


/**
 * SegmentSelectors implementation in C++.
 * 
 * @b Description
 * The SegmentSelectors is the definition of modes for some selectors.
 * This class creates selector instances and check modes.
 */
class SegmentSelectors {

private:

	inline static const std::unordered_map<SSMode, std::string> _modeMap = {
		{SSMode::THRESHOLD, "Threshold"},
		{SSMode::INNER_TOGGLE_THRESHOLD, "Inner Toggle Threshold"},
		{SSMode::OUTER_TOGGLE_THRESHOLD, "Outer Toggle Threshold"},
		{SSMode::ADAPTIVE, "Adaptive"},
		{SSMode::SEPARATE_ADAPTIVE, "Separate Adaptive"}
	};

public:

	/**
	 * SegmentSelectors constructor.
	 */
	SegmentSelectors() = default;

	/**
	 * Selectors destructor.
	 */
	~SegmentSelectors() = default;

	/**
	 * Create a selector instance based on the selector mode.
	 * 
	 * @param mode The mode of a selector.
	 * @param params The parameters of the selector.
	 * 
	 * @return The selector pointer.
	 */
	static PSelector createSelector(
		const SSMode& mode,
		const Connections* connections,
		const UInt nbCells,
		const UInt nbRegions
	);

	/**
	 * Returns the name of mode.
	 * 
	 * @param mode The mode of a selector.
	 * 
	 * @return The name of the selector.
	 */
	static const std::string& getName(const SSMode& mode);

	/**
	 * Returns the mode from the name.
	 * 
	 * @param name The name of a selector.
	 * 
	 * @returns The mode of the selector.
	 */
	static const SSMode& getMode(const std::string& name);

	/**
	 * Returns whether the name exists or not.
	 * 
	 * @return The boolean value whether the name exists or not in the modes.
	 */
	static bool isExistName(const std::string& name);

};


} // namespace htm

#endif // SEGMENT_SELECTOR_HPP