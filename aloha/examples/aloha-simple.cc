#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>

#include <ns3/log.h>
#include <ns3/core-module.h>
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/ipv4-routing-helper.h"
#include "ns3/aloha-helper.h"
#include "ns3/net-device-container.h"
#include "ns3/aloha-net_device.h"
#include "ns3/random-variable-stream.h"

using namespace ns3;

void EnqueueTrace (Ptr<OutputStreamWrapper> stream, Mac48Address addr)
{
  *stream->GetStream() << Simulator::Now() << " Enqueued packet at " << addr << std::endl;
}

void AckReceiveTrace (Ptr<OutputStreamWrapper> stream, Mac48Address addr)
{
  *stream->GetStream() << Simulator::Now() << " " << addr << " received ACK"  << std::endl;
}

int main(int argc, char *argv[])
{

  NS_LOG_COMPONENT_DEFINE("aloha-simple");
  Time::SetResolution(Time::US);

  Time simulationTime = Seconds(4);
  uint64_t seed = 0;
  bool verbose = false;

  CommandLine cmd (__FILE__);
  cmd.AddValue ("seed", "turn on log components", seed);
  cmd.AddValue ("verbose", "turn on log components", verbose);
  cmd.AddValue ("simulationTime", "turn on log components", simulationTime);
  cmd.Parse (argc, argv);

  SeedManager::SetRun(seed);

  LogComponentEnable("aloha-simple", (LogLevel)(LOG_LEVEL_INFO | LOG_PREFIX_ALL));
  if (verbose) {
    Packet::EnablePrinting();
    LogComponentEnableAll (LogLevel (LOG_PREFIX_ALL) );
    LogComponentEnable("AlohaNetDevice", (LogLevel)(LOG_LEVEL_WARN));
    LogComponentEnable("AlohaHelper", (LogLevel)(LOG_LEVEL_WARN));
    LogComponentEnable("AlohaMac", (LogLevel)(LOG_LEVEL_INFO));
  }

  NodeContainer nodes;
  nodes.Create(3);

  Ptr<ListPositionAllocator> allocator = CreateObject<ListPositionAllocator>();
  allocator->Add( Vector(0, 0, 0) );
  allocator->Add( Vector(50, 0, 0) );
  allocator->Add( Vector(100, 0, 0) );

  MobilityHelper mobility;
  mobility.SetPositionAllocator(allocator);
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.Install(nodes);

  AlohaHelper aloha;
  aloha.SetChannelAttribute("TransmissionRange", DoubleValue(100.0));
  aloha.SetDeviceAttribute("SinkAddress", Mac48AddressValue("00:00:00:00:00:02"));
  NetDeviceContainer devices = aloha.Install(nodes);
  
  InternetStackHelper internet;
  internet.Install(nodes);

  Ipv4AddressHelper ipAddrs;
  ipAddrs.SetBase ("10.0.0.0", "255.255.255.0");
  auto interfaces = ipAddrs.Assign (devices);

  // the IP address of node 1 (the sink)
  auto sinkAddress = interfaces.GetAddress(1);

  UdpClientHelper client (sinkAddress, 9); 
  client.SetAttribute ("MaxPackets", UintegerValue (10));
  client.SetAttribute ("Interval", TimeValue ( MilliSeconds(100) ));
  client.SetAttribute ("PacketSize", UintegerValue (128));

  auto apps = client.Install ( NodeContainer(nodes.Get(0), nodes.Get(2)) );
  apps.Start (Seconds (1.0));

  Ptr<OutputStreamWrapper> os = Create<OutputStreamWrapper>("output.tr", std::ios::out);
  
  for (auto iter = devices.Begin(); iter != devices.End(); ++iter) {
    Ptr<AlohaNetDevice> alohaDev = DynamicCast<AlohaNetDevice> (*iter); 
    Ptr<AlohaMac> mac = alohaDev->GetMac();
    mac->TraceConnectWithoutContext("EnqueueTx", MakeBoundCallback(&EnqueueTrace, os));
    mac->TraceConnectWithoutContext("AckReceive", MakeBoundCallback(&AckReceiveTrace, os));
  }

  /* RUN SIMULATION */
  Simulator::Stop(simulationTime);
  Simulator::Run(); 

  Simulator::Destroy();

  return 0;
}
