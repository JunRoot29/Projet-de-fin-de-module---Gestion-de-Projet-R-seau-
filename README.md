# 🌐 Comparaison des Protocoles TCP : NewReno vs CUBIC vs BBR

> Projet de simulation réseau réalisé avec NS-3.40 dans le cadre de l'ECUE **Gestion de Projet Réseaux** — Université Félix Houphouët-Boigny

---

## 📋 Présentation

Ce projet compare les performances de trois algorithmes majeurs de contrôle de congestion TCP :

| Protocole | Description |
|-----------|-------------|
| **TCP NewReno** | Algorithme classique basé sur la détection de pertes, successeur de TCP Reno |
| **TCP CUBIC** | Algorithme à croissance cubique, algorithme par défaut du noyau Linux |
| **TCP BBR** | Algorithme basé sur l'estimation de la bande passante (développé par Google) |

Les métriques analysées sont : **débit**, **pertes de paquets**, **délai** et **stabilité**, à travers des simulations réalisées avec **NS-3.40**.

---

## 🖥️ Configuration matérielle

### Machine 1 — AGBENONZAN

| Composant | Spécification |
|-----------|---------------|
| Modèle | HP ProBook 440 G5 |
| Processeur | Intel Core i5-8250U @ 1.60 GHz (boost 1.80 GHz) |
| RAM | 8,00 Go |
| Système hôte | Windows 11 Professionnel Insider Preview |
| Environnement | WSL2 (Ubuntu 24.04.1 LTS) |
| NS-3 | 3.40 |

### Machine 2 — SANOGO

| Composant | Spécification |
|-----------|---------------|
| Modèle | HP ProBook 640 G1 |
| Processeur | Intel Core i5-4200M @ 2.50 GHz |
| RAM | 12,0 Go |
| Système hôte | Windows 10 Professionnel |
| Environnement | MSYS2 MINGW64 |
| NS-3 | 3.40 |

---

## 🛠️ Outils utilisés

| Outil | Version | Usage |
|-------|---------|-------|
| NS-3 | 3.40 | Simulateur réseau événementiel |
| C++ | — | Développement des scripts de simulation |
| Python | 3.12 | Traitement des données et visualisation |
| Matplotlib | — | Génération des graphiques |
| Wireshark | — | Analyse des captures paquets (PCAP) |
| Bash | — | Automatisation des scripts |
| Git | — | Gestion de version |

---

## 📁 Structure du projet

```
~/workspace/ns-3-allinone/ns-3-dev/
├── scratch/
│   └── comparison-tcp/
│       ├── tcp-comparison.cc      # Script principal NS-3
│       ├── run-experiment.sh      # Script d'exécution
│       ├── plot-results.py        # Visualisation des résultats
│       └── analyse-pcap.sh        # Analyse des captures PCAP
│
├── NEWRENO-0-0.pcap               # Capture NewReno (généré)
├── CUBIC-2-0.pcap                 # Capture CUBIC (généré)
├── BBR-4-0.pcap                   # Capture BBR (généré)
├── resultats_comparaison.csv      # Résultats agrégés (généré)
└── comparaison_tcp.png            # Graphique comparatif (généré)
```

---

## ⚙️ Paramètres de simulation

| Paramètre | Valeur |
|-----------|--------|
| Débit du lien | 2 Mbps |
| Délai de propagation | 100 ms |
| Taille de la file d'attente | 15 paquets |
| Type de file | DropTail |
| Durée de simulation | 15 secondes |
| Trafic généré | OnOff (10 Mbps) |
| Taille des paquets | 1 024 octets |

---

## 🚀 Installation et exécution

### 1. Prérequis système

```bash
# Mise à jour du système
sudo apt update && sudo apt upgrade -y

# Dépendances NS-3
sudo apt install -y build-essential git python3 python3-dev python3-pip \
    cmake mercurial g++ pkg-config sqlite3 qtbase5-dev qtchooser \
    qt5-qmake qtbase5-dev-tools openmpi-bin openmpi-common \
    openmpi-doc libopenmpi-dev gdb valgrind doxygen graphviz \
    imagemagick texlive texlive-latex-extra

# Bibliothèques supplémentaires
sudo apt install -y libgsl-dev libssl-dev libxml2-dev libgtk-3-dev \
    libsqlite3-dev libdouble-conversion-dev libboost-all-dev

# Python pour la visualisation
sudo apt install -y python3-matplotlib python3-numpy
```

### 2. Télécharger et compiler NS-3

```bash
cd ~
wget https://www.nsnam.org/releases/ns-allinone-3.40.tar.bz2
tar -xjf ns-allinone-3.40.tar.bz2
cd ns-allinone-3.40/ns-3.40

./ns3 configure --enable-examples --enable-tests
./ns3 build
```

### 3. Créer le projet

```bash
cd scratch
mkdir -p comparison-tcp
cd comparison-tcp
# Copier les fichiers du projet ici
```

### 4. Rendre les scripts exécutables

```bash
chmod +x run-experiment.sh plot-results.py analyse-pcap.sh
```

### 5. Lancer la simulation

```bash
# Depuis le répertoire principal NS-3
cd ~/ns-allinone-3.40/ns-3.40
./scratch/comparison-tcp/run-experiment.sh
```

---

## 📄 Description des fichiers

### `tcp-comparison.cc` — Script principal NS-3

- Création de 3 paires de nœuds (une par protocole)
- Configuration des variantes TCP via `Config::Set`
- Génération de trafic avec `OnOffHelper`
- Collecte des statistiques avec `FlowMonitor`
- Capture PCAP pour analyse Wireshark

### `run-experiment.sh` — Script d'exécution

- Nettoyage des anciens fichiers
- Compilation du script NS-3
- Exécution de la simulation
- Affichage des résultats

### `plot-results.py` — Visualisation

Génère 4 graphiques :
1. Débit moyen par protocole
2. Pertes de paquets
3. Taux d'efficacité (Tx/Rx)
4. Tableau récapitulatif

### `analyse-pcap.sh` — Analyse PCAP

Extrait depuis les fichiers PCAP :
- Nombre total de paquets
- Nombre de retransmissions
- Nombre d'ACK dupliqués

---

## 📊 Résultats

### Tableau comparatif

| Protocole | Débit (Mbps) | Pertes | Paquets Tx | Paquets Rx | Efficacité |
|-----------|:------------:|:------:|:----------:|:----------:|:----------:|
| **NewReno** | 0,53 | 1 | 1 569 | 1 568 | 99,94 % |
| **CUBIC** | 1,05 | 36 | 3 216 | 3 149 | 97,92 % |
| **BBR** | 0,21 | 5 | 626 | 619 | 98,88 % |

### Analyse par protocole

#### TCP NewReno
- **Comportement** : Conservateur et stable
- **Débit** : Moyen — 0,53 Mbps
- **Pertes** : Très faibles (1 seule perte)
- **Conclusion** : Fiable mais limité sur les liens à haute bande passante

#### TCP CUBIC
- **Comportement** : Agressif, exploration active de la fenêtre
- **Débit** : Élevé — 1,05 Mbps *(meilleur résultat)*
- **Pertes** : Élevées (36 pertes)
- **Conclusion** : Excellent débit au prix de nombreuses retransmissions

#### TCP BBR
- **Comportement** : Stable après la phase de démarrage
- **Débit** : Faible — 0,21 Mbps
- **Pertes** : Faibles (5 pertes)
- **Conclusion** : Sensible à la taille de la file d'attente dans cet environnement

---

## 📈 Analyse des captures PCAP

### Visualisation avec Wireshark

```bash
wireshark NEWRENO-0-0.pcap &
wireshark CUBIC-0-0.pcap &
wireshark BBR-0-0.pcap &
```

### Commandes d'analyse (tshark)

```bash
# Compter les retransmissions par protocole
tshark -r NEWRENO-0-0.pcap -Y "tcp.analysis.retransmission" | wc -l
tshark -r CUBIC-0-0.pcap  -Y "tcp.analysis.retransmission" | wc -l
tshark -r BBR-0-0.pcap    -Y "tcp.analysis.retransmission" | wc -l

# Graphiques TCP dans Wireshark :
# Statistics → TCP Stream Graph → Time-Sequence (Stevens)
```

### Observations sur les graphiques Stevens

| Protocole | Allure du graphique | Interprétation |
|-----------|---------------------|----------------|
| NewReno | Dents de scie régulières | Réduction classique de fenêtre après perte |
| CUBIC | Croissance cubique prononcée | Pente de plus en plus raide entre les pertes |
| BBR | Ligne quasi-droite | Débit constant, peu de variations |

---

## 🔧 Dépannage

### `TcpBbr::GetTypeId()` non trouvé

```bash
# Vérifier la disponibilité de BBR
grep -r "TcpBbr" src/internet/model/

# Si absent, remplacer par une alternative disponible
# ex : TcpWestwoodPlus ou TcpLinuxReno
```

### `import matplotlib` échoue

```bash
sudo apt install python3-matplotlib python3-numpy
```

### Fichiers PCAP non générés

Vérifier que `EnablePcap` est activé dans `tcp-comparison.cc` :

```cpp
p2p.EnablePcap("NEWRENO", dNR.Get(0));
```

### Compilation trop lente

```bash
# Limiter le nombre de threads utilisés
./ns3 configure --enable-examples --enable-tests -- -j2
./ns3 build
```

---

## 📝 Structure du rapport final

1. **Introduction** — Contexte, objectifs, présentation des protocoles
2. **Environnement technique** — Configuration matérielle, outils
3. **Méthodologie** — Configuration NS-3, paramètres, métriques
4. **Analyse des résultats** — NewReno, CUBIC, BBR, comparaison générale
5. **Conclusion** — Synthèse, recommandations, perspectives

---

## 📚 Références

- [Documentation NS-3](https://www.nsnam.org/documentation/)
- [BBR — Google Research](https://research.google/pubs/pub45646/)
- [RFC 8312 — CUBIC](https://tools.ietf.org/html/rfc8312)
- [RFC 6582 — NewReno](https://tools.ietf.org/html/rfc6582)

---

## 👥 Auteurs

| Nom | Machine |
|-----|---------|
| AGBENONZAN | HP ProBook 440 G5 — WSL2 (Ubuntu 24.04) |
| SANOGO | HP ProBook 640 G1 — MSYS2 MINGW64 |

---

## 📜 Licence

Projet académique réalisé dans le cadre de l'**ECUE Gestion de Projet Réseaux** — Université Félix Houphouët-Boigny.

*Dernière mise à jour : Mars 2026*
