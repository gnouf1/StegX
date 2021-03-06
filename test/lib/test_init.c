/*
 * This file is part of the StegX project.
 * Copyright (C) 2018  StegX Team
 * 
 * StegX is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include "cmocka.h"

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "stegx.h"
#include "common.h"

/*
 * Test si la fonction stegx_init fonctionne correctement pour un cas
 * d'insertion sans mot de passe. 
 * =====================================================================
 */
void test_insert_init_without_passwd()
{
    stegx_choices_s *choices = malloc(sizeof(stegx_choices_s));
    choices->host_path = malloc((strlen("../../../env/test/bmp/test1.bmp") + 1) * sizeof(char));
    strcpy(choices->host_path, "../../../env/test/bmp/test1.bmp");
    choices->res_path = malloc((strlen("stdout") + 1) * sizeof(char));
    strcpy(choices->res_path, "stdout");
    choices->passwd = NULL;
    choices->mode = STEGX_MODE_INSERT;
    choices->insert_info = malloc(sizeof(stegx_info_insert_s));
    choices->insert_info->hidden_path =
        malloc((strlen("../../../env/test/others/short.txt") + 1) * sizeof(char));
    strcpy(choices->insert_info->hidden_path, "../../../env/test/others/short.txt");
    choices->insert_info->algo = STEGX_ALGO_EOF;

    info_s *infos = stegx_init(choices);

    int test;
    // Teste si le mode est correct
    test = (infos->mode == STEGX_MODE_INSERT);
    assert_int_equal(test, 1);

    // Teste si le fichier host a bien ete ouvert    
    test = (infos->host.host != NULL);
    assert_int_equal(test, 1);

    // Teste si le fichier a cacher a bien ete ouvert 
    test = (infos->hidden != NULL);
    assert_int_equal(test, 1);

    // Teste si le fichier resultat a bien ete cree 
    test = (infos->res == stdout);
    assert_int_equal(test, 1);

    // Teste si le mot de passe a bien ete initialise
    test = (infos->passwd == NULL);
    assert_int_equal(test, 1);

    if (infos->hidden != NULL)
        fclose(infos->hidden);
    if (infos->host.host != NULL)
        fclose(infos->host.host);
    if (infos->res != NULL)
        fclose(infos->res);
    free(infos);
    free(choices->insert_info->hidden_path);
    free(choices->insert_info);
    free(choices->res_path);
    free(choices->host_path);
    free(choices);
}

/*
 * Test si la fonction stegx_init fonctionne correctement pour un cas
 * d'insertion avec mot de passe. 
 * =====================================================================
 */
void test_insert_init_with_passwd()
{
    stegx_choices_s *choices = malloc(sizeof(stegx_choices_s));
    choices->host_path = malloc((strlen("../../../env/test/bmp/test1.bmp") + 1) * sizeof(char));
    strcpy(choices->host_path, "../../../env/test/bmp/test1.bmp");
    choices->res_path = malloc((strlen("stdout") + 1) * sizeof(char));
    strcpy(choices->res_path, "stdout");
    choices->passwd = malloc((strlen("stegx") + 1) * sizeof(char));
    strcpy(choices->passwd, "stegx");
    choices->mode = STEGX_MODE_INSERT;
    choices->insert_info = malloc(sizeof(stegx_info_insert_s));
    choices->insert_info->hidden_path = "../../../env/test/others/short.txt";
    choices->insert_info->algo = STEGX_ALGO_EOF;

    info_s *infos = stegx_init(choices);

    int test;
    // Teste si le mode est correct
    test = (infos->mode == STEGX_MODE_INSERT);
    assert_int_equal(test, 1);

    // Teste si le fichier host a bien ete ouvert    
    test = (infos->host.host != NULL);
    assert_int_equal(test, 1);

    // Teste si le fichier resultat a bien ete cree 
    test = (infos->res != NULL);
    assert_int_equal(test, 1);

    // Test que le nom du fichier à cacher à bien été découpé. */
    assert_string_equal(infos->hidden_name, "short.txt");

    // Teste si le mot de passe a bien ete initialise
    test = (strcmp(infos->passwd, "stegx") == 0);
    assert_int_equal(test, 1);

    if (infos->hidden != NULL)
        fclose(infos->hidden);
    if (infos->host.host != NULL)
        fclose(infos->host.host);
    if (infos->res != NULL)
        fclose(infos->res);
    free(infos);
    free(choices->insert_info);
    free(choices->res_path);
    free(choices->host_path);
    free(choices);
}

/*
 * Test si la fonction stegx_init fonctionne correctement pour un cas
 * d'insertion invalide (un fichier hote inexistant). 
 * =====================================================================
 */
void test_insert_init_invalid()
{
    stegx_choices_s *choices = malloc(sizeof(stegx_choices_s));
    choices->host_path = malloc((strlen("inexistant") + 1) * sizeof(char));
    strcpy(choices->host_path, "inexistant");
    choices->res_path = malloc((strlen("stdout") + 1) * sizeof(char));
    strcpy(choices->res_path, "stdout");
    choices->passwd = malloc((strlen("stegx") + 1) * sizeof(char));
    strcpy(choices->passwd, "stegx");
    choices->mode = STEGX_MODE_INSERT;
    choices->insert_info = malloc(sizeof(stegx_info_insert_s));
    choices->insert_info->hidden_path =
        malloc((strlen("../../../env/test/others/short.txt") + 1) * sizeof(char));
    strcpy(choices->insert_info->hidden_path, "../../../env/test/others/short.txt");
    choices->insert_info->algo = STEGX_ALGO_EOF;

    info_s *infos = stegx_init(choices);

    // Teste si infos est bien invalide
    assert_null(infos);

    free(choices->insert_info->hidden_path);
    free(choices->insert_info);
    free(choices->res_path);
    free(choices->host_path);
    free(choices);
}

/*
 * Test si la fonction stegx_init fonctionne correctement pour un cas
 * d'extraction sans mot de passe. 
 * =====================================================================
 */
void test_extract_init_without_passwd()
{
    stegx_choices_s *choices = malloc(sizeof(stegx_choices_s));
    choices->host_path = malloc((strlen("../../../env/test/bmp/test1.bmp") + 1) * sizeof(char));
    strcpy(choices->host_path, "../../../env/test/bmp/test1.bmp");
    choices->res_path = malloc((strlen("./") + 1) * sizeof(char));
    strcpy(choices->res_path, "./");
    choices->passwd = NULL;
    choices->mode = STEGX_MODE_EXTRACT;
    choices->insert_info = NULL;

    info_s *infos = stegx_init(choices);

    int test;
    // Teste si le mode est correct
    test = (infos->mode == STEGX_MODE_EXTRACT);
    assert_int_equal(test, 1);

    // Teste si le fichier host a bien ete ouvert    
    test = (infos->host.host != NULL);
    assert_int_equal(test, 1);

    // Teste si le fichier a cacher a bien ete ouvert 
    test = (infos->hidden == NULL);
    assert_int_equal(test, 1);

    // Teste si le fichier resultat a bien ete cree 
    test = (infos->res == NULL);
    assert_int_equal(test, 1);

    // Teste si le mot de passe a bien ete initialise
    test = (infos->passwd == NULL);
    assert_int_equal(test, 1);

    if (infos->hidden != NULL)
        fclose(infos->hidden);
    if (infos->host.host != NULL)
        fclose(infos->host.host);
    if (infos->res != NULL)
        fclose(infos->res);
    free(infos);
    free(choices->res_path);
    free(choices->host_path);
    free(choices);
}

/*
 * Test si la fonction stegx_init fonctionne correctement pour un cas
 * d'extraction avec mot de passe. 
 * =====================================================================
 */
void test_extract_init_with_passwd()
{
    stegx_choices_s *choices = malloc(sizeof(stegx_choices_s));
    choices->host_path = malloc((strlen("../../../env/test/others/short.txt") + 1) * sizeof(char));
    strcpy(choices->host_path, "../../../env/test/others/short.txt");
    choices->res_path = malloc((strlen("./") + 1) * sizeof(char));
    strcpy(choices->res_path, "./");
    choices->passwd = malloc((strlen("stegx") + 1) * sizeof(char));
    strcpy(choices->passwd, "stegx");
    choices->mode = STEGX_MODE_EXTRACT;
    choices->insert_info = NULL;

    info_s *infos = stegx_init(choices);
    assert_non_null(infos);

    int test;
    // Teste si le mode est correct
    test = (infos->mode == STEGX_MODE_EXTRACT);
    assert_int_equal(test, 1);

    // Teste si le fichier a cacher a bien ete ouvert 
    test = (infos->hidden == NULL);
    assert_int_equal(test, 1);

    // Teste si le fichier resultat a bien ete cree 
    test = (infos->res == NULL);
    assert_int_equal(test, 1);

    // Teste si le mot de passe a bien ete initialise
    test = (strcmp(infos->passwd, "stegx") == 0);
    assert_int_equal(test, 1);

    if (infos->hidden != NULL)
        fclose(infos->hidden);
    if (infos->host.host != NULL)
        fclose(infos->host.host);
    if (infos->res != NULL)
        fclose(infos->res);
    free(infos);
    free(choices->res_path);
    free(choices->host_path);
    free(choices);
}

/*
 * Test si la fonction stegx_init fonctionne correctement pour un cas
 * d'extraction invalide (un fichier hote inexistant). 
 * =====================================================================
 */
void test_extract_init_invalid()
{
    // Initialisation.
    stegx_choices_s choices;
    choices.mode = STEGX_MODE_EXTRACT;
    choices.host_path = "inexistant";
    choices.res_path = "./";
    choices.passwd = "stegx";

    // Test que l'erreur est approriée.
    assert_null(stegx_init(&choices));
    assert_int_equal(stegx_errno, ERR_HOST);
}

/*
 * MAIN
 * =====================================================================
 */

int main(void)
{
    /* Structures CMocka contenant la liste des tests. */
    const struct CMUnitTest test_stegx_init_insert[] = {
        cmocka_unit_test(test_insert_init_without_passwd),
        cmocka_unit_test(test_insert_init_with_passwd),
        cmocka_unit_test(test_insert_init_invalid),
    };

    const struct CMUnitTest test_stegx_init_extract[] = {
        cmocka_unit_test(test_extract_init_without_passwd),
        cmocka_unit_test(test_extract_init_with_passwd),
        cmocka_unit_test(test_extract_init_invalid),
    };

    /* Exécute les tests. */
    return cmocka_run_group_tests(test_stegx_init_insert, NULL, NULL)
        + cmocka_run_group_tests(test_stegx_init_extract, NULL, NULL);
}
