#ifndef STYX_LIST_HPP
#define STYX_LIST_HPP

#include <deque>
#include <stdexcept>

#include "styx/element.hpp"

namespace styx
{
    class list_out_of_bounds_exception;
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
        const_iterator begin() const { return elements.begin(); }
        const_iterator end() const { return elements.end(); }
        const_iterator cbegin() const { return elements.cbegin(); }
        const_iterator cend() const { return elements.cend(); }
        reverse_iterator rbegin() { return elements.rbegin(); }
        reverse_iterator rend() { return elements.rend(); }
        const_reverse_iterator rbegin() const { return elements.rbegin(); }
        const_reverse_iterator rend() const { return elements.rend(); }
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
        iterator erase(iterator first, iterator last)
        {
            return elements.erase(first, last);
        }
        void push_back(const element& e) { elements.push_back(e); }
        void pop_back() { elements.pop_back(); }
        void push_front(const element& e) { elements.push_front(e); }
        void pop_front() { elements.pop_front(); }
    };

    // An operation could not be completed because the list is empty (excluding
    // cases where a specific index is requested, when
    // list_out_of_bounds_exception is more appropriate).
    class empty_list_exception : public std::exception
    {
    public:
        const char *what() const noexcept override;
    };
    // An index outside of the list was requested.
    class list_out_of_bounds_exception : public std::exception
    {
    public:
        const char *what() const noexcept override;
    };

    // Get the first element of a list or throw an empty_list_exception.
    element first(list&&);
    // Get the first element of a list or throw an empty_list_exception.
    const element& first(const list&);
    // Get the first element of a list or throw an empty_list_exception.
    element& first(list&);
}

#endif
