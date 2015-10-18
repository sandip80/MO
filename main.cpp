#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <iterator>
#include <../cpp_common/iterator.hpp>
#include <algorithm>
#include <cassert>
#include <../Artificial_Intelligence_A_Modern_Approach/search.hpp>
using namespace common;

template< typename IN_ITERATOR, typename OUT_ITERATOR >
OUT_ITERATOR flatten( IN_ITERATOR begin, IN_ITERATOR end, OUT_ITERATOR out )
{
    auto p = make_iterator_iterator( begin, end );
    return std::copy( p.first, p.second, out );
}

template< typename STATE_VAR, size_t NUM_VAR, typename ACTION, typename ACT_FUNC, typename LEARNER >
struct MO
{
    typedef std::array< STATE_VAR, NUM_VAR > STATE;
    typedef std::vector< ACTION > MACRO;
    std::map< std::pair< size_t, STATE_VAR >, MACRO > table;
    STATE goal_state;
    ACT_FUNC act_func;
    LEARNER learn;
    template< typename ITER >
    STATE apply_macro( const STATE & st, ITER begin, ITER end ) const { return std::accumulate( begin, end, st, act_func ); }

    template< typename OUT_ITER >
    OUT_ITER solve( STATE st, OUT_ITER out )
    {
        std::vector< MACRO > ret;
        ret.reserve( NUM_VAR );
        for ( size_t i = 0; i < NUM_VAR; ++i )
        {
            auto it = table.find( { i, st[i] } );
            MACRO mac = it == table.end( ) ? learn( table, st, i, goal_state[i] ) : it->second;
            ret.push_back( mac );
        }
        std::copy( ret.begin( ), ret.end( ), out );
    }
};

using namespace std;

int main()
{
    std::vector< std::vector< std::string > > vvs = { { "str", "sasvg" }, { "vagas", " safgfsa " } };
    flatten( vvs.begin( ), vvs.end( ), std::ostream_iterator< std::string >( std::cout ) );
    cout << "Hello World!" << endl;
    return 0;
}

