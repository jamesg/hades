#ifndef STYX_LIST_HPP
#define STYX_LIST_HPP

#include <deque>

#include "styx/element.hpp"

namespace styx
{
    /*!
     * \brief Simple list type based on std::deque.  Represents a polymorphic
     * list.
     */
    class list
    {
    public:
        typedef std::deque<element>::size_type size_type;
        typedef std::deque<element>::iterator iterator;
        typedef std::deque<element>::const_iterator const_iterator;
        typedef std::deque<element>::reverse_iterator reverse_iterator;
        typedef std::deque<element>::const_reverse_iterator
            const_reverse_iterator;

        std::deque<element> elements;

        size_type size() const;
        void append(const element& obj);
        /*!
         * \brief Append all the elements in another styx::list to the end of
         * this list.
         */
        void append_list(const element& obj);
        void prepend(const element& o);
        void prepend_list(const element& o);
        template<typename T>
        T& get(size_type k);

        // Iterators.
        iterator begin() { return elements.begin(); }
        iterator end() { return elements.end(); }
        const_iterator cbegin() const { return elements.cbegin(); }
        const_iterator cend() const { return elements.cend(); }
        reverse_iterator rbegin() { return elements.rbegin(); }
        reverse_iterator rend() { return elements.rend(); }
        const_reverse_iterator crbegin() const { return elements.crbegin(); }
        const_reverse_iterator crend() const { return elements.crend(); }

        // Accessors.
        bool empty() { return elements.empty(); }
        element& operator[](int i);
        const styx::element& at(int i) const { return elements.at(i); }
        element& front() { return elements.front(); }
        const element& front() const { return elements.front(); }
        element& back() { return elements.back(); }
        const element& back() const { return elements.back(); }

        // Modifiers.
        void clear() { elements.clear(); }
        iterator erase(iterator first)
        {
            return elements.erase(first);
        }
        //iterator erase(const_iterator first)
        //{
            //return elements.erase(first);
        //}
        iterator erase(iterator first, iterator last)
        {
            return elements.erase(first, last);
        }
        //iterator erase(const_iterator first, const_iterator last)
        //{
            //return elements.erase(first, last);
        //}
        void push_back(const element& e) { elements.push_back(e); }
        void pop_back() { elements.pop_back(); }
        void push_front(const element& e) { elements.push_front(e); }
        void pop_front() { elements.pop_front(); }
    };
}

#endif

