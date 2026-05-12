#!/bin/bash

echo "=========================================="
echo "ANALYSE DES CAPTURES PCAP"
echo "=========================================="
echo ""

cd ~/ns-allinone-3.40/ns-3.40

for variant in NEWRENO CUBIC BBR; do
    if [ -f "${variant}-0-0.pcap" ]; then
        echo "=== $variant ==="
        echo "Nombre total de paquets: $(tshark -r ${variant}-0-0.pcap 2>/dev/null | wc -l)"
        echo "Retransmissions: $(tshark -r ${variant}-0-0.pcap -Y tcp.analysis.retransmission 2>/dev/null | wc -l)"
        echo "ACK dupliqués: $(tshark -r ${variant}-0-0.pcap -Y tcp.analysis.duplicate_ack 2>/dev/null | wc -l)"
        echo ""
    fi
done