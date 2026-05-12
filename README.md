````markdown
# Comparaison des protocoles TCP (NewReno vs CUBIC vs BBR)

## 📋 Description du projet

Ce projet vise à comparer les performances de trois versions majeures du protocole TCP :

| Protocole | Description |
|-----------|-------------|
| **TCP NewReno** | Algorithme classique basé sur la détection de pertes, successeur de TCP Reno |
| **TCP CUBIC** | Algorithme à croissance cubique, par défaut dans le noyau Linux |
| **TCP BBR** | Algorithme basé sur l'estimation de la bande passante (développé par Google) |

L'étude analyse le **débit**, les **pertes de paquets**, le **délai** et la **stabilité** à travers des simulations réalisées avec **NS-3.40**.

---

## 🖥️ Configuration matérielle

### Machine 1 (AGBENONZAN)
| Composant | Spécification |
|-----------|----------------|
| Modèle | HP ProBook 440 G5 |
| Processeur | Intel Core i5-8250U @ 1.60GHz (1.80 GHz) |
| RAM | 8,00 Go |
| Système hôte | Windows 11 Professionnel Insider Preview |
| Environnement | WSL2 (Ubuntu 24.04.1 LTS) |
| NS-3 version | 3.40 |

### Machine 2 (SANOGO)
| Composant | Spécification |
|-----------|----------------|
| Modèle | HP ProBook 640 G1 |
| Processeur | Intel Core i5-4200M @ 2.50GHz |
| RAM | 12,0 Go |
| Système hôte | Windows 10 Professionnel |
| Environnement | MSYS2 MINGW64 |
| NS-3 version | 3.40 |

---

## 🛠️ Outils utilisés

| Outil | Version | Usage |
|-------|---------|-------|
| NS-3 | 3.40 | Simulateur réseau événementiel |
| C++ | - | Développement des scripts de simulation |
| Python 3 | 3.12 | Traitement des données et visualisation |
| Matplotlib | - | Génération des graphiques |
| Wireshark | - | Analyse des captures paquets (PCAP) |
| Bash | - | Automatisation des scripts |
| Git | - | Gestion de version |

---

## 📁 Arborescence du projet

~/workspace/ns-3-allinone/ns-3-dev/
├── scratch/
│   └── comparison-tcp/
│       ├── tcp-comparison.cc          # Script principal NS-3
│       ├── run-experiment.sh          # Script d'exécution
│       ├── plot-results.py            # Visualisation des résultats
│       └── analyse-pcap.sh            # Analyse des captures PCAP
│
├── NEWRENO-0-0.pcap                   # Capture NewReno (généré)
├── CUBIC-2-0.pcap                     # Capture CUBIC (généré)
├── BBR-4-0.pcap                       # Capture BBR (généré)
├── resultats_comparaison.csv          # Résultats (généré)
└── comparaison_tcp.png                # Graphique comparatif (généré)
````

---

## ⚙️ Configuration de la simulation

| Paramètre                   | Valeur          |
| --------------------------- | --------------- |
| Débit du lien               | 2 Mbps          |
| Délai de propagation        | 100 ms          |
| Taille de la file d'attente | 15 paquets      |
| Type de file                | DropTail        |
| Durée de simulation         | 15 secondes     |
| Trafic généré               | OnOff (10 Mbps) |
| Taille des paquets          | 1024 octets     |

---

## 🚀 Installation et exécution

### 1. Prérequis

```bash
# Mettre à jour le système
sudo apt update && sudo apt upgrade -y

# Installer les dépendances NS-3
sudo apt install -y build-essential git python3 python3-dev python3-pip \
    cmake mercurial g++ pkg-config sqlite3 qtbase5-dev qtchooser \
    qt5-qmake qtbase5-dev-tools openmpi-bin openmpi-common \
    openmpi-doc libopenmpi-dev gdb valgrind doxygen graphviz \
    imagemagick texlive texlive-latex-extra

# Installer les bibliothèques supplémentaires
sudo apt install -y libgsl-dev libssl-dev libxml2-dev libgtk-3-dev \
    libsqlite3-dev libdouble-conversion-dev libboost-all-dev

# Installer Python pour la visualisation
sudo apt install -y python3-matplotlib python3-numpy
```

### 2. Télécharger et compiler NS-3

```bash
# Télécharger NS-3.40
cd ~
wget https://www.nsnam.org/releases/ns-allinone-3.40.tar.bz2
tar -xjf ns-allinone-3.40.tar.bz2
cd ns-allinone-3.40/ns-3.40

# Compiler NS-3
./ns3 configure --enable-examples --enable-tests
./ns3 build
```

### 3. Créer le projet

```bash
# Créer le répertoire du projet
cd scratch
mkdir -p comparison-tcp
cd comparison-tcp

# Créer les fichiers (copier-coller depuis ce README)
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

## 📄 Fichiers du projet

### 1. Script principal NS-3 (`tcp-comparison.cc`)

Points clés du code :

* Création de 3 paires de nœuds (une par protocole)
* Configuration des variantes TCP via `Config::Set`
* Utilisation de `OnOffHelper` pour générer le trafic
* FlowMonitor pour collecter les statistiques
* Capture PCAP pour analyse Wireshark

### 2. Script d'exécution (`run-experiment.sh`)

Fonctions :

* Nettoyage des anciens fichiers
* Compilation du script
* Exécution de la simulation
* Affichage des résultats

### 3. Script de visualisation (`plot-results.py`)

Genère 4 graphiques :

* Débit moyen par protocole
* Pertes de paquets
* Taux d'efficacité (Tx/Rx)
* Tableau récapitulatif

### 4. Script d'analyse PCAP (`analyse-pcap.sh`)

Analyse les fichiers PCAP et affiche :

* Nombre total de paquets
* Nombre de retransmissions
* Nombre d'ACK dupliqués

---

## 📊 Résultats obtenus

### Résultats quantitatifs

| Protocole   | Débit (Mbps) | Pertes | Tx   | Rx   | Efficacité |
| ----------- | ------------ | ------ | ---- | ---- | ---------- |
| **NEWRENO** | 0.53         | 1      | 1569 | 1568 | 99.94%     |
| **CUBIC**   | 1.05         | 36     | 3216 | 3149 | 97.92%     |
| **BBR**     | 0.21         | 5      | 626  | 619  | 98.88%     |

### Analyse par protocole

#### TCP NewReno

* **Comportement** : Conservateur et stable
* **Débit** : Moyen (0.53 Mbps)
* **Pertes** : Très faibles (1 seule perte)
* **Conclusion** : Fiable mais moins performant sur bande passante élevée

#### TCP CUBIC

* **Comportement** : Aggressif, exploration active
* **Débit** : Élevé (1.05 Mbps, le meilleur)
* **Pertes** : Élevées (36 pertes)
* **Conclusion** : Excellent débit au prix de nombreuses retransmissions

#### TCP BBR

* **Comportement** : Stable après phase de startup
* **Débit** : Faible (0.21 Mbps)
* **Pertes** : Faibles (5 pertes)
* **Conclusion** : Sensible à la taille de la file d'attente

---

## 📈 Analyse des captures PCAP

### Pour visualiser avec Wireshark

```bash
# Depuis WSL
wireshark NEWRENO-0-0.pcap &
wireshark CUBIC-0-0.pcap &
wireshark BBR-0-0.pcap &
```

### Statistiques clés par protocole

```bash
# Compter les retransmissions
tshark -r NEWRENO-0-0.pcap -Y "tcp.analysis.retransmission" | wc -l
tshark -r CUBIC-0-0.pcap -Y "tcp.analysis.retransmission" | wc -l
tshark -r BBR-0-0.pcap -Y "tcp.analysis.retransmission" | wc -l

# Voir les graphiques TCP
# Dans Wireshark : Statistics → TCP Stream Graph → Time-Sequence (Stevens)
```

### Ce qu'il faut observer

| Protocole | Graphique Stevens  | Observations                      |
| --------- | ------------------ | --------------------------------- |
| NewReno   | Dents de scie      | Réduction classique de fenêtre    |
| CUBIC     | Croissance cubique | Pente de plus en plus raide       |
| BBR       | Ligne droite       | Débit constant, peu de variations |

---

## 🔧 Dépannage

### Erreur: `TcpBbr::GetTypeId()` non trouvé

```bash
# Vérifier si BBR est disponible
grep -r "TcpBbr" src/internet/model/

# Si absent, remplacer par TcpBbrWestwoodPlus ou TcpLinux
```

### Erreur: `import matplotlib` failed

```bash
sudo apt install python3-matplotlib python3-numpy
```

### Erreur PCAP non générés

```bash
# Vérifier que EnablePcap est actif
# Dans tcp-comparison.cc, chercher :
p2p.EnablePcap ("NEWRENO", dNR.Get(0));
```

### La compilation prend trop de temps

```bash
# Utiliser moins de threads
./ns3 configure --enable-examples --enable-tests -- -j2
./ns3 build
```

---

## 📝 Structure du rapport

Le rapport final doit contenir :

1. **Introduction**

   * Contexte et objectifs
   * Présentation des trois protocoles

2. **Environnement technique**

   * Configuration matérielle
   * Outils utilisés

3. **Méthodologie**

   * Configuration NS-3
   * Paramètres de simulation
   * Métriques collectées

4. **Analyse des résultats**

   * TCP NewReno
   * TCP CUBIC
   * TCP BBR
   * Comparaison générale

5. **Conclusion**

   * Synthèse des résultats
   * Recommandations
   * Perspectives

---

## 👥 Auteurs

| Nom        | Machine                   |
| ---------- | ------------------------- |
| AGBENONZAN | HP ProBook 440 G5 (WSL2)  |
| SANOGO     | HP ProBook 640 G1 (MSYS2) |

---

## 📚 Références

* [https://www.nsnam.org/documentation/](https://www.nsnam.org/documentation/)
* [https://research.google/pubs/pub45646/](https://research.google/pubs/pub45646/)
* [https://tools.ietf.org/html/rfc8312](https://tools.ietf.org/html/rfc8312)
* [https://tools.ietf.org/html/rfc6582](https://tools.ietf.org/html/rfc6582)

---

## 📜 Licence

Ce projet a été réalisé dans le cadre de la formation à l'Université Félix Houphouët-Boigny - ECUE Gestion de Projet Réseaux.

---

*Dernière mise à jour : Mars 2026*

```
```
