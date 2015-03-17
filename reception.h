#ifndef RECEPTION_H
#define RECEPTION_H

/** Analyse le texte en provenance de l'uart. Transmet les commandes au robot.
 *
 * \note Fonction a exécuter lors des interruption en provenance de l'uart
 *
 * Commandes connues :
 * x=value # prochaine coordonée (en x) que le robot va devoir atteindre
 * y=value # prochaine coordonée (en y) que le robot va devoir atteindre
 * go # applique x et y précédement reçu
 */
void uart_interrupt(char uart_char);


#endif /* RECEPTION_H */
