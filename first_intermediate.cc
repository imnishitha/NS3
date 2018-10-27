//Add an intermediate node between the server and client
/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
 /*
  * This is the architecture
  * n0-----n1-----n2
  *    L1      L2
  * L1 -> 5Mbps, 2ms
  * L2 -> 10Mbps, 5ms
  * n0 - client, n2 - server
  * 
  */

#include "ns3/netanim-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
	Time::SetResolution (Time::NS);
	LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
	LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

	NodeContainer nodes;
	nodes.Create (3);
	
	InternetStackHelper stack;
	stack.Install (nodes);
	
	PointToPointHelper p2p1;
	p2p1.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
	p2p1.SetChannelAttribute ("Delay", StringValue ("1ms"));

	PointToPointHelper p2p2;
	p2p2.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
	p2p2.SetChannelAttribute ("Delay", StringValue ("5ms"));

	Ipv4AddressHelper address;
	address.SetBase ("10.1.1.0", "255.255.255.0"); 
	NetDeviceContainer devices;
	devices = p2p1.Install (nodes.Get (0), nodes.Get (1));
	Ipv4InterfaceContainer interfaces = address.Assign (devices);
	
	devices = p2p2.Install (nodes.Get (1), nodes.Get (2));
	address.SetBase ("10.1.2.0", "255.255.255.0"); 
	interfaces = address.Assign (devices);
	
	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  
	UdpEchoServerHelper echoServer (9);

	ApplicationContainer serverApps = echoServer.Install (nodes.Get (2));
	serverApps.Start (Seconds (1.0));
	serverApps.Stop (Seconds (10.0));

	UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
	echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
	echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
	echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

	ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
	clientApps.Start (Seconds (2.0));
	clientApps.Stop (Seconds (10.0));

	AnimationInterface anim ("my_example_1.xml");
	anim.SetConstantPosition (nodes.Get(0), 0.0, 0.0);
    anim.SetConstantPosition (nodes.Get(1), 2.0, 2.0);
    anim.SetConstantPosition (nodes.Get(2), 3.0, 3.0);

	Simulator::Run ();
	Simulator::Destroy ();
	
	return 0;
}
