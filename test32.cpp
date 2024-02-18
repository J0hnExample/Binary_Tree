// basic tests for treemap with iterator (A3.2)
// Author: Hartmut Schirmacher, Sascha Athmer
// Berliner Hochschule für Technik (BHT)

#include "treemap.h"
#include "payload_v2.h"
#include <algorithm>
#include <cassert>
#include <iostream>

using namespace std; 
using my::treemap;

void test32()
{

    {
        cout << "begin(), operator*(), operator->() ..." << endl;

        // fill tree with some elements
        treemap<int, Payload> m; 
        m[5] = Payload("five");
        m[2] = Payload("two");
        m[0] = Payload("zero");
        m[10] = Payload("ten");
        assert(m.size() == 4); 
        assert(Payload::alive_count() == m.size());

        // get first element via iterator
        treemap<int,Payload>::iterator it = m.begin();
        cout << "first in map is " << it->first << ": " << it->second << endl;
        assert(Payload::alive_count() == m.size());

        assert(( *it == make_pair(0,Payload("zero")) ));
        assert(it->first == 0);
        assert(it->second == Payload("zero"));

    }
    assert(Payload::alive_count() == 0); 
    cout << "done." << endl;


// once you are ready to test the next part, change to #if 1 
#if 1
    {
        cout << "find() und end()..." << endl;

        // fill tree with some elements
        treemap<int, Payload> m; 
        m[5] = Payload("five");
        m[2] = Payload("two");
        m[3] = Payload("three");
        m[7] = Payload("seven");

        auto it3 = m.find(3);
        assert(it3 != m.end());
        assert(it3->first == 3);
        assert(it3->second == Payload("three"));

        auto it5 = m.find(5);
        assert(it5 != m.end());
        assert(( *it5 == make_pair(5, Payload("five")) ));
        
        auto it33 = m.find(33);
        assert(it33 == m.end());

    }
    assert(Payload::alive_count() == 0); 
    cout << "done." << endl;
#endif

#if 1

    {
        cout << "clear(), is iterator non-owning? ..." << endl;

        // fill tree with some elements
        treemap<int, Payload> m; 
        m[5] = Payload("five");
        m[2] = Payload("two");
        m[0] = Payload("zero");
        assert(m.size() == 3); 
        assert(Payload::alive_count() == m.size());

        // get iterator pointing to some element
        auto it = m.begin();
        // should not change Payload count
        assert(Payload::alive_count() == m.size());

        // this should actually delete all nodes
        m.clear(); 
        assert(Payload::alive_count() == 0);

        // now the iterator should point to "no element"
        assert(it == m.end());
    }
    cout << "done." << endl;

#endif

#if 1
    {
        cout << "insert(), insert_or_assign()..." << endl;

        // fill tree with some elements
        treemap<int, Payload> m; 

        auto [it1, was_inserted1] = m.insert(1, Payload("one"));
        assert(was_inserted1); 
        assert(it1 != m.end()); 
        assert(it1->first == 1); 
        assert(it1->second == Payload("one")); 

        auto [it5, was_inserted5] = m.insert(5, Payload("five"));
        assert(was_inserted5); 
        assert(it5->first == 5); 

        // insert() does not overwrite - so existing content should be preserved
        auto [it1x, was_inserted1x] = m.insert(1, Payload("one, do not overwrite"));
        assert(was_inserted1x == false); 
        assert(it1x != m.end()); 
        assert(it1x->first == 1); 
        assert(it1x->second == Payload("one")); 

        // insert_or_assign() overwrites in case the key is already in the map
        auto [it1xx, was_inserted1xx] = m.insert_or_assign(1, Payload("one, overwrite"));
        assert(was_inserted1xx == false); 
        assert(it1xx != m.end()); 
        assert(it1xx->first == 1); 
        assert(it1xx->second == Payload("one, overwrite")); 

    }
    assert(Payload::alive_count() == 0); 
    cout << "done." << endl;
#endif 

#if 1
    {
        cout << "operator++(), range-based for, use with std algorithms" << endl;

        // fill tree with some elements
        treemap<std::string, Payload> m; 
        m["Uranus"] = Payload("Uranus");
        m["Earth"] = Payload("Earth");
        m["Pluto"] = Payload("Pluto");
        m["Mars"] = Payload("Mars");

        cout << "iterating through list in normal order:" << endl;
        for(auto it=m.begin(); it!=m.end(); ++it)
            cout << "  " << it->first << ": " << it->second << endl;

        assert(m.begin()->first == "Earth");
        assert((++m.begin())->first == "Mars");
        assert((++++m.begin())->first == "Pluto");
        assert((++++++m.begin())->first == "Uranus");

        // iterator++, iterator operator*
        cout << "iterating through list using range-based for loop:" << endl;
        for(auto x : m)
            cout << "  " << x.first << ": " << x.second << endl;

        // use some std algorithm on the map just for "fun" - transform
        cout << "using transform() on the elements of the map:" << endl;
        auto transformer = [](const std::pair<std::string, Payload>& v) { 
            auto result = v; result.second.content += " transformed"; return result; };
/*         std::transform(m.begin(), m.end(), m.begin(), transformer);
        for(auto x : m)
            cout << "  " << x.first << ": " << x.second << endl;
 */
    }
    assert(Payload::alive_count() == 0); 
    cout << "done." << endl;
#endif

#if 1
    {
        cout << "deep copy ..." << endl;

        treemap<int, Payload> m;
        m[1] = Payload("one"); 
        m[5] = Payload("five"); 
        m[2] = Payload("two"); 
        m[10] = Payload("ten"); 
        m[0] = Payload("zero"); 
        assert(m.size() == 5);
        assert(Payload::alive_count() == 5);

        // copy it using copy ctor
        auto m2 = m;

        // exactly twice as many objects now?
        assert(Payload::alive_count() == 10);

        // keys and values still intact?
        assert(m2[10] == m[10]);
        assert(m2[0] == m[0]);

        // two independent copies?
        {
            auto old5 = m[5];
            m[5] = Payload("not five");
            assert(m[5] == Payload("not five"));
            assert(m2[5] == old5);
        } 

        // copy assignment, overwrite an existing map object with new content
        m = m2;
        assert(Payload::alive_count() == 10);

        // keys and values still intact?
        assert(m[5] == Payload("five"));

    } // deep copy
    assert(Payload::alive_count() == 0); 
    cout << "done." << endl;

#endif

#if 1
    {
        cout << "operator--()" << endl;

        // fill tree with some elements
        treemap<std::string, Payload> m; 
        m["Uranus"] = Payload("Uranus");
        m["Earth"] = Payload("Earth");
        m["Pluto"] = Payload("Pluto");
        m["Mars"] = Payload("Mars");

        // get last element via iterator
        auto it = m.end();
        assert(it == m.end());
        --it;
        cout << "last in map is " << it->first << ": " << it->second << endl;
        assert(( *it == make_pair(string("Uranus"),Payload("Uranus")) ));

        cout << "iterating through list in reverse order:" << endl;
        for(auto it=m.end(); it!=m.begin(); ) {
            --it; cout << "  " << it->first << ": " << it->second << endl;
        }

    }
    assert(Payload::alive_count() == 0); 
    cout << "done." << endl;
#endif 


}