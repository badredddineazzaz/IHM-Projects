# Vision-Based Interaction

**Expériences d’interface homme-machine en C++ et OpenCV.**

[Projets](#projets) · [Prise en main](#prise-en-main) · [Limites](#limites) · [Portfolio](https://github.com/badredddineazzaz)

Ce dépôt de travaux pratiques explore le suivi d’objets par webcam et son utilisation dans une interface : déplacement du curseur Windows, détection de mains et navigation dans des images. Chaque dossier contient une solution Visual Studio indépendante.

## Projets

| Projet | Comportement présent dans le code |
| --- | --- |
| [sourisDetect](sourisDetect) | Segmentation HSV d’un objet rouge, suivi de son centre et lissage sur 10 positions ; déplacement du curseur Windows et clic droit après un maintien détecté |
| [Library](Library) | Suivi d’objet et contrôle du curseur dans un thread ; visualisation d’images JPG et bandeau de sélection dans un autre |
| [HandDetection](HandDetection) | Détection par classificateur en cascade ; affichage et coordonnées du centre des mains détectées |
| [Calibrate](Calibrate) | Prototype de visualiseur JPG avec bandeau de miniatures et navigation clavier |

Le dossier `Calibrate` contient actuellement un visualiseur d’images. Le réglage interactif des seuils de couleur décrit dans l’ancien README n’est pas implémenté.

## Technologies

C++, OpenCV et Visual Studio. `sourisDetect` et `Library` utilisent aussi les fonctions de l’API Windows pour agir sur le curseur.

## Prise en main

```bash
git clone https://github.com/badredddineazzaz/IHM-Projects.git
cd IHM-Projects
```

1. Sous Windows, ouvrez la solution `.sln` du projet choisi dans Visual Studio avec les outils C++.
2. Installez OpenCV pour C++ et configurez ses chemins d’inclusion, bibliothèques et DLL pour la configuration de compilation sélectionnée. Le dépôt ne fournit pas de procédure d’installation portable.
3. Vérifiez les paramètres locaux avant de compiler : les projets webcam ouvrent la caméra d’index **1** ; `HandDetection` attend un fichier de cascade externe ; `Library` et `Calibrate` utilisent des chemins de dossiers JPG écrits dans le code. Le fichier de cascade n’est pas inclus dans le dépôt.
4. Compilez la solution choisie puis lancez-la. Les commandes de compilation et la capture caméra n’ont pas été exécutées pendant cette révision documentaire.

Ces paramètres sont des constantes dans les sources, pas des options de lancement. Une adaptation à votre poste peut donc nécessiter une modification locale du code.

## Utilisation

- Pour `sourisDetect`, présentez un objet rouge à la webcam et observez le déplacement du curseur. Le programme peut agir sur le pointeur du système et déclencher un clic droit.
- Pour `Library`, fournissez des images JPG et utilisez le bandeau pour sélectionner une image.
- Pour `HandDetection`, fournissez un classificateur compatible avant le lancement.
- La touche **Échap** est gérée dans les boucles d’affichage pour quitter.

## Limites

Il s’agit de prototypes pédagogiques avec des paramètres fixes. La détection dépend de la lumière, des seuils de couleur et de la caméra. Le maintien du clic droit dans `sourisDetect` ne comporte pas de remise à zéro explicite pour répéter l’action ; ce README ne promet donc pas un contrôle gestuel complet.

Les dossiers de compilation et exécutables historiques sont conservés. Aucun résultat de performance, compatibilité matérielle ou validation automatisée n’est revendiqué.

## Pistes de travail

Rendre les chemins et paramètres configurables, documenter une compilation reproductible et ajouter une démonstration réelle avant de présenter ces prototypes comme des applications prêtes à distribuer.
