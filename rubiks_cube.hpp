#include <iostream>
#include <array>
#include <random>

// U, L, F, R, B, D
typedef std::array<size_t, 54> puzzle;
std::random_device rd;

enum class Action { R, L, U, D, F, B };

puzzle permute5( const puzzle& cube, int a, int b, int c, int d, int e) {
    puzzle ret = cube;
    int temp = ret[d - 1];
    ret[d - 1] = ret[c - 1];
    ret[c - 1] = ret[b - 1];
    ret[b - 1] = ret[a - 1];
    ret[a - 1] = temp;
    return ret;
}

puzzle act( const puzzle& cube, Action a ) {
    puzzle ret = cube;
    switch (a) {
        case Action::R :
            ret = permute5( ret, 28, 30, 36, 34, 28 );
            ret = permute5( ret, 29, 33, 35, 31, 29 );
            ret = permute5( ret, 3, 43, 52, 21, 3 );
            ret = permute5( ret, 6, 40, 49, 24, 6 );
            ret = permute5( ret, 9, 37, 46, 27, 9 );
            return ret;
        case Action::L :
            ret = permute5( ret, 10, 12, 18, 16, 10 );
            ret = permute5( ret, 11, 15, 17, 13, 11 );
            ret = permute5( ret, 1, 19, 54, 45, 1 );
            ret = permute5( ret, 4, 22, 51, 42, 4 );
            ret = permute5( ret, 7, 25, 48, 39, 7 );
            return ret;
        case Action::U :
            ret = permute5( ret, 1, 3, 9, 7, 1 );
            ret = permute5( ret, 2, 6, 8, 4, 2 );
            ret = permute5( ret, 19, 10, 37, 28, 19 );
            ret = permute5( ret, 20, 11, 38, 29, 20 );
            ret = permute5( ret, 21, 12, 39, 30, 21 );
            return ret;
        case Action::D :
            ret = permute5( ret, 46,  48,  54,  52,  46 );
            ret = permute5( ret, 47,  51,  53,  49,  47 );
            ret = permute5( ret, 25,  30,  43,  16,  25 );
            ret = permute5( ret, 26,  35,  44,  17,  26 );
            ret = permute5( ret, 27,  36,  45,  18,  27 );
            return ret;
        case Action::F :
            ret = permute5( ret, 19,  21,  27,  25,  19 );
            ret = permute5( ret, 20,  24,  26,  22,  20 );
            ret = permute5( ret, 7,  28,  52,  18,  7 );
            ret = permute5( ret, 8,  31,  53,  15,  8 );
            ret = permute5( ret, 9,  34,  54,  12,  9 );
            return ret;
        default :
            ret = permute5( ret, 37,  39,  45,  43,  37 );
            ret = permute5( ret, 38,  42,  44,  40,  38 );
            ret = permute5( ret, 3,  10,  48,  36,  3 );
            ret = permute5( ret, 2,  13,  47,  33,  2 );
            ret = permute5( ret, 1,  16,  46,  30,  1 );
            return ret;
    }
}

constexpr puzzle goal_state = { { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54 } };

Action random_action( ) {
    switch (std::uniform_int_distribution<int> (1, 6)(rd)) {
        case 1 : return Action::R;
        case 2 : return Action::L;
        case 3 : return Action::U;
        case 4 : return Action::D;
        case 5 : return Action::F;
        default :  return Action::B;
    }
}

puzzle random_puzzle( ) {
    std::vector<Action> moves;
    for (int i = 0; i < 20; i++) {
        moves.push_back( random_action() );
    }
    return std::accumulate(moves.begin(), moves.end(), goal_state, act);
}

template< typename OS >
OS & operator << ( OS & os, const puzzle & state ) {
    char face[] = { 'U', 'L', 'F', 'R', 'B', 'D' };
    for (int i = 0; i < 54; i++) {
        if ( i % 9 == 0 ) {
            os << face[i / 9] << std::endl;
        }
        os << state[i] << " ";
        if ( i % 3 == 2 ) {
            os << std::endl;
        }
    }
    return os;
}

int main() {
    puzzle state = random_puzzle();
    std::cout << goal_state;
    std::cout << act(goal_state, Action::U);
}
