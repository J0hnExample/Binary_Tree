// C++ Aufgabe Treemap V5.2 - treemap_iterator, alias treemap::iterator
// Autor: Hartmut Schirmacher, Berliner Hochschule für Technik

#pragma once

// an iterator references a treemap_node, so it must know about it
// please note that the iterator does *not* need to know the treemap itself! (except for the "friend" stateent below)
#include "treemap_node.h"
#include <iostream>
using namespace std;

namespace my
{

    // forward declaration of treemap, just in case you want to keep a pointer to a treemap or such
    template <typename K, typename T>
    class treemap;

    // iterator: references a node within the tree
    template <typename K, typename T>
    class treemap_iterator
    {
    protected:
        // treemap is a friend, can call protected constructor
        // template<typename KK, typename TT>
        friend class treemap<K, T>;
        friend class treemap_node<K, T>;

        // construct iterator referencing a speciic node
        // - only treemap shall be allowed to do so
        // Alias für Knoten
        using node_ptr = std::weak_ptr<treemap_node<K, T>>;

        // Iterator erstellen auf gefundenen Knoten
        treemap_iterator(node_ptr node)
            : node_(node) {}

        // Iterator für ende und root
        treemap_iterator(std::nullptr_t, node_ptr root)
            : node_(), root_(root) {}

        // Iterator für node und root
        /*  treemap_iterator(node_ptr node, node_ptr root)
            : node_(node), root_(root) {}
 */
        // Schwacher zeiger auf die Wurzel

        std::weak_ptr<treemap_node<K, T>> root_;

        // non-owning reference to the actual node
        node_ptr node_;
        node_ptr previous_node_;

    public:
        // type aliases, should be exactly the same as for treemap itself
        using key_type = K;
        using mapped_type = T;
        using value_type = std::pair<K, T>;
        using node = my::treemap_node<K, T>; // from treemap_node.h

        // access data of referenced map element (node)
        value_type &operator*() // Gibt Value zurück
        {
            {
                // Aus weak pointer wird zu shared pointer umgewandelt
                auto locked_node = node_.lock();
                return locked_node->value_;
            }
        }
        value_type *operator->() // Gibt zeiger zurück
        {
            auto locked_node = node_.lock();
            return &(locked_node->value_);
        }

        // two iterators are equal if they point to the same node
        bool operator==(const treemap_iterator &rhs) const
        {

            auto rhs_locked_node = rhs.node_.lock();
            auto locked_node = node_.lock();
            return locked_node == rhs_locked_node;
        }

        bool operator!=(const treemap_iterator &rhs) const
        {
            auto rhs_locked_node = rhs.node_.lock();
            auto locked_node = node_.lock();
            return locked_node != rhs_locked_node;
        }

        // next element in map, pre-increment
        // note: must modify self!
        // Erst rechts dann links und ansonsten nach oben
        // Aktualisierung des WeakPointers auf neuen knoten und gib den Itearator zurück
        treemap_iterator &operator++()
        {
            auto locked_node = node_.lock();
            previous_node_ = node_;

            if (locked_node->right_)
            {
                // Wenn es einen rechten Kindknoten gibt, gehe zum linksten Knoten im rechten Teilbaum
                locked_node = locked_node->right_;
                while (locked_node->left_)
                {
                    locked_node = locked_node->left_;
                }
            }
            else
            {
                // Wenn es keinen rechten Kindknoten gibt, gehe zu einem Knoten, der vom aktuellen Knoten aus erreichbar ist
                auto parent = locked_node->up_.lock();
                while (parent && locked_node == parent->right_)
                {
                    locked_node = parent;
                    parent = parent->up_.lock();
                }
                // Gehe zum übergeordneten Knoten, wenn vorhanden
                locked_node = parent;
            }

            // Aktualisiere den Iteratorknoten
            node_ = locked_node;
            return *this; // Iterator wird zurückgegeben
        }

        // prev element in map, pre-decrement
        // note: must modify self!
        treemap_iterator &operator--()
        {
            auto locked_node = node_.lock(); //make shared pointer from weak pointer

            if (!locked_node)
            {
                auto previous_lock = previous_node_.lock();
                // Wenn der Iterator bei end() ist, finde das größte Element

                if (previous_lock)
                {
                    // Wenn es einen Vorgängerknoten gibt, gehe zu diesem
                    locked_node = previous_lock;
                }
                else
                {
                    // Wenn es keinen Vorgänger gibt, gehe zum Wurzelknoten
                    locked_node = root_.lock();
                }
            }
            else
            {
                if (locked_node->left_)
                {
                    // Wenn es einen linken Kindknoten gibt, gehe zum rechtesten Knoten im linken Teilbaum
                    locked_node = locked_node->left_;

                    while (locked_node->right_)
                    {
                        locked_node = locked_node->right_;
                    }
                }
                else
                {
                    // Wenn es keinen linken Kindknoten gibt, gehe zu einem Knoten, der vom aktuellen Knoten aus erreichbar ist
                    auto parent = locked_node->up_.lock();

                    while (parent && locked_node == parent->left_)
                    {
                        locked_node = parent;
                        parent = parent->up_.lock();
                    }

                    // Gehe zum übergeordneten Knoten, wenn vorhanden
                    locked_node = parent;
                }
            }

            // Aktualisiere den Iteratorknoten
            node_ = locked_node;
            return *this;
        }

    }; // class iterator

} // my::
