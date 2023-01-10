#include "wireless-mac-upcalls.h"

namespace ns3 {

WirelessMacUpcalls::WirelessMacUpcalls(RxMacCallback rxCallback, CarrierSenseCallback startSenseCallback, CarrierSenseCallback endSenseCallback, FinishTransmitCallback finishTransmitCallback)
:rxCallback(rxCallback), startSenseCallback(startSenseCallback), endSenseCallback(endSenseCallback), finishTransmitCallback(finishTransmitCallback){
}

WirelessMacUpcalls::~WirelessMacUpcalls(){
}

void
WirelessMacUpcalls::Receive(Ptr<Packet> pkt)
{
	NS_ASSERT(rxCallback.GetImpl() != 0);
	rxCallback(pkt);
}

void
WirelessMacUpcalls::StartCarrierSense(void)
{
	NS_ASSERT(startSenseCallback.GetImpl() != 0);
	startSenseCallback();
}

void
WirelessMacUpcalls::EndCarrierSense(void)
{
	NS_ASSERT(endSenseCallback.GetImpl() != 0);
	endSenseCallback();
}

void
WirelessMacUpcalls::FinishTransmit(void)
{
	NS_ASSERT(finishTransmitCallback.GetImpl() != 0);
	finishTransmitCallback();
}

} /* namespace ns3 */