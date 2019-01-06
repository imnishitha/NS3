#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");
int main (int argc, char *argv[])
{
Time::SetResolution (Time::NS);
LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
NodeContainer nodes;
nodes.Create(3);
PointToPointHelper pointToPoint;
pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
NetDeviceContainer devices, devices1;
devices = pointToPoint.Install (nodes.Get(0),nodes.Get(1));
devices1 = pointToPoint.Install (nodes.Get(2),nodes.Get(1));
InternetStackHelper stack;
stack.Install (nodes);
Ipv4AddressHelper address;
address.SetBase ("10.1.1.0", "255.255.255.0");
Ipv4InterfaceContainer interfaces = address.Assign (devices);
Ipv4InterfaceContainer interfaces1 = address.Assign (devices1);
UdpEchoServerHelper echoServer (90);
ApplicationContainer serverApps = echoServer.Install (nodes.Get(1));
serverApps.Start (Seconds(1.0));
serverApps.Stop (Seconds(30.0));
UdpEchoClientHelper echoClient (interfaces.GetAddress (1),90);
echoClient.SetAttribute("MaxPackets", UintegerValue (1));
echoClient.SetAttribute("Interval", TimeValue (Seconds (1.0)));
echoClient.SetAttribute("PacketSize", UintegerValue (1024));
ApplicationContainer clientApps = echoClient.Install (nodes.Get(0));
clientApps.Start (Seconds (2.0));
clientApps.Stop (Seconds (10.0));
UdpEchoClientHelper echoClient1 (interfaces1.GetAddress(1), 90);
echoClient1.SetAttribute("MaxPackets", UintegerValue(1));
echoClient1.SetAttribute("Interval", TimeValue (Seconds(1.0)));
echoClient1.SetAttribute("PacketSize", UintegerValue(1024));
ApplicationContainer clientApps1 = echoClient1.Install(nodes.Get (2));
clientApps1.Start(Seconds(11.0));
clientApps1.Stop (Seconds(20.0));
Simulator::Run ();
Simulator::Destroy ();
return 0;
}
