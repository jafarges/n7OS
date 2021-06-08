#include <n7OS/console.h>
#include <inttypes.h>
#include <n7OS/cpu.h>
#include <string.h>
#include <n7OS/time.h>

// Nombre de ligne de la console
#define CONSOLE_LINES 25

// Nombre de colones
#define CONSOLE_COLUMNS 80

// Nombre d'espaces par tablulation
#define CONSOLE_TAB 3

// Pointeur sur l'origine de la console (coin supérieur gauche)
uint16_t * const CONSOLE_ORIGIN = (uint16_t *) 0xB8000;


// Ligne courante
int line=0;

// Colone courante
int column=0;

// Font pour l'affichage des caractères (ici blanc)
uint8_t font=0xF;

// Les couleurs d'affichage de caractère et pour le fond
typedef enum color_t {BLACK,BLUE,GREEN,CYAN,RED,MAGENTA,BROWN,GRAY,DK_GRAY,LT_BLUE,LT_GREEN,LT_CYAN,LT_RED,LT_MAGENTA,YELLOW,WHITE} color_t;

/*
* Défini la couleur du texte (elle sera utilisée pour les prochaines utilisation de console_putchar)
*/
void setcolor(color_t c){
    font &= 0xF0; // On ré-intialise la couleur du texte (4 bits de poids faibles)
    font |= c; // Et on lui affecte sa nouvelle valeur
}

/*
* Changement de la couleur de fond
*/
void setbgcolor(color_t c){
        font &= 0x0F; // On ré-initialise la couleur de fond (4 bits de poids forts)
        font |= c<<4; // Et on lui affecte sa nouvelle valeur
}

/*
* Positionne le curseur physique aux coordonnées suivantes 
*    - l : la ligne
*    - c : la colone
*
* Les indices sont en base 0 (Première ligne => l=0)
*/
static inline void setcursor(int l, int c){
    outb(0xF,0x3D4);
    int position=l*CONSOLE_COLUMNS+c;
    outb(position & (0xFF),0x3D5);
    outb(0xE,0x3D4);
    outb((position >> 8),0x3D5);
}

/*
* Place le curseur virtuel (emplacement ou l'on écrit) à la position qui suit (de gauche à droite, de bas en haut)
* If the cursor reaches the last available position, it goes
* back to its origin (0,0)
*/
static inline void nextpos(){
    // Si l'on a atteint la dernière colone, on change de ligne (ou l'on défile si on se trouve en dernière ligne)
    if(column==(CONSOLE_COLUMNS - 1)){
        column=0;
        if(line == CONSOLE_LINES - 1){
            memcpy(CONSOLE_ORIGIN,CONSOLE_ORIGIN+CONSOLE_COLUMNS,(CONSOLE_LINES-1)*CONSOLE_COLUMNS*2);
            // Défilement
            for(int i=0;i<CONSOLE_COLUMNS;i++){
                CONSOLE_ORIGIN[(CONSOLE_LINES-1)*CONSOLE_COLUMNS+i] = (font<<8)|' ';
            }
        }else{
            // Changement de ligne
            line++;
        }
    }else{
        column++;
    }
}

/*
* Affiche le caractère passé en paramètre
*    - c: le caractère à afficher
*/
void console_putchar(const char c){

    if(c > 31 && c < 127){
        // Caractère directement affichable
        CONSOLE_ORIGIN[line*CONSOLE_COLUMNS+column] = (font<<8)|c;
        nextpos();
    }else if(c == '\n'){
        // On effectue le retour chariot
        column = 0;
        if(line < (CONSOLE_LINES - 1)){
            line++;
        }else{
            // On copie le bloc de ligne qui suivent la première pour faire défiler
            memcpy(CONSOLE_ORIGIN,CONSOLE_ORIGIN+CONSOLE_COLUMNS,(CONSOLE_LINES-1)*CONSOLE_COLUMNS*2);
            for(int i=0;i<CONSOLE_COLUMNS;i++){
                CONSOLE_ORIGIN[(CONSOLE_LINES-1)*CONSOLE_COLUMNS+i] = (font<<8)|' ';
            }
        }
    }else if(c == '\b'){
        if(column > 0){
            // Décalage à gauche classique
            column--;
        }else{
            // On passe à la ligne "du dessus" donc on fini en fin de ligne
            column = CONSOLE_COLUMNS-1;
            if(line>0){
                line--;
            }
        }
    }else if(c == '\t'){
        // Tabulation
        for(int j=0;j < CONSOLE_TAB;j++){
            CONSOLE_ORIGIN[line*CONSOLE_COLUMNS+column] = (font<<8)|' ';
            nextpos();
        }
    }else if(c == '\f'){
        // Forme feed : on affiche des espaces sur toutes les lignes,
        // pour toutes les colones
        for(int j=0;j<CONSOLE_LINES;j++){
            for(int i=0;i<CONSOLE_COLUMNS;i++){
                CONSOLE_ORIGIN[j*CONSOLE_COLUMNS+i] = (font<<8)|' ';
            }
        }
        // L'origine est replacée au coin supérieur gauche
        line=0;
        column=0;
    }else if(c == '\r'){
        column=0; // On retourne au début de la ligne
    }  
    // Mise à jour de la position du curseur physique
    setcursor(line,column);
}

/* Affiche la chaîne de caractères passée en paramètre
*    -s: les caractères à afficher
*    -len: le nombre de caractères à afficher
*/
void console_putbytes(const char *s, int len) {
    // On définit la couleur du texte : blanc
    // Et la couleur de fond : cyan
    setcolor(WHITE);
    setbgcolor(CYAN);
	for(int i=0;i<len;i++){
        // Affichage des caractères
        console_putchar(s[i]);
	}
}

void setcursorpos(int lin,int col){
    if(lin<CONSOLE_LINES && col < CONSOLE_COLUMNS){
        line=lin;
        column=col;
    }
}

void display_time(){
    setcolor(BLACK);
    setbgcolor(WHITE);

    // Récupération du temps écoulé depuis le début de l'exécution du noyau
    time_t tElps;
    getTimeElapsed(&tElps);

    // Affichage au format hh:mm:ss
    int dx=8;

    int d_hrs = tElps.hours/10;
    int d_mn = tElps.minutes/10;
    int d_sc = tElps.seconds/10;
    

    // Dizaines d'heures
    CONSOLE_ORIGIN[CONSOLE_COLUMNS-dx]=(font<<8)|('0'+d_hrs);
    tElps.hours -= d_hrs * 10;
    dx--;
    
    // Heures
    CONSOLE_ORIGIN[CONSOLE_COLUMNS-dx]=(font<<8)|('0'+tElps.hours);
    dx--;

    // ':'
    CONSOLE_ORIGIN[CONSOLE_COLUMNS-dx]=(font<<8)|':';
    dx--;

    // Dizaines de minutes
    CONSOLE_ORIGIN[CONSOLE_COLUMNS-dx]=(font<<8)|('0'+d_mn);
    tElps.minutes -= d_mn * 10;
    dx--;
    
    // Minutes
    CONSOLE_ORIGIN[CONSOLE_COLUMNS-dx]=(font<<8)|('0'+tElps.minutes);
    dx--;

    // ':'
    CONSOLE_ORIGIN[CONSOLE_COLUMNS-dx]=(font<<8)|':';
    dx--;


    // Dizaines de secondes
    CONSOLE_ORIGIN[CONSOLE_COLUMNS-dx]=(font<<8)|('0'+d_sc);
    tElps.seconds -= d_sc * 10;
    dx--;
    
    // Secondes
    CONSOLE_ORIGIN[CONSOLE_COLUMNS-dx]=(font<<8)|('0'+tElps.seconds);
    
}