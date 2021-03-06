#ifndef __CONSOLE_H__
#define __CONSOLE_H__

/*
 * This is the function called by printf to send its output to the screen. You
 * have to implement it in the kernel and in the user program.
 */
void console_putbytes(const char *s, int len);

/*
* Fonction permettant d'afficher le temps total d'exécution du noyau
* au format hh:mm:ss
*/
void display_time();

/*
* Fonction permettant de modifier l'emplacement du curseur
* d'écriture
*/
void setcursorpos(int lin,int col);

#endif
