#include <iostream>
#include <array>
#include <random>

typedef struct {
    int pos;
    int orient;
} cubie;

typedef std::array<cubie, 20> puzzle;
constexpr puzzle goal_state = { { {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0} } };
std::random_device rd;

enum class Action { U, D, L, R, F, B, Ur, Dr, Lr, Rr, Fr, Br };

puzzle permutation(const puzzle& cube, int a, int b, int c, int d) {
    puzzle ret = cube;
    cubie temp = ret[d];
    ret[d] = cube[c];
    ret[c] = cube[b];
    ret[b] = cube[a];
    ret[a] = temp;
    return ret;
}

cubie orientOne(const cubie& c, int direction) {
    cubie ret = c;
    switch (c.orient) {
        case 0 : ret.orient = (direction == 0) ? 1 : 2; return ret;
        case 1 : ret.orient = (direction == 0) ? 2 : 0; return ret;
        case 2 : ret.orient = (direction == 0) ? 0 : 1; return ret;
    }
}

puzzle flip_edges (const puzzle& cube, std::array<int, 4> a) {
    puzzle ret = cube;
    for (int i : a) {
        ret[i].orient = !ret[i].orient;
    }
    return ret;
}

// change definition of parity here if needed (of corners)
puzzle orient(const puzzle& cube, Action a) {
    puzzle ret = cube;
    switch (a) {
        case Action::R :
            ret[3] = orientOne(ret[3], 0);
            ret[5] = orientOne(ret[5], 0);
            ret[1] = orientOne(ret[1], 1);
            ret[7] = orientOne(ret[7], 1);
            return ret;
        case Action::L :
            ret[2] = orientOne(ret[2], 1);
            ret[4] = orientOne(ret[4], 1);
            ret[0] = orientOne(ret[0], 0);
            ret[6] = orientOne(ret[6], 0);
            return ret;
        case Action::F :
            ret[3] = orientOne(ret[3], 1);
            ret[6] = orientOne(ret[6], 1);
            ret[2] = orientOne(ret[2], 0);
            ret[7] = orientOne(ret[7], 0);
            return ret;
        case Action::B :
            ret[1] = orientOne(ret[1], 0);
            ret[4] = orientOne(ret[4], 0);
            ret[0] = orientOne(ret[0], 1);
            ret[5] = orientOne(ret[5], 1);
            return ret;
        case Action::Rr :
            ret[3] = orientOne(ret[3], 1);
            ret[5] = orientOne(ret[5], 1);
            ret[1] = orientOne(ret[1], 0);
            ret[7] = orientOne(ret[7], 0);
            return ret;
        case Action::Lr :
            ret[2] = orientOne(ret[2], 0);
            ret[4] = orientOne(ret[4], 0);
            ret[0] = orientOne(ret[0], 1);
            ret[6] = orientOne(ret[6], 1);
            return ret;
        case Action::Fr :
            ret[3] = orientOne(ret[3], 0);
            ret[6] = orientOne(ret[6], 0);
            ret[2] = orientOne(ret[2], 1);
            ret[7] = orientOne(ret[7], 1);
            return ret;
        case Action::Br :
            ret[1] = orientOne(ret[1], 1);
            ret[4] = orientOne(ret[4], 1);
            ret[0] = orientOne(ret[0], 0);
            ret[5] = orientOne(ret[5], 0);
            return ret;
        default :
            return ret;
    }
}

puzzle act(const puzzle& cube, Action a ) {
    puzzle ret = cube;
    switch (a) {
        case Action::U :
            ret = permutation(ret, 0, 1, 3, 2);
            return permutation(ret, 8, 9, 10, 11);
        case Action::D :
            ret = permutation(ret, 7, 6, 4, 5);
            return permutation(ret, 18, 17, 16, 19);
        case Action::L :
            ret = orient(ret, a);
            ret = flip_edges(ret, {11, 15, 19, 12});
            ret = permutation(ret, 11, 15, 19, 12);
            return permutation(ret, 0, 2, 6, 4);
        case Action::R :
            ret = orient(ret, a);
            ret = flip_edges(ret, {14, 9, 13, 17});
            ret = permutation(ret, 14, 9, 13, 17);
            return permutation(ret, 3, 1, 5, 7);
        case Action::F :
            ret = orient(ret, a);
            ret = flip_edges(ret, {10, 14, 18, 15});
            ret = permutation(ret, 10, 14, 18, 15);
            return permutation(ret, 3, 7, 6, 2);
        case Action::B :
            ret = orient(ret, a);
            ret = flip_edges(ret, {13, 8, 12, 16});
            ret = permutation(ret, 13, 8, 12, 16);
            return permutation(ret, 1, 0, 4, 5);
        case Action::Ur :
            return act(act(act(ret, Action::U), Action::U), Action::U);
        case Action::Dr :
            return act(act(act(ret, Action::D), Action::D), Action::D);
        case Action::Lr :
            return act(act(act(ret, Action::L), Action::L), Action::L);
        case Action::Rr :
            return act(act(act(ret, Action::R), Action::R), Action::R);
        case Action::Fr :
            return act(act(act(ret, Action::F), Action::F), Action::F);
        case Action::Br :
            return act(act(act(ret, Action::B), Action::B), Action::B);
    }
}

template< typename OS >
OS & operator << ( OS & os, const puzzle & state ) {
    os << "corners" << std::endl;
    for (int i = 0; i < 8; i++) {
        os << "(" << state[i].pos << ", " << state[i].orient << ") ";
    }
    os << std::endl << "edges" << std::endl;
    for (int i = 0; i < 12; i++) {
        os << "(" << state[i + 8].pos << ", " << state[i + 8].orient << ") ";
    }
    os << std::endl;
    return os;
}


Action random_action( ) {
    switch (std::uniform_int_distribution<int> (1, 12)(rd)) {
        case 1 : return Action::R;
        case 2 : return Action::L;
        case 3 : return Action::U;
        case 4 : return Action::D;
        case 5 : return Action::F;
        case 6 :  return Action::B;
        case 7 : return Action::Rr;
        case 8 : return Action::Lr;
        case 9 : return Action::Ur;
        case 10 : return Action::Dr;
        case 11 : return Action::Fr;
        default :  return Action::Br;
    }
}

puzzle random_puzzle( ) {
    std::vector<Action> moves;
    for (int i = 0; i < 20; i++) {
        moves.push_back( random_action() );
    }
    return std::accumulate(moves.begin(), moves.end(), goal_state, act);
}

int main() {
    puzzle state = random_puzzle();
    std::cout << goal_state;
    //std::cout << permute5( goal_state, 1, 3, 9, 7, 1);
    std::cout << "=========================" << std::endl;
    std::cout << act(goal_state, Action::R);
    std::cout << "=========================" << std::endl;
    std::cout << act(act(goal_state, Action::R), Action::Rr);
}
