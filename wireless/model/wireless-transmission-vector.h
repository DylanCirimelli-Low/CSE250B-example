#ifndef WIRELESS_TRANSMISSION_VECTOR_H
#define WIRELESS_TRANSMISSION_VECTOR_H

#include <ns3/simple-ref-count.h>
#include <ns3/net-device.h>
#include "ns3/nstime.h"
#include "ns3/mobility-model.h"
#include "ns3/log.h"

namespace ns3 {

class TransmissionVector : public SimpleRefCount<TransmissionVector>
{
public:
    
    TransmissionVector(
        Ptr<Packet> pkt,
        Ptr<NetDevice> device,
        Ptr<MobilityModel> m_mobility
        );
    
    virtual ~TransmissionVector();

    Ptr<Packet> GetPacket (void) const;
    Ptr<NetDevice> GetDevice (void) const;
    Ptr<MobilityModel> GetMobility (void) const;
    Time GetDuration (void) const;


private:
    Ptr<Packet> m_pkt;
    Ptr<NetDevice> m_device;
    Ptr<MobilityModel> m_mobility;
};

} /* namespace ns3 */

#endif /* WIRELESS_TRANSMISSION_VECTOR_H */