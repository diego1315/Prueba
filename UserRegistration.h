#include <iostream>
#include <math.h>
#include <ncurses.h>
#include <string.h>

using std::string;
using std::to_string;

/* Clase que define e implementa un sistema de registro de dos usuarios */

class UserRegistration {

private:
  /* Atributos de la clase */
  string playerOne, playerTwo; // nickname de los jugadores 1 y 2
  int playerOneSprite, playerTwoSprite; // sprite seleccionado para cada jugador

  /**
   * Función que verifica si un string contiene unicamente caracteres
   * alfanuméricos 
   * Parámetros: 
   *      username: un string a evaluar 
   * Retorna: 
   *      un booleano que indica si el string contiene solamente alfanuméricos
   */
  bool alphaOnly(string username) {
    bool alphaOnly = true;
    for (char c : username) {
      if (!isalpha(c)) {
        alphaOnly = false;
        break;
      }
    }

    return alphaOnly;
  }

public:
  /* Constructor */
  UserRegistration() : playerOneSprite(1), playerTwoSprite(2) {}

  /* Métodos de la clase */

  /**
   * Método que muestra el ingreso de un usuario
   * Parámetros:
   *      player: un int con el número del jugador a ingresar
   */
  void showPlayerRegistration(int player) {
    /* Limpieza y refrescamiento de la pantalla, con lectura de teclado (echo) */
    clear();
    refresh();
    echo();
    cbreak();

    /* Obtención del tamaño máximo de la pantalla */
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    /* Impresión del título y el mensaje de restricciones */
    string message = "Player " + to_string(player) + " chose your name";

    attron(COLOR_PAIR(player + 4));
    mvprintw(yMax / 4, (xMax / 2) - 12, "%s", message.c_str());
    attroff(COLOR_PAIR(player + 4));
    refresh();

    attron(COLOR_PAIR(0));
    mvprintw(yMax / 4 + 2, (xMax / 2) - 56,
             "Please, enter a not null username of alphabetic characters, with "
             "at least 2 characters and maximum 5 characters");
    attroff(COLOR_PAIR(0));
    refresh();

    /* Creación de la ventana para el ingreso del nickname */
    WINDOW *inputwin = newwin(3, 20, yMax / 4 + 4, (xMax - 20) / 2);

    /* Muestra del ingreso como caja y refrescamiento del menú */
    box(inputwin, 0, 0);
    refresh();
    wrefresh(inputwin);

    /* Manejo del ingreso del usuario */
    char input[6] = {0};
    mvwgetnstr(inputwin, 1, 1, input, 5);

    /* Cast del ingreso del usuario en un string */
    string username(input);

    /* Verifica si se ingresó el mismo nickname dos veces */
    bool repeated = false;
    if (player == 2 && username == playerOne)
      repeated = true;

    /* Verifica las restricciones del nickname */
    while (!alphaOnly(username) || username.empty() || username.length() < 2 ||
           username.length() > 5 || repeated) {
      repeated = false;

      /* Limpieza y refrescamiento de la pantalla */
      werase(inputwin);
      box(inputwin, 0, 0);
      wrefresh(inputwin);

      /* Impresión del mensaje de error */
      attron(COLOR_PAIR(0));
      mvprintw(yMax / 4 + 8, (xMax / 2) - 7, "Invalid user!");
      attroff(COLOR_PAIR(0));
      refresh();

      /* Manejo del ingreso del usuario */
      mvwgetnstr(inputwin, 1, 1, input, 5);
      username = input;

      /* Verifica si se ingresó el mismo nickname dos veces */
      if (player == 2 && username == playerOne)
        repeated = true;
    }

    /* Guarda el nickname en el usuario correspondiente */
    if (player == 1) {
      playerOne = username;
    } else {
      playerTwo = username;
    }

    /* Limpieza y refrescamiento de la pantalla */
    delwin(inputwin);
    noecho();
    clear();
    refresh();

    /* Mostrar selección de sprite */
    showSpriteSelection(player);
  }

  /**
   * Método que muestra la selección de sprite para un jugador
   * Parámetros:
   *      player: un int con el número del jugador
   */
  void showSpriteSelection(int player) {
    /* Limpieza y refrescamiento de la pantalla */
    clear();
    refresh();
    noecho();
    cbreak();

    /* Obtención del tamaño máximo de la pantalla */
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    /* Impresión del título */
    string message = "Player " + to_string(player) + " choose your spaceship";
    attron(COLOR_PAIR(player + 4));
    mvprintw(yMax / 4, (xMax / 2) - 16, "%s", message.c_str());
    attroff(COLOR_PAIR(player + 4));
    refresh();

    /* Sprites disponibles */
    int availableSprites[] = {0, 1, 2, 3, 4, 6, 8};
    int numSprites = 7;
    int selection = 0; // índice del sprite seleccionado
    int choice;

    /* Bucle de selección */
    while (true) {
      /* Limpiar área de sprites */
      for (int i = yMax / 4 + 3; i < yMax / 4 + 12; i++) {
        move(i, 0);
        clrtoeol();
      }

      /* Mostrar sprites disponibles */
      for (int i = 0; i < numSprites; i++) {
        int startCol = (xMax / 2) - (numSprites * 4) + (i * 8);

        if (i == selection) {
          attron(A_REVERSE);
          mvprintw(yMax / 4 + 2, startCol - 1, "[ %d ]", availableSprites[i]);
          attroff(A_REVERSE);
        } else {
          mvprintw(yMax / 4 + 2, startCol - 1, "  %d  ", availableSprites[i]);
        }

        /* Mostrar el sprite */
        char sprite[3][3];
        getSpriteMatrix(availableSprites[i], sprite);

        if (i == selection) {
          attron(COLOR_PAIR(player + 4));
        }

        for (int row = 0; row < 3; row++) {
          for (int col = 0; col < 3; col++) {
            mvaddch(yMax / 4 + 4 + row, startCol + col, sprite[row][col]);
          }
        }

        if (i == selection) {
          attroff(COLOR_PAIR(player + 4));
        }
      }

      /* Instrucciones */
      attron(COLOR_PAIR(0));
      mvprintw(yMax / 4 + 9, (xMax / 2) - 25,
               "Use LEFT/RIGHT arrows to select, ENTER to confirm");
      attroff(COLOR_PAIR(0));
      refresh();

      /* Leer entrada del usuario */
      choice = getch();

      switch (choice) {
      case KEY_LEFT:
        selection = (selection == 0) ? numSprites - 1 : selection - 1;
        break;
      case KEY_RIGHT:
        selection = (selection == numSprites - 1) ? 0 : selection + 1;
        break;
      case 10: // ENTER
        if (player == 1) {
          playerOneSprite = availableSprites[selection];
        } else {
          playerTwoSprite = availableSprites[selection];
        }
        clear();
        refresh();
        return;
      }
    }
  }

  /**
   * Función que obtiene la matriz de sprites para un tipo dado
   * Parámetros:
   *      type: tipo de sprite
   *      sprite: matriz 3x3 donde se guardará el sprite
   */
  void getSpriteMatrix(int type, char sprite[3][3]) {
    if (type == 0) {
      char temp[3][3] = {{'/', '^', '\\'}, {'|', '0', '|'}, {'\\', '#', '/'}};
      std::copy(&temp[0][0], &temp[0][0] + 9, &sprite[0][0]);
    } else if (type == 1) {
      char temp[3][3] = {{'|', '^', '|'}, {'|', '1', '|'}, {'|', '|', '|'}};
      std::copy(&temp[0][0], &temp[0][0] + 9, &sprite[0][0]);
    } else if (type == 2) {
      char temp[3][3] = {{'{', '^', '}'}, {'$', '2', '$'}, {'{', '-', '}'}};
      std::copy(&temp[0][0], &temp[0][0] + 9, &sprite[0][0]);
    } else if (type == 3) {
      char temp[3][3] = {{'|', '-', '|'}, {'(', '3', ')'}, {'(', '=', ')'}};
      std::copy(&temp[0][0], &temp[0][0] + 9, &sprite[0][0]);
    } else if (type == 4) {
      char temp[3][3] = {{'~', '|', '~'}, {'|', '4', '|'}, {'~', '|', '~'}};
      std::copy(&temp[0][0], &temp[0][0] + 9, &sprite[0][0]);
    } else if (type == 6) {
      char temp[3][3] = {{'\\', '^', '/'}, {'-', '6', '-'}, {'/', '|', '\\'}};
      std::copy(&temp[0][0], &temp[0][0] + 9, &sprite[0][0]);
    } else if (type == 8) {
      char temp[3][3] = {{'/', '#', '\\'}, {'(', '8', ')'}, {'\\', '=', '/'}};
      std::copy(&temp[0][0], &temp[0][0] + 9, &sprite[0][0]);
    } else {
      char temp[3][3] = {{'/', '^', '\\'}, {'|', '0', '|'}, {'\\', '#', '/'}};
      std::copy(&temp[0][0], &temp[0][0] + 9, &sprite[0][0]);
    }
  }

  /**
   * Función para obtener el nickname del primer jugador
   * Retorna:
   *      string con el nickname del primer jugador
   */
  string getPlayerOne() { return playerOne; }

  /**
   * Función para obtener el nickname del segundo jugador
   * Retorna:
   *      string con el nickname del segundo jugador
   */
  string getPlayerTwo() { return playerTwo; }

  /**
   * Función para obtener el sprite del primer jugador
   * Retorna:
   *      int con el tipo de sprite del primer jugador
   */
  int getPlayerOneSprite() { return playerOneSprite; }

  /**
   * Función para obtener el sprite del segundo jugador
   * Retorna:
   *      int con el tipo de sprite del segundo jugador
   */
  int getPlayerTwoSprite() { return playerTwoSprite; }
};
