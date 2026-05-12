#!/usr/bin/env python3
import matplotlib.pyplot as plt
import numpy as np
import csv

# Lire les résultats
protocoles = []
debits = []
pertes = []
tx_packets = []
rx_packets = []

try:
    with open('resultats_comparaison.csv', 'r') as f:
        reader = csv.DictReader(f, delimiter=';')
        for row in reader:
            protocoles.append(row['Protocole'])
            debits.append(float(row['Debit_Mbps']))
            pertes.append(int(row['Pertes']))
            tx_packets.append(int(row['TxPackets']))
            rx_packets.append(int(row['RxPackets']))
except FileNotFoundError:
    print("Fichier resultats_comparaison.csv non trouve!")
    exit(1)

# Couleurs
colors = ['#FF6B6B', '#4ECDC4', '#45B7D1']

# Créer la figure
fig, axes = plt.subplots(2, 2, figsize=(14, 10))

# 1. Graphique des débits
ax1 = axes[0, 0]
bars1 = ax1.bar(protocoles, debits, color=colors, alpha=0.8)
ax1.set_ylabel('Débit (Mbps)')
ax1.set_title('Débit moyen par protocole')
ax1.grid(True, alpha=0.3)
for bar, val in zip(bars1, debits):
    ax1.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.02,
             f'{val:.2f}', ha='center', va='bottom')

# 2. Graphique des pertes
ax2 = axes[0, 1]
bars2 = ax2.bar(protocoles, pertes, color=colors, alpha=0.8)
ax2.set_ylabel('Nombre de paquets perdus')
ax2.set_title('Pertes de paquets')
ax2.grid(True, alpha=0.3)
for bar, val in zip(bars2, pertes):
    ax2.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.5,
             f'{val}', ha='center', va='bottom')

# 3. Graphique d'efficacité (Tx/Rx)
ax3 = axes[1, 0]
efficacite = [(r/t*100) for r, t in zip(rx_packets, tx_packets)]
bars3 = ax3.bar(protocoles, efficacite, color=colors, alpha=0.8)
ax3.set_ylabel('Taux de délivrance (%)')
ax3.set_title('Efficacité de transmission')
ax3.set_ylim(95, 100)
ax3.grid(True, alpha=0.3)
for bar, val in zip(bars3, efficacite):
    ax3.text(bar.get_x() + bar.get_width()/2, bar.get_height() - 0.3,
             f'{val:.2f}%', ha='center', va='top')

# 4. Tableau récapitulatif
ax4 = axes[1, 1]
ax4.axis('off')
ax4.axis('tight')
table_data = []
for i, p in enumerate(protocoles):
    table_data.append([
        p,
        f"{debits[i]:.2f} Mbps",
        f"{pertes[i]}",
        f"{tx_packets[i]}",
        f"{rx_packets[i]}",
        f"{efficacite[i]:.2f}%"
    ])
columns = ['Protocole', 'Débit', 'Pertes', 'Tx', 'Rx', 'Efficacité']
table = ax4.table(cellText=table_data, colLabels=columns, 
                  cellLoc='center', loc='center',
                  colColours=['#4472C4']*6)
table.auto_set_font_size(False)
table.set_fontsize(10)
table.scale(1.2, 1.5)

plt.suptitle('Comparaison des protocoles TCP', fontsize=14, fontweight='bold')
plt.tight_layout()
plt.savefig('comparaison_tcp.png', dpi=300, bbox_inches='tight')
plt.show()

print("Graphique sauvegarde: comparaison_tcp.png")