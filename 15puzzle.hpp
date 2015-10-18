#include <iostream>
#include <array>
#include <algorithm>
#include <random>
#include <chrono>
#include <cassert>
typedef std::array<int, 16> puzzle;
enum class Action { up, down, left, right };
puzzle act( const puzzle & state, Action a ) {
    auto i = std::find(state.begin( ), state.end( ), 0) - state.begin( );
    assert( i < 16 );
    auto act = [&]( bool is_valid, size_t swap_with )
    {
        if ( is_valid )
        {
            puzzle ret = state;
            std::swap( ret[i], ret[swap_with] );
            return ret;
        }
        else { return state; }
    };
    switch ( a )
    {
        case Action::up : return act( i > 3, i - 4 );
        case Action::down : return act( i < 12, i + 4 );
        case Action::left : return act( i % 4 > 0, i - 1 );
        case Action::right : return act( i % 4 < 3, i + 1 );
    }
}

puzzle random_puzzle( ) {
    puzzle state = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0 };
    unsigned seed = std::chrono::system_clock::now( ).time_since_epoch( ).count( );
    shuffle( state.begin( ), state.end( ), std::default_random_engine( seed ) );
    return state;
}

template< typename OS >
OS & operator << ( OS & os, const puzzle & state ) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            os << state[i * 4 + j] << " ";
        }
        os << std::endl;
    }
    return os;
}
