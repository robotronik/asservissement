# asservissement

Gère les fonctions d'asservissement du robot.

Le makefile supporte les options de compilations suivantes :
    SDL=[yes|no]        defaut: yes
    DEBUG=[yes|no]      defaut: no

Il est conseillé de faire
    make mrproper
avant de changer un set d'option.

Exemple : pour activer le DEBUG sans la SDL :
    make SDL=no DEBUG=yes
Exemple : pour compiler la cible demo, avec l'option par défaut pour la SDL et
le DEBUG activé :
    make demo DEBUG=yes
