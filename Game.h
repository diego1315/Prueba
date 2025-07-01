#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ncurses.h>
#include <random>
#include <thread>
#include "SpaceShip.h"



using std::cout;
using std::endl;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;

/* Clase que define e implementa una instancia de juego */

class Game {

  private:
    /* Atributos de la clase */
    double seconds = 45; //tiempo de juego
    int frecuency = 30; //frecuencia movimiento de meteoritos
    int N = 35; //filas
    int M = 110; //columnas
    SpaceShip *ship1; //nave espacial del jugador 1
    SpaceShip *ship2; //nave espacial del jugador 2
    char **mat; //matriz de juego
    bool tiebreaker = false; //bandera de juego de desempate
    bool end = false; //bandera de final de juego

  public:
    /* Constructores */
    Game() : Game(1, 2) {}

    Game(int ship1Type, int ship2Type) {
      /* Configuración de N y M respecto al tamaño de la pantalla */
      getmaxyx(stdscr, N, M);

      N = N - 2;
      M = M - 2;

      // Configurar generador de números aleatorios
      std::random_device rd;  // Semilla aleatoria
      std::mt19937 gen(rd()); // Generador Mersenne Twister
      std::uniform_int_distribution<> dis(
          1,
          M); // Distribución uniforme entre 1 y M

      this->mat = new char *[N];

      for (int i = 0; i < N; i++) {
        this->mat[i] = new char[M];
      }

      /* Llenado de la matriz con los espacios vacíos y los asteroides */
      for (int i = 0; i < N; i++) {

        for (int j = 0; j < M; j++) {
          this->mat[i][j] = ' ';
        }
        if (i < N - 5)
          for (int j = 0; j < dis(gen) % 5; j++) {
            this->mat[i][dis(gen) % M] = '*';
          }
      }

      /* Crear las naves en el espacio correspondiente */
      this->ship1 = new SpaceShip(M / 4, N - 4, ship1Type);
      this->ship2 = new SpaceShip(3 * M / 4, N - 4, ship2Type);
    }

    /* Destructor */
    ~Game() {
      for (int i = 0; i < N; i++) {
        delete[] mat[i];
      }
      delete[] mat;

      delete ship1;
      delete ship2;

      endwin();
    }
    
    

    /* Métodos de la clase */

    /**
     * Método para la impresión de la matriz en pantalla
     * Parámetros:
     *      perc: el porcentaje de la barra de tiempo
     */
    void printMat(double perc) {

      for (int i = 0; i < N; i++) {
        int offset = 0;
        for (int j = 0; j < M; j++) {
          char charToPrint = mat[i][j];

          if (i == N - 1) {
            if (j == M / 4 + 1) {
              printw("%d", ship1->getPoints());
              refresh();
              if (ship1->getPoints() % 3 == 0 && ship1->getPoints() != 0) {
                attron(COLOR_PAIR(4)); // Verde
                printw(" +speed");
                attroff(COLOR_PAIR(4));
                refresh();
                offset=6;
              }
            }

            if (j == 3 * M / 4-offset) {
              printw("%d", ship2->getPoints());
              refresh();
              if (ship2->getPoints() % 3 == 0 && ship2->getPoints() != 0) {
                attron(COLOR_PAIR(4)); // Verde
                printw(" +speed");
                attroff(COLOR_PAIR(4));
                refresh();
              }
            }
          }

          // Verificar si esta posición corresponde a la nave 1
          if (i >= ship1->getY() && i < ship1->getY() + 3 && j >= ship1->getX() &&
              j < ship1->getX() + 3) {
            int shipRow = i - ship1->getY();
            int shipCol = j - ship1->getX();
            if (mat[i][j] == ' ') {
              charToPrint = ship1->getSprite(shipRow, shipCol);
            } else {
              // Hay colisión
              ship1->setX(M / 4);
              ship1->setY(N - 4);
              attron(COLOR_PAIR(1)); // Activar color rojo
              printw("BOOM!");
              attroff(COLOR_PAIR(1)); // Desactivar color
              refresh();
              if (ship1->getPoints() > 0) {
                ship1->subPoint();
                if (tiebreaker) {
                displayResults();
                sleep_for(std::chrono::milliseconds(2000));
                this->tiebreaker = false;
                this->end = true;
                return;
                }
              }
              
              for (int k = 0; k < 5; k++) {
                mat[i][j] = ' ';
                j++;
              }
            }
          }

          // Verificar si esta posición corresponde a la nave 2
          if (i >= ship2->getY() && i < ship2->getY() + 3 && j >= ship2->getX() &&
              j < ship2->getX() + 3) {
            int shipRow = i - ship2->getY();
            int shipCol = j - ship2->getX();
            if (mat[i][j] == ' ') {
              charToPrint = ship2->getSprite(shipRow, shipCol);
            } else {
              // Hay colisión
              ship2->setX(3 * M / 4);
              ship2->setY(N - 4);
              attron(COLOR_PAIR(1)); // Activar color rojo
              printw("BOOM!");
              attroff(COLOR_PAIR(1)); // Desactivar color
              refresh();
              if (ship2->getPoints() > 0) {
                ship2->subPoint();
                if (tiebreaker) {
                displayResults();
                sleep_for(std::chrono::milliseconds(2000));
                this->tiebreaker = false;
                this->end = true;
                return;
              }
              }
              for (int k = 0; k < 5; k++) {
                mat[i][j] = ' ';
                j++;
              }
            }
          }

          // Determinar si es parte de una nave y aplicar color correspondiente
          bool isShip1 =
              (i >= ship1->getY() && i < ship1->getY() + 3 &&
              j >= ship1->getX() && j < ship1->getX() + 3 && mat[i][j] == ' ');
          bool isShip2 =
              (i >= ship2->getY() && i < ship2->getY() + 3 &&
              j >= ship2->getX() && j < ship2->getX() + 3 && mat[i][j] == ' ');

          // Aplicar la lógica de la línea vertical del medio
          if ((j == M / 2 || j == M / 2 + 1) && i != N - 1) {
            if ((1 - (double)i / (N - 2)) <= 1 - perc) {
              addch('|');
            } else {
              // Agregar color a meteoritos o naves
              if (charToPrint == '*') {
                attron(COLOR_PAIR(2)); // Activar color amarillo para meteoritos
                addch(charToPrint);
                attroff(COLOR_PAIR(2)); // Desactivar color
              } else if (isShip1) {
                attron(COLOR_PAIR(1)); // Activar color rojo para nave 1
                addch(charToPrint);
                attroff(COLOR_PAIR(1)); // Desactivar color
              } else if (isShip2) {
                attron(COLOR_PAIR(3)); // Activar color azul para nave 2
                addch(charToPrint);
                attroff(COLOR_PAIR(3)); // Desactivar color
              } else {
                addch(charToPrint);
              }
            }
          } else {
            // Agregar color a meteoritos y naves fuera de la línea central
            if (charToPrint == '*') {
              attron(COLOR_PAIR(2)); // Activar color amarillo para meteoritos
              addch(charToPrint);
              attroff(COLOR_PAIR(2)); // Desactivar color
            } else if (isShip1) {
              attron(COLOR_PAIR(1)); // Activar color rojo para nave 1
              addch(charToPrint);
              attroff(COLOR_PAIR(1)); // Desactivar color
            } else if (isShip2) {
              attron(COLOR_PAIR(3)); // Activar color azul para nave 2
              addch(charToPrint);
              attroff(COLOR_PAIR(3)); // Desactivar color
            } else {
              addch(charToPrint);
            }
          }
        }
        addch('\n');
      }
      addch('\n');
      refresh();
    }

    /**
     * Método que actualiza los datos de la matriz, de acuerdo con el movimiento
     * Parámetros:
     *      perc: el porcentaje de la barra de tiempo
     */
    void mov(double perc) {

      char temp = ' ';
      char tempf = ' ';

      for (int i = 0; i < N; i++) {
        temp = mat[i][0];
        tempf = mat[i][M - 1];
        if (i % 2 == 0) {
          for (int j = 0; j < M - 1; j++) {
            mat[i][j] = mat[i][(j + 1) % M];
          }
          mat[i][M - 1] = temp;
        } else {
          for (int j = M - 1; j > 0; j--) {
            mat[i][j] = mat[i][(j - 1 + M) % M];
          }
          mat[i][0] = tempf;
        }
      }

      // Actualizar la pantalla
      clear();
      move(0, 0);
      printMat(perc); //imprime la matriz
    }
    
    /**
     * Método que detecta la tecla presionada para cambiar el posicionamiento de las naves
     */
    void moveShip() {
    int ch = getch();

    switch (ch) {
      case 'w':
      case 'W':
        N-=4;
        moveShipAsm(ship1, 0); // arriba
        N+=4;
        break;

      case 's':
      case 'S':
        N-=4;
        moveShipAsm(ship1, 1); // abajo
        N+=4;
        break;

      case KEY_UP:
        N-=4;
        moveShipAsm(ship2, 0); // arriba
        N+=4;
        break;

      case KEY_DOWN:
        N-=4;
        moveShipAsm(ship2, 1); // abajo
        N+=4;
        break;
    }
}

    /**
     * Función para mostrar la situación de juego en pantalla
     * Retorna: 
     *      bool con el dato de si hubo o no empate (true en caso de que hubo empate, false en el caso contrario)
     */
    bool play() {
      initscr();
      noecho();
      curs_set(FALSE);
      keypad(stdscr, TRUE);
      nodelay(stdscr, TRUE);
      timeout(0);

      flushinp();

      // Inicializar pares de colores
      start_color();
      init_pair(1, COLOR_RED, COLOR_BLACK); // Rojo sobre negro
      init_pair(2, COLOR_YELLOW,
                COLOR_BLACK); // Amarillo sobre negro para meteoritos
      init_pair(3, COLOR_BLUE,
                COLOR_BLACK); // Azul sobre negro

      printMat(0.0);

      // Obtener el tiempo inicial en microsegundos
      auto start_time = std::chrono::high_resolution_clock::now();
      auto last_time = start_time;

      while (true) {

        // Obtener el tiempo actual
        auto current_time = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
            current_time - last_time);

        // Ejecutar animación cada 100,000
        // microsegundos//////////////////////////////////////////////////////////////////////////////////
        if (elapsed.count() >= frecuency * 2000) {
          auto played_time = current_time - start_time;
          double perc =
              static_cast<double>(played_time.count()) / (seconds * 1000000000.0);

          if (tiebreaker) {
            perc = 1.0;
          }

          // Formato legible para el double
          mov(perc);

          if ((perc >= 1.0 && !this->tiebreaker) ||
              this->end) // Agregar la condición del tiebreaker
            break;

          last_time = current_time;
        }
        moveShip();
      }

      if (ship1->getPoints() != ship2->getPoints()) {
        displayResults();
        return false; //si los puntos son diferentes no hay empates
      } else {
        displayResults();
        return true; //si los puntos son iguales hay empate
      }
    }

    /**
     * Método que muestra los resultados del juego en pantalla
     */
    void displayResults() {
      /* Limpia el área y muestra el título */
      clear();
      printw("***SCORES*** \n\n");

      // Mostrar nave 1
      attron(COLOR_PAIR(1)); // Activa el color rojo para la nave

      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          addch(ship1->getSprite(i, j));
          if (i == 1 && j == 2) {
            attroff(COLOR_PAIR(1)); // Desactivar color para puntos
            printw(" => %d", ship1->getPoints());
            attron(COLOR_PAIR(1)); // Reactivar color
          }
        }
        printw("\n");
      }

      attroff(COLOR_PAIR(1)); // Desactiva el color
      printw("\n\n");

      // Mostrar nave 2
      attron(COLOR_PAIR(3)); // Activa el color azul para la nave
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          addch(ship2->getSprite(i, j));
          if (i == 1 && j == 2) {
            attroff(COLOR_PAIR(3)); // Desactivar color para puntos
            printw(" => %d", ship2->getPoints());
            attron(COLOR_PAIR(3)); // Reactivar color
          }
        }
        printw("\n");
      }
      attroff(COLOR_PAIR(3)); // Desactiva el color
      printw("\n");

      /* Obtiene los puntos para dar la salida de quién fue el jugador ganador */
      if (ship1->getPoints() > ship2->getPoints()) { //caso en el que el primer jugador es el ganador
        printw("Winner: Player 1");
        refresh();
        sleep_for(std::chrono::milliseconds(2000));

      } else if (ship1->getPoints() < ship2->getPoints()) { //caso en el que el segundo jugador es el ganador
        printw("Winner: Player 2");
        refresh();
        sleep_for(std::chrono::milliseconds(2000));

      } else { //caso en el que se dio un empate entre los jugadores
        /* Se muestra el conteo para iniciar el juego de desempate */
        this->tiebreaker = true;
        end = false;
        printw("Tie-breaker round in...\n");
        refresh();
        ship1->setX(M / 4);
        ship1->setY(N - 4);
        ship2->setX(3 * M / 4);
        ship2->setY(N - 4);
        sleep_for(std::chrono::milliseconds(1000));
        printw("3\n");
        refresh();
        sleep_for(std::chrono::milliseconds(1000));
        printw("2\n");
        refresh();
        sleep_for(std::chrono::milliseconds(1000));
        printw("1\n\n");
        refresh();
        sleep_for(std::chrono::milliseconds(1000));
      }

      refresh();
      getch(); // Esperar a que el usuario presione una tecla
    }

    /**
     * Función para acceder al dato de los puntos de la nave del primer jugador
     * Retorna: 
     *      int con la cantidad de puntos del primer jugador 
     */
    int getPlayerOneScore() { return ship1->getPoints(); }

    /**
     * Función para acceder al dato de los puntos de la nave del segundo jugador
     * Retorna: 
     *      int con la cantidad de puntos del segundo jugador 
     */
    int getPlayerTwoScore() { return ship2->getPoints(); }

    /**
     * Método para dar un valor nuevo a la bandera de juego de desempate
     * Parámetros:
     *      tiebreaker: un valor booleano para establecer el valor de verdad de la bandera
     */
    void setTiebreaker(bool tiebreaker) { 
      this->tiebreaker = tiebreaker; 
    }
    
    void moveShipAsm(SpaceShip* ship, int direction) {
  __asm__ __volatile__ (
    // Entradas:
    //   r0 = ship      (lo pone el constraints %[ship])
    //   r1 = direction (0=up, 1=down)
    //   r2 = tiebreaker (ptr)
    //   r3 = end        (ptr)

    "push {r4-r8, lr}            \n"

    // r4 = points
    "ldr r4, [r0, #12]           \n"   // points offset

    // r6 = iteraciones = points/(2 ó 3) + 1
    "cmp r1, #0                  \n"
    "moveq r5, #3                \n"
    "movne r5, #2                \n"
    "udiv r6, r4, r5             \n"
    "add  r6, r6, #1             \n"

    // r7 = y actual
    "ldr r7, [r0, #4]            \n"

    // r8 = N‑4 (constante pasada por registro porque puede ser >255)
    "mov  r8, %[nmenos4]         \n"

  "1:                             \n"  // loop
    "cmp  r6, #0                 \n"
    "ble 3f                      \n"

    "cmp  r1, #0                 \n"
    "beq  2f                     \n"

    /* --- mover abajo --- */
    "cmp  r7, r8                 \n"
    "bge  4f                     \n"
    "add  r7, r7, #1             \n"
    "b    4f                     \n"

  /* --- mover arriba --- */
  "2:                             \n"
    "cmp  r7, #0                 \n"
    "ble 5f                      \n"
    "sub  r7, r7, #1             \n"
    "b    4f                     \n"

  /* --- wrap‑around + ++points --- */
  "5:                             \n"
    "mov  r7, r8                 \n"
    /* ++points */
    "ldr  r5, [r0, #12]          \n"
    "add  r5, r5, #1             \n"
    "str  r5, [r0, #12]          \n"

    /* if (*tiebreaker) { *tiebreaker = 0; *end = 1; } */
    "ldr  r5, [r2]               \n"   // r2 = ptr tiebreaker
    "cmp  r5, #0                 \n"
    "beq  4f                     \n"
    "mov  r5, #0                 \n"
    "str  r5, [r2]               \n"
    "mov  r5, #1                 \n"
    "str  r5, [r3]               \n"

  /* --- actualizar Y --- */
  "4:                             \n"
    "str  r7, [r0, #4]           \n"

    "sub  r6, r6, #1             \n"
    "b    1b                     \n"

  "3:                             \n"
    "pop  {r4-r8, pc}            \n"
    :
    : [ship]      "r" (ship),
      [dir]       "r" (direction),
      [tiebreaker]"r" (&tiebreaker),
      [endFlag]   "r" (&end),
      [nmenos4]   "r" (N)
    : "r0","r1","r2","r3","r4","r5","r6","r7","r8","cc","memory"
  );
}
    

    
    
};
