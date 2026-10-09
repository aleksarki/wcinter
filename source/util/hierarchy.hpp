#pragma once
#ifndef WCI_SOURCE_UTIL_HIERARCHY_HPP
#define WCI_SOURCE_UTIL_HIERARCHY_HPP

#include <list>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "../defs.hpp"

namespace wci
{
    template<typename T>
    class Hierarchy
    {
    public:
        template<typename T>
        struct Node
        {
            T* object;
            Node<T>* parent;
            std::vector<Node<T>*> children;
        };

        static constexpr Node<T>* nullnode = nullptr;  // denotes an absense of node

    private:
        std::list<Node<T>> storage;
        std::unordered_map<Id, Node<T>*> nodes;  // id -> node
        std::unordered_map<const Node<T>*, Id> ids;    // node -> id
        Node<T>* rootnode;
        Id counter;

        Node<T>* produce(T* object);

    public:
        Hierarchy(T* object);

        /*
         * Get the id of a node. Get wci::npos if no such node exists.
         */
        Id id(const Node<T>* node) const noexcept;
        /*
         * Get the id of a node by its content. Get wci::npos if no such node exists.
         */
        Id id(const T* object) const noexcept;

        /*
         * Get a node by its id. Get wci::Hierarchy<T>::nullnode if no such node exists.
         */
        Node<T>* node(Id id) const noexcept;
        /*
         * Get a node by its content. Get wci::Hierarchy<T>::nullnode if no such node exists.
         */
        Node<T>* node(const T* object) const noexcept;

        /*
         * Get whether the hierarchy has such a node.
         */
        bool has(Id id) const noexcept;
        /*
         * Get whether the hierarchy has such a node.
         */
        bool has(const T* object) const noexcept;

        /*
         * Get the root node of the hierarchy.
         */
        Node<T>* root() const noexcept;

        /*
         * Create a new child node.
         */
        Id add(T* object, Id parent);

        /*
         * Move a node to a new parent.
         */
        void move(Id child, Id parent);

        /*
         * Remove a node by id and all its children.
         */
        void remove(Id id);
    };
}

// template<typename T>
// inline constexpr wci::Hierarchy<T>::Node<T>* nullnode = nullptr;

template<typename T>
inline wci::Hierarchy<T>::Node<T>* wci::Hierarchy<T>::produce(T* object)
{
    storage.emplace_back(object, wci::Hierarchy<T>::nullnode, std::vector<Node<T>*>());
    auto* node = &storage.back();
    auto id = counter++;
    nodes[id] = node;
    ids[node] = id;
    return node;
}

template<typename T>
wci::Hierarchy<T>::Hierarchy(T* object) : counter(0)
{
    rootnode = produce(object);
}

template<typename T>
inline wci::Id wci::Hierarchy<T>::id(const Node<T>* node) const noexcept
{
    auto it = ids.find(node);
    return it == ids.end() ? wci::npos : it->second;
}
template<typename T>
inline wci::Id wci::Hierarchy<T>::id(const T* object) const noexcept
{
    for (const auto& [node, id] : ids)
        if (node->object == object)
            return id;
    return wci::npos;
}

template<typename T>
inline wci::Hierarchy<T>::Node<T>* wci::Hierarchy<T>::node(wci::Id id) const noexcept
{
    if (!nodes.contains(id))
        return wci::Hierarchy<T>::nullnode;
    return nodes.at(id);
}
template<typename T>
inline wci::Hierarchy<T>::Node<T>* wci::Hierarchy<T>::node(const T* object) const noexcept
{
    for (const auto& [node, id] : ids)
        if (node->object == object)
            return node;
    return wci::Hierarchy<T>::nullnode;
}

template<typename T>
inline bool wci::Hierarchy<T>::has(wci::Id id) const noexcept
{
    return nodes.contains(id);
}
template<typename T>
inline bool wci::Hierarchy<T>::has(const T* object) const noexcept
{
    for (const auto& [node, id] : ids)
        if (node->object == object)
            return true;
    return false;
}

template<typename T>
inline wci::Hierarchy<T>::Node<T>* wci::Hierarchy<T>::root() const noexcept
{
    return rootnode;
}

template<typename T>
inline wci::Id wci::Hierarchy<T>::add(T* object, Id parentId)
{
    auto* parentNode = node(parentId);
    if (parentNode == wci::Hierarchy<T>::nullnode)
        throw std::invalid_argument("No such node exists");
    auto* childNode = produce(object);
    childNode->parent = parentNode;
    parentNode->children.push_back(childNode);
    return id(childNode);
}

template<typename T>
inline void wci::Hierarchy<T>::move(wci::Id childId, wci::Id parentId)
{
    if (childId == parentId)
        throw std::invalid_argument("A node cannot be its own parent");
    auto* childNode = node(childId);
    if (childNode == wci::Hierarchy<T>::nullnode)
        throw std::invalid_argument("No such node exists");
    if (childNode == rootnode)
        throw std::invalid_argument("Root node cannot be moved");
    auto* parentNode = node(parentId);
    if (parentNode == wci::Hierarchy<T>::nullnode)
        throw std::invalid_argument("No such node exists");
    for (auto* p = parentNode; p; p = p->parent)
        if (p == childNode)
            throw std::invalid_argument("Cannot create a circular dependency");
    std::erase(childNode->parent->children, childNode);
    childNode->parent = parentNode;
    parentNode->children.push_back(childNode);
}

template<typename T>
inline void wci::Hierarchy<T>::remove(wci::Id id)
{
    auto* node = this->node(id);
    if (node == wci::Hierarchy<T>::nullnode)
        throw std::invalid_argument("No such node exists");
    if (node == rootnode)
        throw std::invalid_argument("Root node cannot be removed");
    for (auto* child : node->children)
        remove(this->id(child));
    ids.erase(node);
    nodes.erase(id);
    storage.remove_if([node](const Node<T>& n) -> bool { return &n == node; });
}

#endif  // WCI_SOURCE_UTIL_HIERARCHY_HPP
