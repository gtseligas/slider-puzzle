#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class Board {
    // private defensive array to copy tiles so that data type is immutable
    vector<vector<int>> tilesCopy;
    int dimension;

    public:
    //* constructor that reads a 2D-array of tiles and saves it in an immutable copy
    Board(vector<vector<int>> tiles) {
        dimension = tiles.size();
        for (int i = 0; i < dimension; i++) {
          vector<int> dummy = vector<int>(dimension);
          tilesCopy.push_back(dummy);
          for (int j = 0; j < dimension; j++)
                tilesCopy[i][j] = tiles[i][j];
        }
    }

    Board() {
      
    }

    // copy constructor
    Board(const Board& b) {
      dimension = b.getDimension();
      for (int i = 0; i < dimension; i++) {
          vector<int> dummy = vector<int>(dimension);
          tilesCopy.push_back(dummy);
          for (int j = 0; j < dimension; j++)
                tilesCopy[i][j] = b.tilesCopy[i][j];
        }
    }

    //* returns the tile array in String format, returning the size of the array in the first line and the rows of the array in each of the following lines
    string toString() const{
        string ret;
        ret += to_string(dimension) + "\n";
        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
              ret += to_string(tilesCopy[i][j]);
              if (j != dimension - 1) ret += " ";
            }
            ret += "\n";
        }
        return ret;
    }

    //* returns the dimensions of the array
    int getDimension() const{
        return dimension;
    }

    //* number of tiles out of place. 0 is not counted as a tile and it does not contribute
    int hamming() {
        int humming = 0;

        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                if (tilesCopy[i][j] != 0 && tilesCopy[i][j] != (i * dimension + j + 1) % (dimension
                        * dimension)) humming++;
            }
        }

        return humming;
    }

    //* returns the manhattan distance. 0 does not contribute to the distance
    int manhattan() {
        int manhattan = 0;

        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                int n = tilesCopy[i][j];
                if (n == 0) continue;
                manhattan += abs(i - targetRow(n)) + abs(j - targetCol(n));
            }
        }

        return manhattan;
    }

    private :
    //* helper for manhattan -> return row in goal board
    int targetRow(int n) {
        if (n == 0) return dimension - 1;
        if (n % dimension == 0) return (n / dimension) - 1;
        return n / dimension;
    }

    //* helper for manhattan -> return col in goal board
    int targetCol(int n) {
        if (n == 0) return dimension - 1;
        if (n % dimension == 0) return dimension - 1;
        return (n % dimension) - 1;
    }

    public:
    //* is this board the goal board?
    bool isGoal() {
        return hamming() == 0;
    }

    
    // is that equal to this?
    bool operator==(const Board &b) {
      if (this->dimension != b.getDimension()) return false;
      for (int i = 0; i < dimension; i++)
            for (int j = 0; j < dimension; j++) {
                if (this->tilesCopy[i][j] != b.tilesCopy[i][j]) return false;
            }

        return true;
    } 



    //* all neighboring boards
    // first it finds the row and column of the zero tile. Than it creates every possible neighbor of the board based on the position of the zero.
    vector<Board> neighbors() {
      int zeroRow = 0, zeroCol = 0;
      vector<Board> neighbors;
        for (int i = 0; i < dimension; i++)
          for (int j = 0; j < dimension; j++)
              if (this->tilesCopy[i][j] == 0) {
                  zeroCol = j;
                  zeroRow = i;
              }

        if (zeroCol != 0) {
            vector<vector<int>> temp = vector<vector<int>>(dimension);
            for (int i = 0; i < dimension; i++) {
              temp[i] = vector<int>(dimension);
                for (int j = 0; j < dimension; j++) {
                    temp[i][j] = tilesCopy[i][j];
                }
            }

            temp[zeroRow][zeroCol] = temp[zeroRow][zeroCol - 1];
            temp[zeroRow][zeroCol - 1] = 0;
            neighbors.push_back(Board(temp));
        }

        if (zeroCol != dimension - 1) {
            vector<vector<int>> temp = vector<vector<int>>(dimension);
            for (int i = 0; i < dimension; i++) {
                temp[i] = vector<int>(dimension);
                for (int j = 0; j < dimension; j++) {
                    temp[i][j] = tilesCopy[i][j];
                }
            }

            temp[zeroRow][zeroCol] = temp[zeroRow][zeroCol + 1];
            temp[zeroRow][zeroCol + 1] = 0;
            neighbors.push_back(Board(temp));
        }

        if (zeroRow != 0) {
            vector<vector<int>> temp = vector<vector<int>>(dimension);
            for (int i = 0; i < dimension; i++) {
                temp[i] = vector<int>(dimension);
                for (int j = 0; j < dimension; j++) {
                    temp[i][j] = tilesCopy[i][j];
                }
            }

            temp[zeroRow][zeroCol] = temp[zeroRow - 1][zeroCol];
            temp[zeroRow - 1][zeroCol] = 0;
            neighbors.push_back(Board(temp));
        }

        if (zeroRow != dimension - 1) {
            vector<vector<int>> temp = vector<vector<int>>(dimension);
            for (int i = 0; i < dimension; i++) {
                temp[i] = vector<int>(dimension);
                for (int j = 0; j < dimension; j++) {
                    temp[i][j] = tilesCopy[i][j];
                }
            }

            temp[zeroRow][zeroCol] = temp[zeroRow + 1][zeroCol];
            temp[zeroRow + 1][zeroCol] = 0;
            neighbors.push_back(Board(temp));
        }

        return neighbors;
    }

    // a board that is obtained by exchanging any pair of tiles
    Board twin() {
        vector<vector<int>> temp = vector<vector<int>>(dimension);
        for (int i = 0; i < dimension; i++) {
           temp[i] = vector<int>(dimension);
           for (int j = 0; j < dimension; j++)
                temp[i][j] = tilesCopy[i][j];
        }

        if (temp[0][0] != 0) {
            if (temp[0][1] != 0) {
                int t = temp[0][1];
                temp[0][1] = temp[0][0];
                temp[0][0] = t;
            }
            else {
                int t = temp[0][0];
                temp[0][0] = temp[1][0];
                temp[1][0] = t;
            }
        }
        else {
            int t = temp[0][1];
            temp[0][1] = temp[1][0];
            temp[1][0] = t;
        }
        return Board(temp);
    }
};
