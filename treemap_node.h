// C++ Aufgabe Treemap V5 part 1 - treemap_node, alias treemap::node
// Autor: Hartmut Schirmacher, BHT Berlin

#pragma once

#include <memory>

namespace my
{
    using namespace std;
    // this is the template for a node in the treemap's tree
    // please note that the node does not need to know anything about the treemap itself (or about the iterator, later)
    template <typename K, typename T>
    class treemap_node : public std::enable_shared_from_this<treemap_node<K, T>>
    {

    public:
        // aloas to shorten things
        using node = treemap_node<K, T>;
        using node_ptr = std::shared_ptr<node>;

        // public attributes
        std::pair<K, T> value_;
        std::weak_ptr<node> up_;
        node_ptr left_, right_;

        // treemap_node(K key, T mapped, node_ptr up = nullptr)
        // Todo nullptr fix

        treemap_node(K key, T mapped, node_ptr up = nullptr)
            : value_(std::make_pair(key, mapped)), up_(up), left_(), right_()
        {
        }

        // try to insert new (key,mapped) node in tree, return (new node, true)
        // if key already in tree, do not overwrite, just return (existing node, false)

       /*  std::pair<node_ptr, bool> insert(K key, T mapped)
        {
            if (key < this->value_.first)   //key kleiner <- linkse Seite
            {
                if (!this->left_)   //links kein kind dann erstellen
                {
                    this->left_ = std::make_shared<treemap_node<K, T>>(key, mapped, this->shared_from_this());
                    return std::make_pair(this->left_, true);
                }
                else    //wenn links kind dann rekursiv aufrufen (nochmal gucken)
                {
                    return this->left_->insert(key, mapped);
                }
            }
            else if (key > this->value_.first) //key groesser -> rechte Seite
            {
                if (!this->right_)   //rechts kein kind dann erstellen
                {
                    this->right_ = std::make_shared<treemap_node<K, T>>(key, mapped, this->shared_from_this());
                    return std::make_pair(this->right_, true);
                }
                else    //wenn rechts kind dann rekursiv aufrufen (nochmal gucken) vielleicht dann links
                {
                    return this->right_->insert(key, mapped);
                }
            }
            else
            {
                return std::make_pair(this->shared_from_this(), false); // Schlüssel bereits vorhanden
            }
        }
 */
        // find node with the given key, and return pointer to it. return nullptr if not found
        std::shared_ptr<node> find(K key)
        {
            if (key < this->value_.first)
            {
                return this->left_ ? this->left_->find(key) : nullptr;
            }
            else if (key > this->value_.first)
            {
                return this->right_ ? this->right_->find(key) : nullptr;
            }
            else
            {
                return this->shared_from_this();
            }
        }

    }; // class treemap_node

} // my::