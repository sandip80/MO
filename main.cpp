#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <iterator>
#include <../cpp_common/iterator.hpp>
#include <algorithm>
#include <cassert>
#include <../Artificial_Intelligence_A_Modern_Approach/search.hpp>
#include <15puzzle.hpp>
#include <boost/iterator/counting_iterator.hpp>
using namespace common;
using namespace std;
using namespace AI;
using boost::make_counting_iterator;
template< typename IN_ITERATOR, typename OUT_ITERATOR >
OUT_ITERATOR flatten( IN_ITERATOR begin, IN_ITERATOR end, OUT_ITERATOR out )
{
    auto p = make_iterator_iterator( begin, end );
    return copy( p.first, p.second, out );
}

template< typename STATE_VAR, size_t NUM_VAR, typename ACTION, typename ACT_FUNC, typename LEARNER >
struct MO
{
    typedef array< STATE_VAR, NUM_VAR > STATE;
    typedef vector< ACTION > MACRO;
    mutable map< pair< size_t, STATE_VAR >, MACRO > table;
    STATE goal_state;
    const vector< ACTION > & all_act;
    ACT_FUNC act_func;
    LEARNER learn;
    template< typename ITER >
    STATE apply_macro( const STATE & st, ITER begin, ITER end ) const { return accumulate( begin, end, st, act_func ); }

    MO( const STATE & goal_state, const vector< ACTION > & all_act, ACT_FUNC act_func, LEARNER learn ) :
        goal_state( goal_state ), all_act( all_act ), act_func( act_func ), learn( learn ) { }

    template< typename OUT_ITER >
    OUT_ITER solve( STATE st, OUT_ITER out ) const
    {
        vector< MACRO > ret;
        ret.reserve( NUM_VAR );
        for ( size_t i = 0; i < NUM_VAR; ++i )
        {
            auto it = table.find( { i, st[i] } );
            MACRO mac = it == table.end( ) ? table.insert( { { i, st[i] }, learn( st, i, goal_state[i] ) } ).first->second : it->second;
            st = apply_macro( st, mac.begin( ), mac.end( ) );
            ret.push_back( mac );
        }
        return copy( ret.begin( ), ret.end( ), out );
    }
};

template< typename STATE_VAR, size_t NUM_VAR, typename ACTION, typename ACT_FUNC, typename LEARNER >
auto make_MO( const array< STATE_VAR, NUM_VAR > & goal_state, const vector< ACTION > & all_act, ACT_FUNC act_func, LEARNER learn )
{ return MO< STATE_VAR, NUM_VAR, ACTION, ACT_FUNC, LEARNER > { goal_state, all_act, act_func, learn }; }

int main()
{
    vector< vector< Action > > ret;
    size_t learned = 0;
    auto mo = make_MO(
        goal_state,
        all_action( ),
        act,
        [&]( const puzzle & state, size_t i, const auto & change_to )
        {
            ++learned;
            vector< Action > ret;
            breadth_first_search< Action >(
                state,
                []( const auto &, auto out ) { return std::copy( all_action( ).begin( ), all_action( ).end( ), out ); },
                act,
                [&]( const auto & st )
                {
                    return
                        all_of(
                            make_counting_iterator( static_cast< size_t >( 0 ) ),
                            make_counting_iterator( i ),
                            [&]( size_t i ){ return true || state[i] == st[i]; } ) && (st[i] == change_to );
                },
                back_inserter( ret ) );
            return ret;
        } );
    while ( true ) {
        learned = 0;
        mo.solve( random_puzzle( ), back_inserter( ret ) );
        std::cout << learned;
        std::cin.get( );
    }
    return 0;
}
