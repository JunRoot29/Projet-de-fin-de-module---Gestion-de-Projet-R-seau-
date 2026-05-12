#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"
#include <fstream>
#include <iomanip>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ComparaisonTcpFinal");

int main (int argc, char *argv[])
{
    CommandLine cmd;
    cmd.Parse (argc, argv);

    // 1. Création des nœuds
    NodeContainer nNR, nCB, nBR;
    nNR.Create (2); nCB.Create (2); nBR.Create (2);

    // 2. Configuration du lien
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute ("DataRate", StringValue ("2Mbps"));
    p2p.SetChannelAttribute ("Delay", StringValue ("100ms"));
    p2p.SetQueue ("ns3::DropTailQueue", "MaxSize", StringValue ("15p"));

    NetDeviceContainer dNR = p2p.Install (nNR);
    NetDeviceContainer dCB = p2p.Install (nCB);
    NetDeviceContainer dBR = p2p.Install (nBR);

    // 3. Pile Internet
    InternetStackHelper stack;
    stack.InstallAll ();

    // 4. IPs
    Ipv4AddressHelper address;
    Ipv4InterfaceContainer iNR, iCB, iBR;
    address.SetBase ("10.1.1.0", "255.255.255.0"); iNR = address.Assign (dNR);
    address.SetBase ("10.1.2.0", "255.255.255.0"); iCB = address.Assign (dCB);
    address.SetBase ("10.1.3.0", "255.255.255.0"); iBR = address.Assign (dBR);

    // 5. CONFIGURATION DES VARIANTES TCP
    Config::Set ("/NodeList/0/$ns3::TcpL4Protocol/SocketType", TypeIdValue (TcpNewReno::GetTypeId ()));
    Config::Set ("/NodeList/2/$ns3::TcpL4Protocol/SocketType", TypeIdValue (TcpCubic::GetTypeId ()));
    Config::Set ("/NodeList/4/$ns3::TcpL4Protocol/SocketType", TypeIdValue (TcpBbr::GetTypeId ()));

    uint16_t port = 8080;

    // 6. Applications OnOff
    // NewReno
    OnOffHelper onoffNR ("ns3::TcpSocketFactory", InetSocketAddress (iNR.GetAddress (1), port));
    onoffNR.SetAttribute ("DataRate", StringValue ("10Mbps"));
    onoffNR.SetAttribute ("PacketSize", UintegerValue (1024));
    ApplicationContainer sourceNR = onoffNR.Install (nNR.Get (0));
    sourceNR.Start (Seconds (1.0)); sourceNR.Stop (Seconds (15.0));

    // Cubic
    OnOffHelper onoffCB ("ns3::TcpSocketFactory", InetSocketAddress (iCB.GetAddress (1), port));
    onoffCB.SetAttribute ("DataRate", StringValue ("10Mbps"));
    onoffCB.SetAttribute ("PacketSize", UintegerValue (1024));
    ApplicationContainer sourceCB = onoffCB.Install (nCB.Get (0));
    sourceCB.Start (Seconds (1.0)); sourceCB.Stop (Seconds (15.0));

    // BBR
    OnOffHelper onoffBR ("ns3::TcpSocketFactory", InetSocketAddress (iBR.GetAddress (1), port));
    onoffBR.SetAttribute ("DataRate", StringValue ("10Mbps"));
    onoffBR.SetAttribute ("PacketSize", UintegerValue (1024));
    ApplicationContainer sourceBR = onoffBR.Install (nBR.Get (0));
    sourceBR.Start (Seconds (1.0)); sourceBR.Stop (Seconds (15.0));

    // 7. Récepteurs
    PacketSinkHelper sink ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));
    
    ApplicationContainer sinkNR = sink.Install (nNR.Get (1));
    sinkNR.Start (Seconds (0.0));
    sinkNR.Stop (Seconds (15.0));
    
    ApplicationContainer sinkCB = sink.Install (nCB.Get (1));
    sinkCB.Start (Seconds (0.0));
    sinkCB.Stop (Seconds (15.0));
    
    ApplicationContainer sinkBR = sink.Install (nBR.Get (1));
    sinkBR.Start (Seconds (0.0));
    sinkBR.Stop (Seconds (15.0));

    // 8. Monitor & PCAP
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();

    p2p.EnablePcap ("NEWRENO", dNR.Get(0));
    p2p.EnablePcap ("CUBIC", dCB.Get(0));
    p2p.EnablePcap ("BBR", dBR.Get(0));

    std::cout << "Simulation en cours... (15 secondes)" << std::endl;
    Simulator::Stop (Seconds (15.0));
    Simulator::Run ();

    // 9. Analyse des résultats
    monitor->CheckForLostPackets();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
    FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats();

    std::ofstream resultFile;
    resultFile.open("resultats_comparaison.csv");
    resultFile << "Protocole;Debit_Mbps;Pertes;TxPackets;RxPackets\n";

    std::cout << "\n=== COMPARAISON DES PROTOCOLES ===\n";
    
    for (auto &flow : stats) {
        Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(flow.first);
        if (t.destinationPort == port) {
            std::string label;
            if (t.sourceAddress == "10.1.1.1") label = "NEWRENO";
            else if (t.sourceAddress == "10.1.2.1") label = "CUBIC";
            else label = "BBR";
            
            double dureeUtile = 14.0;
            double debit = flow.second.rxBytes * 8.0 / dureeUtile / 1e6;
            
            std::cout << label << " - Debit: " << std::fixed << std::setprecision(2) 
                      << debit << " Mbps, Pertes: " << flow.second.lostPackets << std::endl;
            
            resultFile << label << ";" << debit << ";" << flow.second.lostPackets << ";" 
                      << flow.second.txPackets << ";" << flow.second.rxPackets << "\n";
        }
    }
    
    resultFile.close();
    Simulator::Destroy ();
    return 0;
}