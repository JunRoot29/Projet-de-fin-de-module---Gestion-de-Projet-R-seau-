#!/bin/bash

echo "================================================"
echo "COMPARAISON DES PROTOCOLES TCP"
echo "NewReno vs CUBIC vs BBR"
echo "================================================"
echo ""

cd ~/workspace/ns-3-allinone/ns-3-dev

echo "[1/4] Nettoyage des anciens fichiers..."
rm -f NEWRENO*.pcap CUBIC*.pcap BBR*.pcap
rm -f resultats_comparaison.csv

echo "[2/4] Compilation en cours..."
./ns3 build scratch/comparison-tcp/tcp-comparison

if [ $? -ne 0 ]; then
    echo "ERREUR: La compilation a echoue!"
    exit 1
fi

echo "[3/4] Execution des simulations..."
./ns3 run scratch/comparison-tcp/tcp-comparison

echo "[4/4] Resultats obtenus:"
if [ -f "resultats_comparaison.csv" ]; then
    echo ""
    cat resultats_comparaison.csv
    echo ""
    echo "Fichiers PCAP generes:"
    ls -la NEWRENO*.pcap CUBIC*.pcap BBR*.pcap 2>/dev/null
else
    echo "ERREUR: Fichier resultats_comparaison.csv non trouve"
fi

echo ""
echo "================================================"
echo "EXPÉRIENCE TERMINEE"
echo "================================================"
