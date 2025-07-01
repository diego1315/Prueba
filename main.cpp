#include "Game.h"
#include "Menu.h"
#include "ScoreSystem.h"
#include "UserRegistration.h"
#include <cstdlib>
#include <iostream>
#include <ncurses.h>

using std::cin;
using std::cout;

/* Método de inicio */
int main() {
  /* Inicialización de ncurses para los despliegues en pantalla */
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);

  srand(time(
      0)); // definición de la semilla para generación de números aleatorios.

  /* Inicialización del uso de colores para ncurses */
  start_color();

  /* Definición de los pares de colores foreground/backgroud del sistema */
  init_pair(0, COLOR_WHITE, COLOR_BLACK);
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);
  init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(4, COLOR_CYAN, COLOR_BLACK);
  init_pair(5, COLOR_RED, COLOR_BLACK);
  init_pair(6, COLOR_BLUE, COLOR_BLACK);

  /* Limpieza y refrescamiento de pantalla */
  clear();
  refresh();

  /* Definición de los objetos que describen a los módulos del sistema */
  Menu menu;
  ScoreSystem scoreSystem;
  UserRegistration userRegistration;

  int pOneScore;
  int pTwoScore;

  /* Impresión del registro de los jugadores 1 y 2 */
  userRegistration.showPlayerRegistration(1);
  userRegistration.showPlayerRegistration(2);

  /* Registro de los jugadores 1 y 2 */
  scoreSystem.registerUser(userRegistration.getPlayerOne());
  scoreSystem.registerUser(userRegistration.getPlayerTwo());

  while (true) // Mientras el programa esté en ejecución
  {
    /* Impresión del menú de opciones */
    menu.showMenu();

    switch (menu.getSelection()) {
    case 1: { // JUGAR
      /* Empieza el juego y permite guardar los 2 puntajes */
      Game game(userRegistration.getPlayerOneSprite(),
                userRegistration.getPlayerTwoSprite());
      if (game.play()) {
        game.setTiebreaker(true);
        game.play();
      } //crea el juego y en caso de haber empate crea el juego de desempate

      /* Obtiene los puntos obtenidos por los usuarios en el juego */
      pOneScore = game.getPlayerOneScore();
      pTwoScore = game.getPlayerTwoScore();

      /* Actualiza los porcentajes de los usuarios de acuerdo con la partida */
      scoreSystem.updateScore(userRegistration.getPlayerOne(), pOneScore);
      scoreSystem.updateScore(userRegistration.getPlayerTwo(), pTwoScore);
      break;
    }
    case 2: // RANKING
      /* Impresión del ranking actual para los primeros 20 jugadores con mayor
       * puntaje */
      scoreSystem.showRanking();
      break;
    case 3: // CAMBIAR JUGADORES
      /* Impresión del registro de los jugadores 1 y 2 */
      userRegistration.showPlayerRegistration(1);
      userRegistration.showPlayerRegistration(2);

      /* Registro de los jugadores 1 y 2 */
      scoreSystem.registerUser(userRegistration.getPlayerOne());
      scoreSystem.registerUser(userRegistration.getPlayerTwo());

      break;
    case 4: // SALIR
      /* Limpieza y refrescamiento de pantalla */
      clear();
      refresh();
      return 0; /* Finalización */
    default:    // POR DEFECTO
      break;
    }
  }

  /* Limpieza y refrescamiento de pantalla */
  clear();
  endwin();
  return 0; /* Finalización */
}
