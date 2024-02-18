// Testtreiber für  treemap 


#include <iostream>
#include <string>
#include <cassert>
#include "treemap.h"
#include "payload_v2.h"
#include "test32.cpp"

using namespace std;

using my::treemap;

int main()
{
    
    
    {
        cout << "0. Payload tests..." << endl;

        Payload::do_logging(true);

        // test Payload if Payload works as designed
        assert(Payload::ctor_count() == 0);
        assert(Payload::dtor_count() == 0);
        assert(Payload::alive_count() == 0);

        { 
            // create first Payload
            Payload p1("Hallo!"); 
            assert(Payload::ctor_count() == 1);
            assert(Payload::dtor_count() == 0);
            assert(Payload::alive_count() == 1);
        }
        // the existing Payload is destroyed
        assert(Payload::ctor_count() == 1);
        assert(Payload::dtor_count() == 1);
        assert(Payload::alive_count() == 0);

        // compare two Payloads
        Payload p2("gleich, aber nicht dasselbe"), p3("gleich, aber nicht dasselbe");
        assert(p2 == p3); // gleich!
        assert(p2.id() != p3.id());  // aber nicht dasselbe!

        Payload::do_logging(false);

        cout << "done..." << endl;

    } // Payload tests 
    cout << endl;


    {
        cout << "1. treemap creation..." << endl;

        // create empty treemap
        auto ctor_before = Payload::ctor_count();
        my::treemap<int, Payload> m;
 
        // initially the map must be empty
        assert(m.size() == 0); 

        // no Payload objects created
        assert(Payload::ctor_count() == ctor_before);
        // no Payload objects alive
        assert(Payload::alive_count() == 0);

        cout << "done..." << endl;
    }
    cout << endl;

    {
        cout << "2. write, read, size..." << endl;

        // if map is empty, count should return 0
        treemap<int, Payload> m;
     // write (key, value) pair
        m[1] = Payload("Value1"); 

        assert(m.size() == 1);
        assert(Payload::alive_count() == 1); 

        // read value for given key
        assert(m[1] == Payload("Value1"));

        // write another (key, value) pair
        m[2] = Payload("Value2"); 
        
        assert(m.size() == 2);
        
        assert(Payload::alive_count() == 2); 

        assert(m[1] == Payload("Value1"));
        assert(m[2] == Payload("Value2"));

        cout << "done." << endl;
    }
    assert(Payload::alive_count() == 0);
    cout << endl;

    {
        cout << "3. overwrite, count..." << endl;

        // if map is empty, count should return 0
        treemap<char, Payload> m;

        m['a'] = Payload("old value"); 
        m['a'] = Payload("new value"); 
        assert(m.size() == 1);
        assert(m.count('a') == 1);
        assert(Payload::alive_count() == 1); 
        assert(m['a'] == Payload("new value")); 

        cout << "done." << endl;
    }
    assert(Payload::alive_count() == 0);
    cout << endl;

    {
        cout << "4. count, size, clear..." << endl;

        // if map is empty, count should return 0
        treemap<int, Payload> m;

        m[1] = Payload("eins"); 
        m[2] = Payload("zwei"); 
        m[3] = Payload("drei"); 
        assert(m.size() == 3);
        assert(Payload::alive_count() == 3); 

        m.clear();
        assert(m.size() == 0);
        assert(m.count('1') == 0);
        assert(m.count('2') == 0);
        assert(m.count('3') == 0);
        assert(Payload::alive_count() == 0); 

        cout << "done." << endl;
    }
    assert(Payload::alive_count() == 0);
    cout << endl;

    //test32
    
    test32();
    cout << "seems it works!" << endl;;

    return 0;
}
