# Interface WiMo Calibration Tool 
***

* **Projet** : Interface WiMo Calibration Tool
* **Auteur** : nke Instrumentation
* **Date de création** : 2020

## Description

IHM PC permettant la calibration des capteurs WiMo.<BR>

* **Page Dashboard** : gestion de configuration.
* **Live view** : visualisation temps réel des mesures des capteurs et des indicateurs.
* **Calibration** : calibration du capteur connecté.
* **Factory** : configuration paramètre usine et mise à jour matériel.

## Version/Evolution

### V1.0.0 - 2020.02 -> 2020.06
 - Création du logiciel interface métrologie PC.
 
### V1.0.1 Proto - 2020/06 -> 2020/07

### V1.0.2 Proto - 2020/08
 - CALIBRATION : Correction des erreurs de trie dans les points de calibration.
 - LIVEVIEW : Correction des erreurs de cadence de mesure en liveview.
 - DASHBOARD : Réorganisation des élements + ajout des voies de mesure.
 - FACTORY : Mise à jour des logos de la page.
 - ALL : Ajout de la sélection du texte/nombre dans les TEdit.

### V1.0.3 Proto - 2020/08 - 2020/08/27 (fourniture metro)
 - LIVEVIEW : BUG sur W7, capteur avec plusieurs voies de mesure ne passe pas
   correctement. Le logiciel crash.
   Cette correction a aussi corrigé le problème de crash logiciel
   se produisant lors d'un changement d'un capteur à x voies pour un capteur
   à n voies (x!=n).

 - Ajout au rapport métrologie le numéro de version logiciel.

 - EN COURS : Détection automatique de l'interface métrologie.

### V1.0.4 Proto - 2020/08/27
 - EN COURS : Détection automatique de l'interface métrologie.
 - Correction d'un problème sur la moyenne.
 - Remise en place du message non-commercial.

#### V1.0.4.6
 - Modification dans le fonctionnement de la boucle main.
 - Suppression du while(1) avec QCoreApplication::processEvents();/bGWIMOModbusManager();
 - Remplacement par app.exec();
   et bGWIMOModbusManager fonctionnant en tâche de fond dans un Timer à 1ms.

#### 1.0.4.7
 - Correction du busy pour le submit calibration.
   Il restait activé après submit calibration type / average.

### V1.0.5 Proto - 2020/09/24
 - Détection automatique de l'interface métrologie.
 - EN COURS : Ajout de la licence.
 - EN COURS : Ajout d'un menu bar.

#### V1.0.5.1 Proto - 2020/09/28
 - Correctif sur la détection de COM.

### V1.0.6.0 Proto - 2020/10/07
 - Mise en place d'un mode spécifique pour la production (lecture seul).
 - Mise en place de la mise à jour du logiciel.

### V1.0.7.0 Proto - 2020/11/03
 - Début refonte O2.
 - Calibration Part, création bandeau Channel selection.
 - Debug en grande partie.

### V1.0.7.1 Proto - 2020/11/05
 - Mise en place d'un blocage du menu de navigation quand on est en mesure.
   Ceci pour éviter que le client modifie des paramètrages en cours de mesure.
 - Ajout blocage de l'ajout rapide de point de calibration en cours de mesure.
 - Lors d'un stop du calcul de moyenne, affichage d'un fenêtre de demande
   si la mesure doit être stoppée pour ajouter le point de calibration calculé.
 - Correction de l'affichage des caractères spéciaux carré et cube lors
   de la génération du rapport de métrologie PDF.

### V1.0.7.2 Proto - 2020/11/06
 - Blocage des capteurs non compatibles avec le logiciel.
   Non-compatible PAR, O2, ISE.

### V1.0.7.3 Proto - 2020/11/18
 - Ajout du champs pour modifier l'unité des capteurs.

### V1.0.7.4 Proto - 2020/11/24
 - Bug corrigé : il y a un problème sur l'arrondie dans l'ajout de points de calibration.

### V1.0.8.0 Proto - 2020/12/02
 - La version simplifiée de la gestion de l'O2 a été codée.
   Il s'agit d'une version permettant d'effectuer une correction linéaire
   sur les voies concentrations et saturation.
   Cette version permet de rentrer le sensor code qui est bien pris en charge.
 - La version prend en charge le capteur TDO2, c'est à dire le capteur oxygène
   3 voies avec voie température ajoutée.
 - Ajout du champs calibration temperature du pH.

### V1.0.8.1 Proto - 2020/12/15
 - Refonte complète de la prise en charge de l'O2 (non-terminé).
 - Bug sur l'ajout de la température de calibration corrigé.
 - Ajout d'un controle plus fin sur les nombres à virgule
   (pour calibration température uniquement).

### V1.0.8.2 Proto - 2020/12/10
 - Refonte complète de la prise en charge de l'O2 (partiel).

### 2021/04/06 - V1.0.8.3 Proto
 - Refonte complète de la prise en charge de l'O2 (partiel).
 - Ajout d'un bouton "Delete all calibration lines"
 - Calibration point: remplacement ',' par des points Valider les points
 - Ajout de l'information port COM dans la fenêtre.
 - Implémenté la relance pour les trames d'écriture de configuration.
 - Autorisation des capteurs :
   - S14 PAR
   - S20 Redox
   - S21 Ammonium
   - S22 Potassium
   - S23 Nitrate
   - S24 ISE-5
   - S25 Oxygène temperature

### V1.0.8.4 Release Client - 2021/04/26
 - Fin de la refonte complète de la prise en charge de l'O2.
 - Pre-Release à destination du client.

### V1.0.9.0 - 2021/05/17
 - Mise à jour automatique capteur.
 - Mise à jour automatique interface USB.

### V1.0.9.1 - 2021/05/19
 - Instanciation dynamique dans le main des objets utilisés.
   Cette modification fait suite à un plantage complet du programme en 64 bits.

### V1.0.9.2 - 2021/05/21
 - Modification de texte dans calibration points section.
   Add line             => Add point
   Modify selected line => Modify selected point
   Delete selected line => Delete selected point
   Delete all lines     => Delete all points
 - Tool channel selection => Working channel selection
 - Retrograde délais O2 500ms => 100ms

### V1.0.11.0 - 2021/09/27
 - Correction du bug de plantage au démarrage (pragma pack).
 - Finalisation de la calibration O2.
 - Finalisation de la mise à jour automatique capteur et interface.

### V1.0.11.1 - 2021/09/27
 - Autorisation des valeurs de numéro de série xx_yyyy avec 1 < xx < 255.
 - Fermeture fenêtre sensor update.

### V1.0.11.2 - 2021/09/30
 - Ajout mode automatique test balai.

### V1.0.11.3 - 2021/09/30
 - Modification du driver modbus.
   Generation des trames modbus en continue et transmission des trames entières.
 - Ajout de la gestion du CTUV en mesure à 10 secondes.
 - Affichage mot de passe pour site support.

### V1.1.0.0 - 2021/10/11
 - Passage du compilateur en version 5.15.1 > 5.15.2.
   Version du code fournis avec les dll correspondantes.

### V1.1.0.1 - 2021/10/11

### V1.1.0.2 - 2021/10/19
 - Passage de la cadence minimum d'acquisition 9600 à 1 seconde pour test CEM.

### V1.1.1.0 - 2021/10/27
 - Ajout de la gestion des points de calibration pour l'O2 température.
   Refonte de la gestion des points de calibration, risque de bug/regression
   sur cette partie.

### V1.1.1.1 - 2021/10/28
 - Correction du bug :
   Si TO2 connecté puis branchement d'une autre capteur :
   CALIBRATION > Channel sélection
   La voie est bloquée sur la voie 3.

 - Retrait de la limitation de 2 points de calibration pour l'étalonnage
   du TO2 - voie température.

### V1.2.0.0 - 2021/11/16
 - Ajout gestion de la traduction.
 - Correction sur la température de calibration.
 - Correction problème d'affichage partie O2 (débordement de ligne).
 - Ajout en mode nke des températures 0 et 100% pour le mode sensor code.
 - Refonte variable calibrationID et dashboardID en CPP.

### V1.2.3 - 2021/12/06
 - O2 - Sensor code - Décalage de la valeur d'amplification entre l'affichage
   et la valeur entrée dans le capteur :
   le sensor code SA7 devient SA6 dans la trame.

 - Problème d'arrondie sur voie 2 (1 et 3 OK).

### V1.2.3 - 2021/12/13
 - O2 - Sensor code - Décalage de la valeur d'amplification entre l'affichage
   et la valeur entrée dans le capteur :
   le sensor code SA7 devient SA6 dans la trame.

 - Problème d'arrondie sur voie 2 (1 et 3 OK).

### V1.2.3.2
 - Autorisation des capteurs en version non minimal en mode nke.

### V1.2.3 - 2021/12/13
 - O2 - Sensor code - Décalage de la valeur d'amplification entre l'affichage
   et la valeur entrée dans le capteur :
   le sensor code SA7 devient SA6 dans la trame.

 - Problème d'arrondie sur voie 2 (1 et 3 OK).

### V1.2.3.2
 - Autorisation des capteurs en version non minimal en mode nke.

### V1.2.4 - 2021/12/14
 - Problème sur l'affichage du calibration type.
   Malgré la sélection d'une autre type que 'None' et submit, l'affichage revient sur 'None'.

### V1.2.5 - 2021/12/16
 - Pour TO2, modification de l'affichage du type de calibration :
   multipoint est renommé corrected.
 - Problème sur les points température O2, il faut 3 chiffre
   après la virgule pour les valeurs "corrected".
 - Dev only -
   Suppr bFCheckO2PointIntegrity
   
### V1.2.5.1
 - Fenêtre debug : ajout d'une limitation de taille de chaine.

### V1.2.5.2
 - Acquisition : ajout de la gestion de la non réponse (flag à 0).
 - Diminution délai attente acquisition.

### V1.2.5.3
 - Levé de la restriction du numéro de version logiciel pour les O2 productions.

### V1.2.5.4 - 2022.04.20
 - Correction bug d'affichage de l'entête 3ième voie de mesure capteur O2.

### V1.2.6.0 - 2022/05/11
 - Problème : plus d'enregistrement sur tous les capteurs avec seulement une voie de mesure.

***
### V1.2.7.0 - (2022.07.27)
 - O2 - Ajout bouton skip sur point de calibration en 100% et 0%.
 - O2 - Correction de la différence de mesure entre le graphique pendant la calibration et le mode liveview.

### V1.2.7.1 - (2022.08.23)
 - O2 - Modification des textes de l'O2.
 - Liveview - Ouverture de l'explorer Windows avec le bouton location
   sous le graphique liveview.
 - Prise en compte des modifications de texte dans la traduction.
 - Liveview - Modification couleur voie S07 oxygène saturation #193D8A => #4F6CAB,
   pour différentiation avec S06 oxygène concentration.

### V1.2.7.2 - (2022.08.25)
 - Correction du texte "1 - Place the sensor in a air-saturated water"
   en "1 - Place the sensor in air-saturated water".

### V1.2.7.3 - (2022.08.26)
  - O2 - Correction de bug sur submit Condition in the sample et Sensor code.
    Depuis les dernières modifications les paramètres ne sont pas pris en compte.
  - O2 - Modification du texte dans 100% calibration : "Place sensor in air saturated water"
    => "Immerse sensor into air saturated water".
  - Modification "caracter" en "character" dans les textes de vérification de paramètre.
  - Obligation d'utilisation V1.4.0 pour O2. Toutes les anciennes version
    sont considérées comme non-utilisable avec cette version du WCT.

### V1.2.7.4 - (2022.10.03)
  - Modification de la mise en forme du rapport de calibration :
    le logo instrumentation a été mis à jour, le logo est désormais en bas de page,
    la calibration O2 est désormais prise en compte et affichée dans le rapport de calibration.
  - Factory - Inversion bloc "USB tool software" et "WiMo sensor software".
  - Factory - Mise à jour interface USB : mise en place d'une restriction dans le nom de fichier,
    doit commencer par "SENSOR_INTERFACE". Sinon le fichier est refusé.

### V99.2.7.4 - (2022.10.13)
  - Correction problème sur controle numéro de version.
  - Levé de restriction sur limitation nombre de chiffre après la virgule
    pour valeur raw capteur CT.

### V98.2.7.4 - (2022.11.09)
  - Pour besoin test CEM : modification pour cadence minimal de 2 sec à 1sec en mode nke.

### V98.2.7.5 - (2023.01.17)
  - Ajout d'un passe droit en mode Nke pour pouvoir lire tous les capteurs (même S60-Test).
  - Mise à jour du splashscreen 2023.

### V1.2.7.6 - (2023.02.01)
  - Ajout pour capteur CT de la calibration théorique sous la forme d'un polynoome d'ordre 1.
  - Ajout pour capteur CT de la compensation en température de la valeur Raw de conductivité.
  - Correction d'un bug sur la saisi sur le calibration settings :
    il n'était possible de valider qu'une seule fois, au-delà la modification n'était pas prise en compte.

### V1.2.7.7 - (2023.02.20)
  - Ajout test d'intégrité (division par zéro) sur calcul coefficient de la calibration théorique.
  - Ajout sur rapport de calibration de 3 chiffres après la virgule pour valeur RAW.

***
### V1.3.0.0 - (2023.02.28)
  - Refonte importante pour tentative de correction de crash au démarrage de l'application
    apparue dans les versions précédentes.
    
### V1.3.1.0 - (2023.03.21)
  - Calib - Modification des labels Corrected/Physical/Raw par Measured/Standard.
    
### V1.3.2.0 - (2023.03.23)
  - General - Ajout d'un délais en mode slow mode 9600 entre la requête de la version interface et la conf product.
  
### V1.3.3.0 - (2023.04.24)
  - Calib - Le rapport de calibration indiquait Corrected au lieu de multipoint pour Tbd, Fluo Chla et Fluo-PC (Erreur).
  - Calib - Rapport de calibration changement en Corrected au lieu de multipoint pour voies O2.

### V1.3.3.1 - (2023.04.24)
  - Calib - Rapport de calibration modification Physical/Raw => Standard/Measured.

### V1.3.3.2 - (2023.05.03)
  - Live - Regression plus de données dans csv pour capteur 1 voie.
  
### V1.3.4 - (2023.06.05)
  - Calib/Factory - Version métro - Paramètre CT disponible.
  - Factory - Version métro - Autorisation de la lecture/écriture configuration capteur.
  - Liveview - Amélioration du graphique pour visualiser les trous de mesure (non-réponse par exemple).
  
### V1.3.5 - (2023.06.07)
  - Calib - Autorisation de la saisie de 3 chiffres après la virgule pour l'ensemble des capteurs pour les points Raw/Measured.
  - Calib - Passage des restrictions 0->262144 à -262144->262144 pour l'ensemble des capteurs pour les points Raw/Measured.
  - Calib - Interdiction du 0 pour les valeurs Phy/Standard du capteur nitrate.

### V1.3.6 - (2023.06.16)
  - Dashboard - Elargissement de la fenêtre de sélection du port COM.
  
### V1.3.7 - (2023.10.22)
  - Factory - Ajout des paramètres pour fluo V2.
  - Factory - La commande reset bloque le logiciel après launch.
