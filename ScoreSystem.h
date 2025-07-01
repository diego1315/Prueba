#include <cstdlib>
#include <string>
#include <ncurses.h>
#include "DLinkedList.h"
#include "KVPair.h"

using std::string;
using std::to_string;

/* Clase que define e implementa un sistema de puntuaciones */

class ScoreSystem {
    private:
        /* Atributos de la clase */
        DLinkedList<KVPair<string, int>> scores; //lista con los datos de usuario:puntaje

    public:        
        /* Constructor */
        ScoreSystem() = default;

        /* Métodos de la clase */

        /**
         * Método que registra un nuevo usuario siendo que no exista previamente
         * Parámetros:
         *      username: un string con el nickname del usuario a registrar
         */
        void registerUser(string username) {
            KVPair<string, int> newUser(username, 0); //crea un nuevo usuario con puntaje 0
            scores.goToStart();

            if(!scores.contains(newUser))
                scores.append(newUser); //si el usuario no existe lo almacena en la lista
        }

        /**
         * Método que cambiar el puntaje de un usuario existente dado su nickname, mantiene la lista siempre ordenada de manera descendente
         * Parámetros:
         *      username: un string con el nickname del usuario al que se le cambia el puntaje
         *      score: el puntaje a sumar 
         */
        void updateScore(string username, int score) {
            if(score < 0) //si el score fue 0 no es necesario continuar
                return;

            /* Definición de variables */
            int newScore = 0;
            bool inserted = false;

            scores.goToStart(); //ir al inicio de la lista
            while (!scores.atEnd()) //mientras no se llegue al final de la lista
            {
                /* Si se encuentra el dato del usuario buscado */
                if(scores.getElement().key == username) {                    
                    KVPair<string, int> tempPair = scores.remove(); //se remueve el dato del usuario
                    newScore = tempPair.value;
                    if(score > newScore) {
                        newScore = score; //el puntaje anterior se cambia por el nuevo si es mayor
                    }          

                    scores.goToStart(); //ir al inicio de la lista
                    while (!scores.atEnd()) //mientras no se llegue al final de la lista
                    {
                        if(scores.getElement().value < newScore) { //busca la posición del nuevo puntaje, para mantener el orden descendente
                            scores.insert(KVPair<string, int>(username,newScore)); //inserta el registro en la posición adecuada
                            inserted = true;
                            break;
                        }         
                        scores.next(); //siguiente elemento
                    }
                    
                    /* Si al final no se hizo ninguna inserción, inserta al registro en la última posición */
                    if(!inserted) scores.insert(KVPair<string, int>(username,newScore));
                    return;
                }
                
                /* Si no se encuentra el usuario buscado se continua recorriendo la lista */
                scores.next();
            }
        }

        /**
         * Método que muestra el ranking de los mejores 20 jugadores
         */
        void showRanking() {
            /* Limpieza y refrescamiento de la pantalla, sin lectura de teclado (noecho) */
            clear();
            refresh();
            noecho();
            cbreak();

            /* Obtención del tamaño máximo de la pantalla */
            int yMax, xMax;
            getmaxyx(stdscr, yMax, xMax);

            /* Generación del fondo estrellado, con asteriscos posicionados de manera aleatoria */
            for(int i = 0; i < 200; i++) {
                int randomx = rand() % xMax+1;
                int randomy = rand() % yMax+1;
                int randomcolor = rand() % 6 + 1;

                attron(COLOR_PAIR(randomcolor));
                mvprintw(randomy, randomx, "*");
                attroff(COLOR_PAIR(randomcolor));
            }

            /* Impresión del título y el mensaje para continuar */
            attron(COLOR_PAIR(2));
            mvprintw(yMax/4, (xMax/2)-5, "TOP PLAYERS");
            attroff(COLOR_PAIR(2));
            refresh();

            attron(COLOR_PAIR(0));
            mvprintw((yMax-yMax*3/4)+24, (xMax/2)-14, "Press enter to continue... ");
            attroff(COLOR_PAIR(0));
            refresh();

            /* Creación de la ventana para el despliegue del ranking */
            WINDOW* rankScores = newwin(22, xMax/4, (yMax-yMax*3/4)+1, xMax/2-xMax/8);

            /* Muestra del ranking como caja y refrescamiento del menú */
            box(rankScores, 0, 0); 
            refresh();
            wrefresh(rankScores);

            int key; //tecla que presiona el usuario

            /* Ciclo para impresión del ranking en la pantalla */
            while (true) {
                scores.goToStart();
                int i = 0;
                while (!scores.atEnd() && i <= 19) { //recorre la lista hasta los primeros 20 elementos
                    string user = scores.getElement().key;
                    string score = to_string(scores.getElement().value);
                    string entry = to_string(i+1) + ". " + user + ": " + score;
                    
                    /* Imprime la información del usuario con su puntaje actual */
                    attron(COLOR_PAIR(0));
                    mvwprintw(rankScores, i+1, 2, "%s", entry.c_str());
                    attroff(COLOR_PAIR(0));
                    
                    scores.next(); //siguiente elemento en la lista
                    i++; //incrementa el contador de registros
                }

                
                key = wgetch(rankScores); //lee la entrada del usuario
                
                if(key == 10) break; //ENTER -> si se presiona enter sale del ciclo
            }
            
            /* Limpieza y refrescamiento de la pantalla */
            werase(rankScores);
            delwin(rankScores);
            refresh();
        }
};
