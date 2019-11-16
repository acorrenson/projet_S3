# Outils et stratégie de développement pour le projet S3

Arthur Correnson <arthur.correnson@gmail>

Ce document présente briévement les outils que j'utiliserai tout au long de la réalisation du projet.

## *Versionning* et sauvegardes

Afin de faire des sauvegardes régulières du code et de favoriser une approche incrémentale du développement, j'utiliserai Git en tant que logiciel de *versionning* ainsi que Github comme plateforme d'hébergement des sources. Ces dernières seront bien sûr gardées privées avant le rendu final du projet.

## éditeur de code

L'outils principal utilisé pour l'écriture du code source sera **Visual Studio Code**. Son *debugger* de programmes C intégré sera également massivement utilisé afin d'accélérer les phases de recherche/correction de bugs.

## Compilateur

Le compilateur C utilisé sera le compilateur **clang**. Il s'agit du compilateur C présent par défaut sous MAC OSX. Afin de garantir la qualité et la compatibilité du code avec toutes les plateformes, les sources seront régulièrement compilées sous machine virtuelle et avec d'autres compilateurs.

## Build

Le câblage du processus de compilation du projet sera effectué de deux manière différentes :

1. d'une part une série de Makefiles pour assurer une certaine universalité du processus de compilation.
2. d'autre part, une version écrite en CMake, une alternative très répendue aux Makefiles. Plusieurs raisons permettent de justifier ce choix :
  + Choisir d'utiliser CMake en plus de makefile permettra aux utilisateurs désireux d'utiliser ce projet comme dépendance de facilement intégrer les sources au sein de leur propre projet Cmake (organisation du code en modules Cmake).
  + CMake est très complet par les fonctionnalités qu'il offre notamment en matière d'expressivité de son langage et de simplicité des fichiers de déscription du processus de compilation.
  + Plusieurs expériences de développement en laboratoire m'ont habituées à prioriser CMake devant Make et je souhaite donc poursuivre mon apprentissage de cet outils qui est un standard dans le monde professionnel.