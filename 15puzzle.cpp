#include <iostream>
#include <array>
#include <algorithm>
#include <random>
#include <chrono>

// state representation
typedef std::array<int, 16> puzzle;

// Actions
// move the blank block up
puzzle act(const puzzle& state, int a) {
    auto i = std::find(state.begin(), state.end(), 0) - state.begin();
    puzzle ret = state;

    if ( a == 0 ) {
        if ( i < 4 ) return state;
        std::swap(ret[i], ret[i-4]);
    } else if ( a == 1 ) {
        if ( i > 11 ) return state;
        std::swap(ret[i], ret[i+4]);
    } else if ( a == 2) {
        if ( i % 4 == 0 ) return state;
        std::swap(ret[i], ret[i-1]);
    } else if ( a == 3 ) {
        if ( i % 4 == 3 ) return state;
        std::swap(ret[i], ret[i+4]);
    }

    return ret;
}

// helper
puzzle getRandom();
void printState(puzzle state);

enum Actions { up, down, left, right };


int main() {
    puzzle goal = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0 };

    puzzle randState = getRandom();
    printState(randState);

    std::cout << "up..." << std::endl;

    printState(act(randState, up));

    return 0;
}

puzzle getRandom() {
    puzzle state { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0 };
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle (state.begin(), state.end(), std::default_random_engine(seed));

    return state;
}

void printState(puzzle state) {
    std::cout << "-----------" << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << state[i * 4 + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-----------" << std::endl;
}
