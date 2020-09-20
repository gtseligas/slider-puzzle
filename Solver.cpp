#include <vector>
#include <list>
#include <queue>
#include "Board.cpp"
using namespace std;

class Solver {
    list<Board> solution = list<Board>();
    int moves = -1;
    bool solvable;

    // helper class to use as a wrapper for the game states ; it includes the board, the number of moves used to reach the board starting from the initial state and a reference
    // to the previous state.
    // Manhattan metric is the sum of the board's manhattan distance and the moves used to reach said board, used by A* as priority function.
    class Node {
        public:
        Board board;
        int moves;
        Node* prev;
        int manhattanMetric;

        Node(const Board &b, int m, Node* p) {
            board = Board(b);
            moves = m;
            prev = p;
            manhattanMetric = board.manhattan() + moves;
        }

        int manhattan() const{
            return manhattanMetric;
        }

        friend bool operator<(const Node& a, const Node& b) {
          return a.manhattan() > b.manhattan();
        }
    };

    // simple comparator to use in the priority queues
    class Compare{
    public:
      bool operator() (Node* a, Node* b){
          return a->manhattan() > b->manhattan();
      }
    };

    public:
    // find a solution to the initial board (using the A* algorithm), and by iterating paralelly on the initial board and its twin to find out who is solvable
    Solver(Board initial) {
        priority_queue<Node*, vector<Node*>, Compare> gamestate;
        priority_queue<Node*, vector<Node*>, Compare> parallel;

        gamestate.push(new Node(initial, 0, nullptr));
        parallel.push(new Node(initial.twin(), 0, nullptr));

        while (true) {
            // remove current minimum elements
            Node* min = gamestate.top();
            gamestate.pop();
          
            Node* parMin = parallel.top();
            parallel.pop();

            // check if any of the minimum's is the goal
            if (min->board.isGoal()) {
                solvable = true;
                backtrack(min);
                break;
            }

            if (parMin->board.isGoal()) {
                solvable = false;
                solution.clear();
                break;
            }

            // add to PQ's the neighbors of the minimums ; check every time if the neighbor is the same as the minimum's parent and if yes discard it
            for (Board b : min->board.neighbors()) {
                if (min->prev != nullptr && b==(min->prev->board)) continue;
                gamestate.push(new Node(b, min->moves + 1, min));
            }

            for (Board b : parMin->board.neighbors()) {
                if (parMin->prev != nullptr && b==(parMin->prev->board)) continue;
                parallel.push(new Node(b, parMin->moves + 1, parMin));
            }
        }
    }

    // backtrack to find the path that leads to the goal
    void backtrack(Node* min) {
        while(min != nullptr) {
          solution.push_front(min->board);
          min = min->prev;
        }
    }

    // is the initial board solvable?
    bool isSolvable() {
        return solvable;
    }

    // min number of moves to solve initial board; -1 if unsolvable
    int getMoves() {
        if (solvable) return moves;
        return -1;
    }

    // sequence of boards in a shortest solution; null if unsolvable
    list<Board> getSolution() {
        if (solvable) return solution;
        // return empty list
        return list<Board>();
    }
};

/*
// test client
int main() {
    // create initial board from file
    vector<vector<int>> v = { {7,8,4}, {1,5,6}, {0,3,2}};
    Board b = Board(v);
    Solver s = Solver(b);
    for (Board b : s.getSolution()) 
    cout<<"SOLUTION "<<b.toString();
}*/
