# ESP32-S3 LVGL + LovyanGFX Multi-Board Demo

Ce projet ESP-IDF C++ permet de piloter les écrans tactiles Waveshare ESP32-S3 (3,5", 5", 7", 7" type B) avec **LovyanGFX** (driver graphique rapide) et **LVGL** (UI graphique moderne) sur plusieurs cartes, avec gestion du tactile **GT911** via LovyanGFX.

## Prérequis

- ESP-IDF v5.x installé : https://docs.espressif.com/projects/esp-idf/fr/latest/esp32/get-started/
- Python 3.7+
- Git
- Carte ESP32-S3 Waveshare

## Dépendances

LovyanGFX (driver écran + tactile GT911) est déjà inclus dans le dossier
`components/LovyanGFX`. Aucun téléchargement supplémentaire n'est nécessaire.

## Compilation

. $IDF_PATH/export.sh
idf.py set-target esp32s3
idf.py build flash monitor

## Sélection de la carte

Lance `idf.py menuconfig` puis choisis **Board Selection** pour sélectionner ta carte :
- Waveshare ESP32-S3 7 inch
- Waveshare ESP32-S3 7 inch Type B
- Waveshare ESP32-S3 5 inch
- Waveshare ESP32-S3 3.5 inch

Toutes les sources C et C++ incluent `board_config.hpp` pour récupérer les
définitions de brochage correspondant à la carte sélectionnée.
La compilation s'arrêtera si aucune carte n'est sélectionnée dans `idf.py menuconfig`. Un réglage par défaut est prévu dans `sdkconfig.defaults` pour la carte 7 pouces.

## Licence

Ce projet est distribue sous licence MIT. Voir le fichier [LICENSE](LICENSE) pour plus de details.

## Exemple d'interface

L'application affiche désormais un slider et un bouton tactiles.
Le mouvement du slider met à jour la valeur indiquée sous celui-ci,
et chaque appui sur le bouton incrémente un compteur affiché en haut
de l'écran.

La dernière version ajoute un slider de luminosité, un interrupteur, un menu déroulant
et une case à cocher. Le slider de luminosité ajuste immédiatement le rétroéclairage de l'écran.
Les états de ces nouveaux widgets sont
affichés dynamiquement.

## Utilisation du thème

L'interface active le thème par défaut de LVGL dans `main/ui/lvgl_ui.cpp` juste
après `lv_init()`. Les couleurs principale et secondaire sont bleu et rouge et
la police utilisée est Montserrat 14. Un style de base applique ces réglages de
couleur de fond et de police aux widgets lors de leur création.
