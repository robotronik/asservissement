# asservissement

Gère les fonctions d'asservissement du robot.

## Makefile
Le makefile supporte les options de compilations suivantes :
    SDL=[YES|no]
    DEBUG=[yes|NO]

## Dépendance au matériel

* "hardware" : les fichiers ".c" ne sont pas les mêmes suivant si on est sur PC ou sur PIC (voire sur autre chose à l'avenir (STM32 par exemple))

* "reglage.h" : ce header contient les réglages qui ne sont pas les mêmes suivant le robot (petit, gros, simulé sur PC)***
