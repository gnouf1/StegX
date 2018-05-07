/**
 * @file stegx.h
 * @brief Fonctions d'utilisations de la bibliothèque StegX.
 * @details Fichier contenant des fonctions publiques destinées à être utilisées
 * par les applications utilisant la bibliothèque.
 */

#ifndef STEGX_H
#define STEGX_H

#include "stegx_common.h"
#include "stegx_errors.h"

/**
 * @brief Initialise la bibliothèque. 
 * @details Initialise la bibliothèque avec les choix de l'utilisateur en
 * remplissant les structures nécéssaire et en ouvrant les fichiers.
 * @sideeffect Initialise la structure \r{info_s} en fonction des choix de
 * l'utilisateur. Il restera les champs \r{info.hidden_length} et \r{info.algo} à
 * initialiser.
 * @param choices Structure contenant les choix de l'utilisateur.
 * @return Structure privée qui contient les informations pour réaliser la
 * dissimulation et l'extraction. 
 */
info_s* stegx_init(stegx_choices_s * choices);

/**
 * @brief Procédure de fin d'utilisation de la bibliothèque.
 * @sideeffect Libère la mémoire de la structure \r{info_s}.
 * @param infos Structure à libérer. 
 */
void stegx_clear(info_s * infos);

/**
 * @brief Vérifie la compatibilité des fichiers.
 * @sideeffect Remplit le champ \r{info_s.host.type} de la structure \r{info_s}.
 * @param infos Structure privée qui contient les informations pour réaliser la
 * dissimulation et l'extraction. 
 * @return 0 si la vérification s'est bien déroulée, sinon 1 en cas d'erreur.
 */
int stegx_check_compatibility(info_s * infos);

/** 
 * @brief Propose les algorithmes de stéganographie utilisables.
 * @details En fonction des informations rentrées dans \r{info_s}, vérifie pour
 * chaque algorithme qu'il est utilisable en fonction type de fichier hôte et de
 * la taille du fichier à cacher.
 * @sideeffect Initialise et rempli le champ \r{info_s.hidden_length} ainsi que la
 * variable globale \r{stegx_propos_algos}.
 * @param infos Structure contenant les informations concernant la dissimulation.
 * @return 0 si tout se passe bien, sinon 1 en cas d'erreur. 
 */
int stegx_suggest_algo(info_s * infos);

/** 
 * @brief Initialise l'algorithme à utiliser pour la dissimulation. 
 * @details A partir de la variable globale \r{stegx_propos_algos} et de
 * "algo_choosen", cette fonction permet de choisir l'algorithme à utiliser.
 * @sideeffect Remplie le champ \r{info_s.algo} avec l'algorithme choisi, ainsi
 * que le champ \r{info_s.passwd} avec un mot de passe aléatoire si
 * l'utilisateur n'en à pas choisi.
 * @param infos Structure contenant les informations concernant la dissimulation.
 * @param algo_choosen Algorithme choisi par l'utilisateur. 
 * @return 0 si tout se passe bien, sinon 1 en cas d'erreur. 
 */
int stegx_choose_algo(info_s * infos, algo_e algo_choosen);

#endif                          /* ifndef STEGX_H */