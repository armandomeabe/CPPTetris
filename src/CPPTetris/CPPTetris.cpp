#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <conio.h> // Para _kbhit() y _getch()
#include <cstdlib> // Para system()

using namespace std;

const int WIDTH = 10;
const int HEIGHT = 20;

enum TetrominoType { I, O, T, S, Z, J, L, NONE };

struct Point {
    int x, y;
};

void clearScreen() {
    system("cls");
}

class Tetromino {
public:
    TetrominoType type;
    Point position;
    vector<vector<int>> shape;

    Tetromino(TetrominoType t, Point pos) : type(t), position(pos) {
        shape = getShape(t);
    }

    vector<vector<int>> getShape(TetrominoType t) {
        switch (t) {
        case I: return {
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        };
        case O: return {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        };
        case T: return {
            {0, 0, 0, 0},
            {0, 1, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0}
        };
        case S: return {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0}
        };
        case Z: return {
            {0, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        };
        case J: return {
            {0, 0, 0, 0},
            {1, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0}
        };
        case L: return {
            {0, 0, 0, 0},
            {0, 0, 1, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0}
        };
        default: return vector<vector<int>>(4, vector<int>(4, 0));
        }
    }

    void rotateClockwise() {
        vector<vector<int>> rotated(4, vector<int>(4, 0));
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                rotated[j][3 - i] = shape[i][j];
            }
        }
        shape = rotated;
    }
};

class Board {
private:
    vector<vector<int>> grid; // Representación del tablero
    vector<Tetromino> tetrominos; // Vector de tetrominos

public:
    vector<Tetromino> GetTetrominos() {
        return this->tetrominos;
    }

    void RotateCurrentTetromino() {
        this->tetrominos[0].rotateClockwise();
    }

    void FallCurrentTetrominoOneRow() {
        Tetromino& current = tetrominos[0];
        if (current.position.y < HEIGHT - 3) {
            current.position.y++;
        }
        else { // Si ya tocamos el fondo, es hora de agregar un nuevo tetromino
            tetrominos.insert(tetrominos.begin(), Tetromino(S, { 1, 1 }));
        }
    }

    void MoveCurrentTetrominoLeft() {
        Tetromino& current = tetrominos[0];
        if (current.position.x > 0) {
            current.position.x--;
        }
    }

    void MoveCurrentTetrominoRight() {
        Tetromino& current = tetrominos[0];
        int maxX = WIDTH - 1;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (current.shape[i][j] == 1) {
                    maxX = min(maxX, WIDTH - 1 - j);
                }
            }
        }
        if (current.position.x < maxX) {
            current.position.x++;
        }
    }

    Board() : grid(HEIGHT, vector<int>(WIDTH, 0)) {
        // Añadir algunos tetrominos para probar
        tetrominos.push_back(Tetromino(L, { 3, 0 }));
        tetrominos.push_back(Tetromino(S, { 5, 5 }));
    }

    // Método para mostrar el tablero
    void display() const {
        // Crear una copia del grid para mostrar los tetrominos
        auto displayGrid = grid;

        // Colocar los tetrominos en el grid
        for (const auto& tetromino : tetrominos) {
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    if (tetromino.shape[i][j] == 1) {
                        int x = tetromino.position.x + j;
                        int y = tetromino.position.y + i;
                        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                            displayGrid[y][x] = 1;
                        }
                    }
                }
            }
        }

        // Mostrar el grid
        for (const auto& row : displayGrid) {
            for (int cell : row) {
                cout << (cell ? "[#]" : "[ ]");
            }
            cout << endl;
        }
    }
};

int main() {
    auto board = new Board();
    board->display(); // Muestra el tablero con tetrominos

    while (true) {
        cout << "Presiona una tecla (A, D, S, W para acciones, Q para salir): ";
        char ch = _getch();
        switch (ch) {
        case 'a':
        case 'A':
            cout << "Presionaste A" << endl;
            board->MoveCurrentTetrominoLeft(); // Mover a la izquierda
            break;
        case 'd':
        case 'D':
            cout << "Presionaste D" << endl;
            board->MoveCurrentTetrominoRight(); // Mover a la derecha
            break;
        case 's':
        case 'S':
            cout << "Presionaste S" << endl;
            board->FallCurrentTetrominoOneRow(); // Lógica para mover el tetromino hacia abajo
            break;
        case 'w':
        case 'W':
            cout << "Presionaste W" << endl;
            board->RotateCurrentTetromino(); // Rotar el tetromino
            break;
        case 'q':
        case 'Q':
            cout << "Saliendo del juego..." << endl;
            delete board; // Liberar memoria
            return 0;
        default:
            cout << "Tecla no reconocida" << endl;
            break;
        }
        clearScreen();
        board->display(); // Mostrar el tablero después de la acción
    }
    delete board; // Liberar memoria
    return 0;
}