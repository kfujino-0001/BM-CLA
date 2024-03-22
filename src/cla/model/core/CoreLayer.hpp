// CoreLayer.hpp

/**
 * @file
 * Definitions for the CoreLayer class in C++
 */

#ifndef CORE_LAYER_HPP
#define CORE_LAYER_HPP

#include <vector>

#include "htm/types/Sdr.hpp"
#include "cla/model/core/CoreAccepter.hpp"
#include "cla/model/core/CoreAdapter.hpp"
#include "cla/model/core/CoreLayer.hpp"
#include "cla/model/core/CoreSpatialPooler.hpp"
#include "cla/model/core/CoreTemporalMemory.hpp"
#include "cla/model/core/CoreSender.hpp"
#include "cla/model/core/CoreReceiver.hpp"
#include "cla/model/module/helper/SDRContainer.hpp"
#include "cla/model/module/helper/LayerProxy.hpp"
#include "cla/utils/Status.hpp"


namespace cla {

/**
 * CoreLayer implementation in C++.
 *
 * @b Description
 * The CoreLayer is the base (interface) class for layers. This class
 * defines additional functions for layers.
 */
class CoreLayer {

public:

	/**
	 * CoreLayer constructor.
	 */
	CoreLayer() = default;

	/**
	 * CoreLayer destructor.
	 */
	virtual ~CoreLayer() = default;

	/**
	 * reset the internal state of the layer. The function is assumed to be
	 * executed at every time step.
	 */
	virtual void restate() = 0;

	/**
	 * Reset internal state of the layer.
	 */
	virtual void reset() = 0;

	/**
	 * Summarize the layer model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	virtual void summary(std::ostream& os = std::cout) const = 0;

	/**
	 * Show the detail of the layer model.
	 * 
	 * @param os The output stream. The default os is std::cout.
	 */
	virtual void detail(std::ostream& os = std::cout) const = 0;

	/**
	 * Forward the input sdr.
	 * This function transfers the sdr of the input values to the columns
	 * of the layer predictive and selects the cells that will be active
	 * and winner. If learning is enable, it adjusts the internal synapse
	 * placement and permanence.
	 *
	 * @param inputSDR The input sdr from the lower layer.
	 * @param learn The boolean value whether or not learning is enabled.
	 * @param activeSDR The active SDR for the upper layer. (This param has
	 * a return value.)
	 * 
	 * @return The boolean value whether the upper layer should run.
	 */
	virtual const bool forward(
		const htm::SDR& inputSDR,
		const bool learn,
		htm::SDR& activeSDR
	) = 0;

	/**
	 * Backward
	 * This process updates the internal state based on the information in
	 * the upper layers and the information in the same layer.
	 *
	 * @param learn The boolean value whether or not learning is enabled.
	 * 
	 * @returns The proxy of this layer. The lower layer can
	 * select the sdr to compute the predictive cell from this proxy. (This param has
	 * a return value.)
	 */
	virtual const PLayerProxy& backward(
		const bool learn
	) = 0;

	/**
	 * Backward
	 * This process updates the internal state based on the information in
	 * the upper layers and the information in the same layer.
	 *
	 * @param upperLayer The proxy of the upper layer.
	 * @param learn The boolean value whether or not learning is enabled.
	 * 
	 * @returns The proxy of this layer. The lower layer can
	 * select the sdr to compute the predictive cell from this proxy. (This param has
	 * a return value.)
	 */
	virtual const PLayerProxy& backward(
		const PLayerProxy& upperLayer,
		const bool learn
	) = 0;


	/************************************************
	 * The module getters.
	 ***********************************************/

	/**
	 * Get the input accepter on this layer.
	 *
	 * @return const PAccepter& The pointer of accepter.
	 */
	virtual const PAccepter& getAccepter() const = 0;

	/**
	 * Get the input adapter on this layer.
	 * 
	 * @return const PAccepter& The pointer of adapter.
	 */
	virtual const PAdapter& getAdapter() const = 0;

	/**
	 * Get the SpatialPoolers on this layer.
	 *
	 * @return const std::vector<PSpatialPooler>& The pointer of SPs.
	 */
	virtual const std::vector<PSpatialPooler>& getSPs() const = 0;

	/**
	 * Get the TemporalMemory on this layer.
	 *
	 * @return const PTemporalMemory& The pointer of TM.
	 */
	virtual const PTemporalMemory& getTM() const = 0;

	/**
	 * Get the active sdr sender on this layer.
	 *
	 * @return const PSender& The pointer of sender.
	 */
	virtual const PSender& getSender() const = 0;
	
	/**
	 * Get the active sdrs receiver on this layer from the upper layer.
	 * 
	 * @return const PReceiver& 
	 */
	virtual const PReceiver& getReceiver() const = 0;

	/**
	 * Get the proxy of this layer.
	 *
	 * @return const LayerProxy& The proxy of this layer.
	 */
	virtual const PLayerProxy& getLayerProxy() const = 0;

	/**
	 * Get the status of this layer.
	 * 
	 * @return const Status& The status.
	 */
	virtual const Status& getStatus() const = 0;

	/**
	 * Get the input dimensions.
	 * 
	 * @return const std::vector<htm::UInt> The input dimensions.
	 */
	virtual const std::vector<htm::UInt>& getInputDimensions() const = 0;

	/**
	 * Get tht columns dimensions.
	 * 
	 * @return const std::vector<htm::UInt> The columns dimensions. 
	 */
	virtual const std::vector<htm::UInt>& getColumnDimensions() const = 0;

	/**
	 * Get the cell dimensions.
	 * 
	 * @return const std::vector<htm::UInt> The cell dimensions.
	 */
	virtual const std::vector<htm::UInt>& getCellDimensions() const = 0;
};

using PLayer = std::unique_ptr<CoreLayer>;


} // namespace cla

#endif // CORE_LAYER_HPP