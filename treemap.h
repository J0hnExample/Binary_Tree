// C++ Aufgabe Treemap V5 part 1 - treemap class, uses treempap_node
// Autor: Hartmut Schirmacher, BHT Berlin

#pragma once

// other includes

#include <memory>
#include <iostream>
#include <utility>
#include <tuple>
#include "treemap_node.h"
#include "treemap_iterator.h"

// forward declarations
/*TODO
insert_ Insert or assign []*/

namespace my
{
    template <typename K, typename T>
    class treemap;
}

template <typename KK, typename TT>
void swap(my::treemap<KK, TT> &lhs, my::treemap<KK, TT> &rhs);

namespace my
{

    /*
     * class treemap<K,T>
     * represents an associative container (dictionary) with unique keys
     * implemented by a binary search tree
     * - no balancing, no remove/erase operations
     * - no separate comparison operators, relies on K::operator==(), K::operator<(), etc.
     */
    template <typename K, typename T>
    class treemap
    {

    public:
        // public type aliases
        using key_type = K;
        using mapped_type = T;
        using value_type = std::pair<K, T>;
        using iterator = my::treemap_iterator<K, T>;

    public:
        // neuen Methode
        iterator begin();
        iterator end();
        iterator end() const;
        iterator find(const K &) const;
        std::pair<iterator, bool> insert(const K &, const T &);
        std::pair<iterator, bool> insert_or_assign(const K &, const T &);

        // construct empty map
        treemap();

        // copy constructor
        treemap(const treemap &other) : root_(copy_rec(other.root_)), count_(other.count_) {}


        // number of keys in map
        size_t size() const;

        // how often is the element contained in the map?
        // (for this type of container, can only return 0 or 1)
        size_t count(const K &) const;

        // random read/write access to value by key
        T &operator[](const K &);

        treemap<K, T> &operator=(treemap rhs);

        // delete all (key,value) pairs in map
        void clear();

        // used for copy&move - declared in global namespace, not in my::
        template <typename KK, typename TT>
        friend void ::swap(treemap<KK, TT> &, treemap<KK, TT> &);

    protected:
        // the node type is only used internally - do not show publicly!
        using node = my::treemap_node<K, T>;    // from treemap_node.h
        using node_ptr = std::shared_ptr<node>; // for passing around pointers to nodes internally (!)
        
        // class attributes
        node_ptr root_;
        size_t count_;

        // add a new (key, value) pait into the tree
        // returns pair, consisting of:
        // - pointer to node containing the (key, value) pair
        // - bool
        //   - true if element was inserted;
        //   - false if key was already in map (will not overwrite existing value)
        std::pair<node_ptr, bool> insert_(const K &, const T &);

        // find element with specific key. returns nullptr if not found.
        node_ptr find_(const K &) const;
    };
    // Neuer Methoden

    template <typename K, typename T>
    typename treemap<K, T>::iterator treemap<K, T>::begin()
    {
        if (root_ == nullptr)
        {
            return end(); // Wenn der Baum leer ist, gib einen leeren Iterator zurück
        }

        node_ptr current = root_->find(root_->value_.first);
        while (current->left_)
        {
            current = current->left_;
        }

        return iterator(current);

    }
    template <typename K, typename T>
    typename treemap<K, T>::iterator treemap<K, T>::end() const
    {
        return iterator(nullptr, root_);
    }

    template <typename K, typename T>
    typename treemap<K, T>::iterator treemap<K, T>::end()
    {
        return iterator(nullptr, root_);
    }

    template <typename K, typename T>
    typename treemap<K, T>::iterator treemap<K, T>::find(const K &key) const
    {
        node_ptr foundNode = find_(key);
        if (foundNode)
        {
            return iterator(foundNode);
        }
        else
        {
            return end(); // Wenn der Schlüssel nicht gefunden wurde, gib end() zurück
        }
    }
    template <typename K, typename T>
    std::pair<typename treemap<K, T>::iterator, bool> treemap<K, T>::insert(const K &key, const T &value)
    {
        auto result = insert_(key, value);

        return {iterator(result.first), result.second};
    }

    template <typename K, typename T>
    std::pair<typename treemap<K, T>::iterator, bool> treemap<K, T>::insert_or_assign(const K &key, const T &value)
    {
        auto result = find_(key);
        if (result) // Wenn der Schlüssel bereits existiert
        {
            result->value_.second = value; // Überschreiben des Wertes
            return std::make_pair(iterator(result), false);
        }
        // Neuer wert einfügen
        auto result2 = insert_(key, value);
        return std::make_pair(iterator(result2.first), result2.second);
    }

    template <typename K, typename T>
    treemap<K, T>::treemap()
        : root_(), count_(0)
    {
    }

    template <typename K, typename T>
    void
    treemap<K, T>::clear()
    {
        root_ = nullptr;
        count_ = 0;
    }

    template <typename K, typename T>
    T &
    treemap<K, T>::operator[](const K &key)
    {

        auto [node, inserted] = insert_(key, T());

        return node->value_.second;
    }

    template <typename K, typename T>
    treemap<K, T> &treemap<K, T>::operator=(treemap rhs)
    {
        swap(*this, rhs);
        return *this;
    }

    // number of elements in map (nodes in tree)
    template <typename K, typename T>
    size_t treemap<K, T>::size() const
    {
        return count_;
    }

    // add a new element into the tree
    // returns:
    // - pointer to element
    // - true if element was inserted; false if key was already in map
    template <typename K, typename T>
    std::pair<typename treemap<K, T>::node_ptr, bool>
    treemap<K, T>::insert_(const K &key, const T &mapped)
    {
        // Zuerst mit find_ überprüfen, ob der Schlüssel bereits vorhanden ist
        node_ptr foundNode = find_(key);

        if (foundNode)
        {
            // Schlüssel ist bereits vorhanden, nichts einfügen
            return std::make_pair(foundNode, false);
        }

        // Schlüssel ist nicht vorhanden, neuer Knoten muss eingefügt werden
        node_ptr newNode = std::make_shared<node>(key, mapped, nullptr); // parent ist zunächst nullptr

        // Jetzt den Ort finden, wo der neue Knoten eingefügt werden soll
        node_ptr parent = nullptr;
        node_ptr current = root_;

        while (current)
        {
            parent = current;
            if (key < current->value_.first)
            {
                current = current->left_;
            }
            else
            {
                current = current->right_;
            }
        }

        newNode->up_ = parent; // Setzen des Parent-Pointers

        if (!parent)
        {
            // Der Baum war leer, neuer Knoten wird Root
            root_ = newNode;
        }
        else if (key < parent->value_.first)
        {
            parent->left_ = newNode;
        }
        else
        {
            parent->right_ = newNode;
        }

        count_++;
        return std::make_pair(newNode, true);
    }
    // find element with specific key. returns end() if not found.
    template <typename K, typename T>
    typename treemap<K, T>::node_ptr
    treemap<K, T>::find_(const K &key) const
    {
        node_ptr current = root_;

        while (current)
        {
            if (key < current->value_.first) //Key kleiner als current key dann links
            {
                current = current->left_;
            }
            else if (key > current->value_.first) //Key groesser als current key dann rechts
            {
                current = current->right_;
            }
            else
            {
                return current; //Schlüssel gefunden
            }
        }
        return nullptr; //Schlüssel nicht gefunden
    }

    // how often is the element contained in the map?
    template <typename K, typename T>
    size_t
    treemap<K, T>::count(const K &key) const
    {
        return find_(key) == nullptr ? 0 : 1;   //
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////+++++++++++Copy++++++++++////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // copy constructor
    template <typename K, typename T>
    static std::shared_ptr<treemap_node<K, T>> copy_rec(const std::shared_ptr<treemap_node<K, T>> &node)
    {
        if (!node) //Wenn kein knoten dann return nullptr
        {
            return nullptr;
        }
        //Kopie eines nodes erstellen
        auto new_node = std::make_shared<treemap_node<K, T>>(node->value_.first, node->value_.second);
        //erstelle rekursiv links vom baum kopieren der nodes 
        new_node->left_ = copy_rec(node->left_);
        if (new_node->left_)
        {
            new_node->left_->up_ = new_node;
        }
        //erstelle rekursiv rechts im baum kopuien der nodes
        new_node->right_ = copy_rec(node->right_);
        if (new_node->right_)
        {
            new_node->right_->up_ = new_node;
        }
        //kopie des neuen knoten zurückgeben
        return new_node;
    }

} // namespace my

// swap contents of two trees
// this is defined in the global namespace, for reasons... (see StackOverflow)
template <typename KK, typename TT>
void swap(my::treemap<KK, TT> &lhs, my::treemap<KK, TT> &rhs)
{
    std::swap(lhs.root_, rhs.root_);
    std::swap(lhs.count_, rhs.count_);
}
