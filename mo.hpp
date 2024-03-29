#ifndef MO_HPP
#define MO_HPP
#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <iterator>
#include <../cpp_common/iterator.hpp>
#include <algorithm>
#include <cassert>
#include <../Artificial_Intelligence_A_Modern_Approach/search.hpp>
#include <boost/iterator/counting_iterator.hpp>
namespace MO
{
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

    template< typename STATE_VAR, size_t NUM_VAR, typename ACTION, typename ALL_ACTION, typename ACT_FUNC, typename LEARNER >
    struct MO
    {
        typedef array< STATE_VAR, NUM_VAR > STATE;
        typedef vector< ACTION > MACRO;
        mutable map< pair< size_t, STATE_VAR >, MACRO > table;
        STATE goal_state;
        ALL_ACTION all_action;
        ACT_FUNC act_func;
        LEARNER learn;
        template< typename ITER >
        STATE apply_macro( const STATE & st, ITER begin, ITER end ) const { return accumulate( begin, end, st, act_func ); }

        MO( const STATE & goal_state, ALL_ACTION all_action, ACT_FUNC act_func, LEARNER learn ) :
            goal_state( goal_state ), all_action( all_action ), act_func( act_func ), learn( learn ) { }

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

        template< typename OUT_ITER >
        OUT_ITER solve_step( STATE st, OUT_ITER out ) const
        {
            for ( size_t i = 0; i < NUM_VAR; ++i )
            {
                auto it = table.find( { i, st[i] } );
                MACRO mac = it == table.end( ) ? table.insert( { { i, st[i] }, learn( st, i, goal_state[i] ) } ).first->second : it->second;
                st = apply_macro( st, mac.begin( ), mac.end( ) );
                if ( ! mac.empty( ) ) { return std::copy( mac.begin( ), mac.end( ), out ); }
            }
            throw;
        }
    };

    template< typename ACTION, typename STATE_VAR, size_t NUM_VAR, typename ALL_ACTION, typename ACT_FUNC, typename LEARNER >
    auto make_MO( const array< STATE_VAR, NUM_VAR > & goal_state, ALL_ACTION all_action, ACT_FUNC act_func, LEARNER learn )
    { return MO< STATE_VAR, NUM_VAR, ACTION, ALL_ACTION, ACT_FUNC, LEARNER > { goal_state, all_action, act_func, learn }; }
}
#endif // MO_HPP

