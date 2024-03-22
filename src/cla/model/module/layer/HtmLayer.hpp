// HtmLayer.hpp

/**
 * @file
 * Definitions for the HtmLayer class in C++
 */

#ifndef HTM_LAYER_HPP
#define HTM_LAYER_HPP

#include <vector>

#include "cla/model/core/CoreLayer.hpp"

namespace cla {

/**
 * HtmLayer implementation in C++.
 *
 * @b Description
 * The HtmLayer is extended class of CoreLayer. HtmLayer is a class that
 * implements general HTM layer behavior. It is basically composed of a
 * spatial pooling module, a temporal memory module, and a set of modules
 * for communicating with the upper and lower layers.
 */
class HtmLayer : public CoreLayer {

private:

	Status status_;
	std::vector<htm::UInt> inputDimensions_;
	std::vector<htm::UInt> columnDimensions_;
	std::vector<htm::UInt> cellDimensions_;
	htm::UInt nbCellsForColumn_;
	htm::UInt nbRegions_;

	PAccepter accepter_;
	PAdapter adapter_;
	PSender sender_;
	PReceiver receiver_;

	std::vector<PSpatialPooler> sps_;
	PTemporalMemory tm_;

	SDRContainer container_;
	PLayerProxy proxy_;

public:

	/**
	 * HtmLayer constructor.
	 */
	HtmLayer() = default;

	/**
	 * HtmLayer constructor with the parameters.
	 *
	 * @param inputDimensions The input dimensions of this layer.
	 * @param columnDimensions The column dimensions of this layer.
	 * @param nbCells The number of the cells.
	 * @param nbRegions The number of the regions.
	 * @param accepter The pointer of the accepter module.
	 * @param adapter The pointer of the adapter module.
	 * @param sender The pointer of the sender module.
	 * @param receiver The pointer of the receiver module.
	 * @param sps The pointers of the spatial pooler module.
	 * @param tm The pointer of the temporal memory module.
	 */
	HtmLayer(
		const std::vector<htm::UInt>& inputDimensions,
		const std::vector<htm::UInt>& columnDimensions,
		const htm::UInt nbCellsForColumn,
		const htm::UInt nbRegions,
		PAccepter accepter,
		PAdapter adapter,
		PSender sender,
		PReceiver receiver,
		const std::vector<PSpatialPooler>& sps,
		PTemporalMemory tm
	);

	/**
	 * HtmLayer destructor.
	 */
	~HtmLayer() = default;

	/**
	 * reset the internal state of the layer. The function is assumed to be
	 * executed at every time step.
	 */
	void restate() override;

	/**
	 * Reset internal state of the layer.
	 */
	void reset() override;

	/**
	 * Summarize the layer model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	void summary(std::ostream& os = std::cout) const override;

	/**
	 * Show the detail of the layer model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	void detail(std::ostream& os = std::cout) const override;

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
	const bool forward(
		const htm::SDR& inputSDR,
		const bool learn,
		htm::SDR& activeSDR
	) override;

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
	const PLayerProxy& backward(
		const bool learn
	) override;

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
	const PLayerProxy& backward(
		const PLayerProxy& upperLayer,
		const bool learn
	) override;


	/************************************************
	 * The module getters.
	 ***********************************************/

	/**
	 * Get the input accepter on this layer.
	 *
	 * @return const PAccepter& The pointer of accepter.
	 */
	const PAccepter& getAccepter() const override;

	/**
	 * Get the input adapter on this layer.
	 *
	 * @return const PAdapter& The pointer of adapter.
	 */
	const PAdapter& getAdapter() const override;

	/**
	 * Get the SpatialPoolers on this layer.
	 *
	 * @return const std::vector<PSpatialPooler>& The pointer of SPs.
	 */
	const std::vector<PSpatialPooler>& getSPs() const override;

	/**
	 * Get the TemporalMemory on this layer.
	 *
	 * @return const PTemporalMemory& The pointer of TM.
	 */
	const PTemporalMemory& getTM() const override;

	/**
	 * Get the active sdr sender on this layer.
	 *
	 * @return const PSender& The pointer of sender.
	 */
	const PSender& getSender() const override;

	/**
	 * Get the active sdrs receiver on this layer from the upper layer.
	 *
	 * @return const PReceiver&
	 */
	const PReceiver& getReceiver() const override;

	/**
	 * Get the proxy of this layer.
	 *
	 * @return const LayerProxy& The proxy of this layer.
	 */
	const PLayerProxy& getLayerProxy() const override;

	/**
	 * Get the status of this layer.
	 *
	 * @return const Status& The status.
	 */
	const Status& getStatus() const override;

	/**
	 * Get the input dimensions.
	 *
	 * @return const std::vector<htm::UInt> The input dimensions.
	 */
	const std::vector<htm::UInt>& getInputDimensions() const override;

	/**
	 * Get tht columns dimensions.
	 *
	 * @return const std::vector<htm::UInt> The columns dimensions.
	 */
	const std::vector<htm::UInt>& getColumnDimensions() const override;

	/**
	 * Get the cell dimensions.
	 *
	 * @return const std::vector<htm::UInt> The cell dimensions.
	 */
	const std::vector<htm::UInt>& getCellDimensions() const override;
};

} // namespace cla

#endif // HTM_LAYER_HPP