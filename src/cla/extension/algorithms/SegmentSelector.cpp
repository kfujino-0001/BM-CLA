// SegmentSelector.cpp

/** 
 * @file
 * Implementation of SegmentSelector.cpp
 */

#include <algorithm>

#include "cla/extension/algorithms/TemporalMemoryExtension.hpp"
#include "cla/extension/algorithms/SegmentSelector.hpp"

namespace htm {


/************************************************
 * SelectorArgs public functions.
 ***********************************************/

SelectorArgs::SelectorArgs(
	const UInt capacity,
	const SynapseIdx threshold
):
	capacity(capacity),
	threshold(threshold) 
{
	const bool isCapacitySet = (capacity != 0u);
	const bool isThresholdSet = (threshold != 0u);

	// Either element is properly configured.
	NTA_ASSERT(isCapacitySet || isThresholdSet);
}



/************************************************
 * CoreSelector protected functions.
 ***********************************************/

const bool CoreSelector::compareSegments_(
	const Segment a, 
	const Segment b
) const {
	return connections_->compareSegments(a, b);
}

const SegmentData& CoreSelector::getSegmentData_(const Segment segment) const {
	return connections_->dataForSegment(segment);
}

const CellIdx CoreSelector::getCellIdx_(const Segment segment) const {
	return connections_->cellForSegment(segment);
}


/************************************************
 * CoreSelector public functions.
 ***********************************************/

CoreSelector::CoreSelector(
	const Connections* connections,
	const UInt nbCells,
	const UInt nbRegions
):
	connections_(connections),
	nbCells_(nbCells),
	nbRegions_(nbRegions)
{
	NTA_ASSERT(connections_ != nullptr);
	NTA_ASSERT(nbCells > 0u);
	NTA_ASSERT(nbRegions > 0u);
}

void CoreSelector::summary(std::ostream& os) const {
	os << "Segment Selector" << std::endl;
}

const UInt CoreSelector::getNbCells() const {
	return nbCells_;
}

const UInt CoreSelector::getNbRegions() const {
	return nbRegions_;
}

void CoreSelector::setNbCells(const UInt nbCells) {
	nbCells_ = nbCells;
}

void CoreSelector::setNbRegions(const UInt nbRegions) {
	nbRegions_ = nbRegions;
}


/************************************************
 * CoreSelector helper functions.
 ***********************************************/

std::ostream& operator<<(
	std::ostream& os, 
	const CoreSelector& selector
) {
	selector.summary(os);
	return os;
}

std::ostream& operator<<(
	std::ostream& os,
	const PSelector& pselector
) {
	NTA_ASSERT(pselector);
	pselector->summary(os);

	return os;
}



/************************************************
 * ThresholdSelector public functions.
 ***********************************************/

ThresholdSelector::ThresholdSelector(
	const Connections* connections,
	const UInt nbCells,
	const UInt nbRegions
):
	CoreSelector(
		connections,
		nbCells,
		nbRegions
	)
{}

void ThresholdSelector::summary(std::ostream& os) const {
	os << "Threshold Selector" << std::endl;
} 

void ThresholdSelector::select(
	const std::vector<SynapseIdx>& numSynsForSegment,
	const Relations<Segment, CellIdx>& relations,
	const SelectorArgs& args,
	std::vector<Segment>& activateSegs
) const {
	const UInt threshold = args.threshold;

	const auto& compareSegments 
		= [&](const Segment a, const Segment b) { 
			return compareSegments_(a, b); 
		};
	

	activateSegs.clear();

	for (Segment segment = 0, size = static_cast<Segment>(numSynsForSegment.size()); segment < size; segment++) {
		if(numSynsForSegment.at(segment) >= threshold) {
			activateSegs.emplace_back(segment);
		}
	}

	std::sort(activateSegs.begin(), activateSegs.end(), compareSegments);
}



/************************************************
 * ToggleThresholdSelector private functions.
 ***********************************************/

const bool ToggleThresholdSelector::hasInnerCell_(
	const SDR_sparse_t& cells,
	const std::size_t threshold
) const {
	return std::find_if(
		cells.begin(), cells.end(), 
		[&](const CellIdx cell){
			return cell < threshold;
		}
	) != cells.end();
}

const bool ToggleThresholdSelector::hasOuterCell_(
	const SDR_sparse_t& cells,
	const std::size_t threshold
) const {
	return !hasInnerCell_(cells, threshold);
}


/************************************************
 * ToggleThresholdSelector public functions.
 ***********************************************/

ToggleThresholdSelector::ToggleThresholdSelector(
	const Connections* connections,
	const UInt nbCells,
	const UInt nbRegions,
	const Trigger trigger
):
	CoreSelector(
		connections,
		nbCells, 
		nbRegions
	),
	trigger_(trigger)
{}

void ToggleThresholdSelector::summary(std::ostream& os) const {
	os	<< "Toggle Threshold Selector(" 
		<< _triggerMap.at(trigger_)
		<< ")"
		<< std::endl;
}

void ToggleThresholdSelector::select(
	const std::vector<SynapseIdx>& numSynsForSegment,
	const Relations<Segment, CellIdx>& relations,
	const SelectorArgs& args,
	std::vector<Segment>& activateSegs
) const {
	const UInt threshold = args.threshold;
	const std::size_t numCells = getNbCells();

	const auto& compareSegments 
		= [&](const Segment a, const Segment b) { 
			return compareSegments_(a, b); 
		};


	activateSegs.clear();

	for (Segment segment = 0, size = static_cast<Segment>(numSynsForSegment.size()); segment < size; segment++) {
		if(numSynsForSegment.at(segment) < threshold) continue;
		if(relations.count(segment) <= 0) continue;

		const SDR_sparse_t& cells = relations.at(segment);

		if(	((trigger_ == Trigger::INNER) && hasInnerCell_(cells, numCells)) ||
			((trigger_ == Trigger::OUTER) && hasOuterCell_(cells, numCells)))
			activateSegs.emplace_back(segment);
	}

	std::sort(activateSegs.begin(), activateSegs.end(), compareSegments);
}



/************************************************
 * AdaptiveSelector public functions.
 ***********************************************/

AdaptiveSelector::AdaptiveSelector(
	const Connections* connections,
	const UInt nbCells,
	const UInt nbRegions
):
	CoreSelector(
		connections,
		nbCells, 
		nbRegions
	)
{}

void AdaptiveSelector::summary(std::ostream& os) const {
	os << "Adaptive Selector" << std::endl;
}

void AdaptiveSelector::select(
	const std::vector<SynapseIdx>& numSynsForSegment,
	const Relations<Segment, CellIdx>& relations,
	const SelectorArgs& args,
	std::vector<Segment>& activateSegs
) const {
	const UInt capacity = args.capacity;

	const auto& compareSegments 
		= [&](const Segment a, const Segment b) { 
			return compareSegments_(a, b); 
		};

	const auto& compareHeapElems
		= [](const auto& a, const auto& b) {
			return a.second > b.second;
		};


	std::vector<std::pair<Segment, SynapseIdx>> heap;

	// Search for the top capacity-th segments with the highest 
	// number of synapses by using the heap.
	for(Segment segment = 0, size = static_cast<Segment>(numSynsForSegment.size()); segment < size; segment++) {
		const SegmentData& segData = getSegmentData_(segment);
		
		// to avoid including deleted segments
		if(segData.synapses.empty()) continue;

		heap.emplace_back(std::make_pair(segment, numSynsForSegment.at(segment)));
		std::push_heap(heap.begin(), heap.end(), compareHeapElems);
		
		if(heap.size() > static_cast<std::size_t>(capacity)) {
			std::pop_heap(heap.begin(), heap.end(), compareHeapElems);
			heap.pop_back();
		}
	}

	NTA_ASSERT(heap.size() <= static_cast<std::size_t>(capacity));
	activateSegs.clear();

	for(const auto& elem : heap)
		activateSegs.emplace_back(elem.first);

	std::sort(activateSegs.begin(), activateSegs.end(), compareSegments);
}



/************************************************
 * SeparateAdaptiveSelector public functions.
 ***********************************************/

SeparateAdaptiveSelector::SeparateAdaptiveSelector(
	const Connections* connections,
	const UInt nbCells,
	const UInt nbRegions
):
	CoreSelector(
		connections,
		nbCells,
		nbRegions
	)
{}

void SeparateAdaptiveSelector::summary(std::ostream& os) const {
	os << "Separate Adaptive Selector" << std::endl;
}

void SeparateAdaptiveSelector::select(
	const std::vector<SynapseIdx>& numSynsForSegment,
		const Relations<Segment, CellIdx>& relations,
		const SelectorArgs& args,
		std::vector<Segment>& activateSegs
) const {

	const UInt nbRegions = getNbRegions();
	const UInt regionCapacity = args.capacity / nbRegions;
	const CellIdx regionCells = getNbCells() / nbRegions;

	const auto& compareSegments 
		= [&](const Segment a, const Segment b) { 
			return compareSegments_(a, b); 
		};
	
	const auto& compareHeapElems
		= [](const auto& a, const auto& b) {
			return a.second > b.second;
		};

	// Define a heap for each region.
	using Heap = std::vector<std::pair<Segment, SynapseIdx>>;
	std::vector<Heap> heaps(nbRegions);

	// Search for the top capacity-th segments with the highest 
	// number of synapses in each region by using heaps.
	for(Segment segment = 0, size = static_cast<Segment>(numSynsForSegment.size()); segment < size; segment++) {
		const SegmentData& segData = getSegmentData_(segment);
		
		// to avoid including deleted segments
		if(segData.synapses.empty()) continue;

		const CellIdx region = getCellIdx_(segment) / regionCells;
		// access the heap in the region to which the segment corresponds.
		auto& heap = heaps.at(region);
		
		heap.emplace_back(std::make_pair(segment, numSynsForSegment.at(segment)));
		std::push_heap(heap.begin(), heap.end(), compareHeapElems);
		
		if(heap.size() > static_cast<std::size_t>(regionCapacity)) {
			std::pop_heap(heap.begin(), heap.end(), compareHeapElems);
			heap.pop_back();
		}
	}

	activateSegs.clear();

	for(const auto& heap : heaps) {
		NTA_ASSERT(heap.size() <= static_cast<std::size_t>(regionCapacity));
		for(const auto& elem : heap)
			activateSegs.emplace_back(elem.first);
	}

	std::sort(activateSegs.begin(), activateSegs.end(), compareSegments);
}



/************************************************
 * SegmentSelectors static public functions.
 ***********************************************/

PSelector SegmentSelectors::createSelector(
	const SSMode& mode,
	const Connections* connections,
	const UInt nbCells,
	const UInt nbRegions
) {
	switch (mode) {
		case SSMode::THRESHOLD:
			return SegmentSelector<ThresholdSelector>::make(
				connections, nbCells, nbRegions
			);
		break;

		case SSMode::INNER_TOGGLE_THRESHOLD:
			return SegmentSelector<ToggleThresholdSelector>::make(
				connections, nbCells, nbRegions,
				ToggleThresholdSelector::Trigger::INNER
			);
		break;

		case SSMode::OUTER_TOGGLE_THRESHOLD:
			return SegmentSelector<ToggleThresholdSelector>::make(
				connections, nbCells, nbRegions,
				ToggleThresholdSelector::Trigger::OUTER
			);
		break;

		case SSMode::ADAPTIVE:
			return SegmentSelector<AdaptiveSelector>::make(
				connections, nbCells, nbRegions
			);
		break;

		case SSMode::SEPARATE_ADAPTIVE:
			return SegmentSelector<SeparateAdaptiveSelector>::make(
				connections, nbCells, nbRegions
			);
		break;
		
		default:
			NTA_CHECK(false) << "You specified a linker mode that doesn't exist.";
		break;
	}
}

const std::string& SegmentSelectors::getName(const SSMode& mode) {
	NTA_CHECK(_modeMap.find(mode) != _modeMap.end())
		<< "You accessed a mode for which name is not registered.";

	return _modeMap.at(mode);
}

const SSMode& SegmentSelectors::getMode(const std::string& name) {
	NTA_CHECK(isExistName(name))
		<< "You accessed a name for which mode is not registered.";

	const auto cmp = [&](const auto& value){ return value.second == name;};
	return std::find_if(_modeMap.begin(), _modeMap.end(), cmp)->first;
}

bool SegmentSelectors::isExistName(const std::string& name) {
	const auto cmp = [&](const auto& value){ return value.second == name;};
	return std::find_if(_modeMap.begin(), _modeMap.end(), cmp) != _modeMap.end();
}

} // namespace htm