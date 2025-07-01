#include <iostream>
#include <cstdlib>
using std::cout;

/* Clase que define e implementa una nave */

class SpaceShip {

  private:
    /* Atributos de la clase */
    int x; //posición en el eje x
    int y; //posición en el eje x
    int type; //tipo de sprite para la nave
    int points; //cantidad de puntos acumulados por la nave
    char sprite[3][3]; //matriz de sprite de la nave

  public:
    /* Constructor */
    SpaceShip(int x, int y, int type) {
      this->x = x;
      this->y = y;
      this->type = type;
      this->points = 0;
      setSprite();
    }

    /* Métodos de la clase */
    
    /**
     * Método que agregar un punto a la nave
     */
    void addPoint() { 
        __asm__ __volatile__(
        "ldr r0, %0\n"    
            "add r0, r0, #1\n" 
            "str r0, %0\n"    
            : "=m" (points)   
            : "m" (points)   
            : "r0", "cc", "memory"
        );
      
    }

    /**
     * Método que quitar un punto a la nave siempre que tenga al menos uno
     */
    void subPoint() {
        __asm__ __volatile__(
        "ldr r0, %0\n" 
        "cmp r0, #0\n"   
        "subgt r0, r0, #1\n" 
        "str r0, %0\n"    
        : "=m" (points)   
        : "m" (points)   
        : "r0", "cc", "memory"
        );
    }

    /**
     * Método para dar un sprite a la nave de acuerdo con el tipo que tenga definido
     */
    void setSprite() {
      /* Da una apariencia a cada sprite de acuerdo con el tipo */
      if (this->type == 0) {
        char temp[3][3] = {{'/', '^', '\\'}, 
                          {'|', '0', '|'}, 
                          {'\\', '#', '/'}};
        std::copy(&temp[0][0], &temp[0][0] + 9, &sprite[0][0]);

      } else if (this->type == 1) {
        char temp[3][3] = {{'|', '^', '|'},
                          {'|', '1', '|'}, 
                          {'|', '|', '|'}};
        std::copy(&temp[0][0], &temp[0][0] + 9, &sprite[0][0]);

      } else if (this->type == 2) {
        char temp[3][3] = {{'{', '^', '}'},
                          {'$', '2', '$'}, 
                          {'{', '-', '}'}};
        std::copy(&temp[0][0], &temp[0][0] + 9, &sprite[0][0]);

      } else if (this->type == 3) {
        char temp[3][3] = {{'|', '-', '|'}, 
                          {'(', '3', ')'}, 
                          {'(', '=', ')'}};
        std::copy(&temp[0][0], &temp[0][0] + 9, &sprite[0][0]);

      } else if (this->type == 4) {
        char temp[3][3] = {{'~', '|', '~'}, 
                          {'|', '4', '|'}, 
                          {'~', '|', '~'}};
        std::copy(&temp[0][0], &temp[0][0] + 9, &sprite[0][0]);

      } else if (this->type == 6) {
        char temp[3][3] = {{'\\', '^', '/'}, 
                          {'-', '6', '-'},
                          {'/', '|', '\\'}};
        std::copy(&temp[0][0], &temp[0][0] + 9, &sprite[0][0]);

      } else if (this->type == 8) {

        char temp[3][3] = {{'/', '#', '\\'}, 
                          {'(', '8', ')'}, 
                          {'\\', '=', '/'}};
        std::copy(&temp[0][0], &temp[0][0] + 9, &sprite[0][0]);

      } else {
        char temp[3][3] = {{'/', '^', '\\'},
                          {'|', '0', '|'}, 
                          {'\\', '#', '/'}};
        std::copy(&temp[0][0], &temp[0][0] + 9, &sprite[0][0]);
      }
    }

    /**
     * Función para acceder al dato de la posición en el eje x
     * Retorna: 
     *      int con la posición en el eje x
     */
    int getX() { return x; }

    /**
     * Función para acceder al dato de la posición en el eje y
     * Retorna: 
     *      int con la posición en el eje y
     */
    int getY() { return y; }

    /**
     * Función para acceder al dato de los puntos de la nave
     * Retorna: 
     *      int con la cantidad de puntos
     */
    int getPoints() { return points; }

    /**
     * Función para acceder a la matriz con el sprite de la nave en una determinada posición
     * Parámetros:
     *      row: un entero con la fila que se necesita
     *      col: un entero con la columna que se necesita
     */
    char getSprite(int row, int col) { return sprite[row][col]; }

    /**
     * Método para dar un valor nuevo a la posición en el eje x
     * Parámetros:
     *      x: un entero con el nuevo valor de la posición en el eje x
     */
    void setX(int x) { this->x = x; }

    /**
     * Método para dar un valor nuevo a la posición en el eje y
     * Parámetros:
     *      y: un entero con el nuevo valor de la posición en el eje y
     */
    void setY(int y) { this->y = y; }
};
