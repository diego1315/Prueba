#include <iostream>
#include <string.h>
#include <ncurses.h>

using std::cout;
using std::endl;
using std::string;

/* Clase que define e implementa un menú de inicio */

class Menu {
    private:
        /* Atributos de la clase */
        int selection; //opcion seleccionada

    public: 
        /* Constructor */
        Menu() = default;

        /* Métodos de la clase */

        /**
         * Método que muestra el menú de opciones.
         */
        void showMenu () {
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

            /* Impresión del título y subtítulo */
            attron(COLOR_PAIR(1));
            mvprintw(yMax/4, (xMax/2)-5, " SPACE RACE");
            attroff(COLOR_PAIR(1));
            refresh();

            attron(COLOR_PAIR(2));
            mvprintw(yMax/4 + 1, (xMax/2)-5, " by group 2");
            attroff(COLOR_PAIR(2));
            refresh();

            /* Creación de la ventana para el menú de opciones */
            WINDOW* menuwin = newwin(12, 20, (yMax-yMax*3/4)+2, xMax/2-8);

            /* Refrescamiento del menú */
            wrefresh(menuwin);
            keypad(menuwin,true);

            /* Declaración de las variables para las elecciones de opción */
            string choices[4] = {"Start", "Ranking", "Change Players ", "Exit "};
            int choice;
            int highlight = 0; //opción marcada 

            /* Ciclo para impresión del menú, cambio de la selección y obtención de la opción seleccionada */
            while (true) { 
                int sep = 0; //define la separación entre opciones
                /* Ciclo para imprimir todas las opciones y cambiar el diseño a la opción seleccionada */
                for (int i = 0; i < 4; i++) {
                    if (i == highlight) {
                        wattron(menuwin, A_REVERSE);
                    }
                    mvwprintw(menuwin, i+sep+2, 8-(choices[i].length()/2), "%s", choices[i].c_str());
                    wattroff(menuwin, A_REVERSE);

                    sep+=2; //incrementa la separación entre opciones en 2 unidades
                }

                choice = wgetch(menuwin); //lee la entrada del usuario

                switch (choice) {
                    case KEY_UP: //FLECHA ARRIBA
                        highlight = (highlight == 0) ? 3 : highlight - 1; //cambia la opción seleccionada
                        break;
                    case KEY_DOWN: //FLECHA ABAJO
                        highlight = (highlight == 3) ? 0 : highlight + 1; //cambia la opción seleccionada
                        break;
                    
                }

                if(choice == 10) break; //ENTER -> si se presiona enter sale del ciclo
            }

            selection = highlight+1; //guarda la selección en el parámetro de la opción seleccionada

            /* Limpieza y refrescamiento de la pantalla */
            werase(menuwin);
            delwin(menuwin);
            refresh();
        }

        /**
         * Función para obtener el número de la opción seleccionada
         * Retorna: 
         *      int con el número de opción
         */
        int getSelection() {
            return selection;
        }
};
