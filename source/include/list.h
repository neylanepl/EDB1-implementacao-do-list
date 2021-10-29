#ifndef _LIST_H_
#define _LIST_H_

#include <iostream>  // cout, endl
using std::cout;
using std::endl;

#include <iterator>  // bidirectional_iterator_tag
using std::advance;
#include <cassert>   // assert()
#include <algorithm> // copy
using std::copy;
using std::swap;
#include <cstddef>   // std::ptrdiff_t
#include <type_traits>

namespace sc { // linear sequence. Better name: sequence container (same as STL).
    /*!
     * A class representing a biderectional iterator defined over a linked list.
     *
     * \note
     * This iterator only works for traversing elements inside the same list.
     *
     * This class is incomplete and it is provided "as is".
     *
     * \date May, 2nd 2017.
     * \author Selan R. dos Santos
     */

    template < typename T >
    class list
    {
        private:
        //=== the data node.
        struct Node
        {
            T data; // Tipo de informação a ser armazenada no container.
            Node * next;
            Node * prev;

            Node( const T &d=T{} , Node * n=nullptr, Node * p=nullptr )
                : data {d}, next{n}, prev{p}
            { /* empty */ }
        };


        //=== The iterator classes.
        public:

        class const_iterator : public std::bidirectional_iterator_tag 
        {
            //=== Some aliases to help writing a clearer code.
            public:
                using value_type        = T;         //!< The type of the value stored in the list.
                using pointer           = T *;       //!< Pointer to the value.
                using reference         = T &;       //!< reference to the value.
                using const_reference   = const T &; //!< const reference to the value.
                using difference_type   = std::ptrdiff_t;
                using iterator_category = std::bidirectional_iterator_tag;

            private:
                Node * m_ptr; //!< The raw pointer.

            public:
                /**
                 * @brief Construct a new const iterator object.
                 */
                const_iterator( Node * ptr = nullptr ): m_ptr{ptr}{/*empty*/}
                /**
                 * @brief Destroy the const iterator object.
                 */
                ~const_iterator() = default;
                /**
                 * @brief Construct a new const iterator object.
                 */
                const_iterator( const const_iterator & ) = default;
                /**
                 * @brief Assignment operator, assign the left variable the right value.
                 * 
                 * @return const_iterator& the result of the expression.
                 */
                const_iterator& operator=( const const_iterator & ) = default;
                /**
                 * @brief The unary indirection operator dereferences a pointer
                 *
                 * @return a value of the type from which the pointer's type is derived
                 */
                reference  operator*() {
                    return m_ptr->data; 
                }
                /**
                 * @brief The unary indirection operator dereferences a pointer
                 *
                 * @return a value of the type from which the pointer's type is derived
                 */
                const_reference  operator*() const {  
                    return m_ptr->data;
                }
                /**
                 * @brief The operator postfix increment.
                 * 
                 * @return const_iterator The result of the expression.
                 */
                const_iterator operator++() { 
                    m_ptr = m_ptr->next;
                    return const_iterator(*this); 
                }
                /**
                 * @brief The operator prefix increment.
                 * 
                 * @return const_iterator The result of the expression.
                 */
                const_iterator operator++(int) { 
                    const_iterator temp = (*this); 
                    m_ptr = m_ptr->next;
                    return const_iterator(temp); 
                }
                /**
                 * @brief The operator postfix decrement.
                 * 
                 * @return const_iterator The result of the expression.
                 */
                const_iterator operator--() {  
                    m_ptr = m_ptr->prev;
                    return const_iterator(*this); 
                }
                /**
                 * @brief The operator prefix decrement.
                 * 
                 * @return const_iterator The result of the expression.
                 */
                const_iterator operator--(int) {
                    const_iterator temp = (*this); 
                    m_ptr = m_ptr->prev;
                    return const_iterator(temp); 
                }
                /**
                 * @brief The equality operator.
                 * 
                 * @param rhs Variable on the right side of the operation.
                 * @return true Case the equality.
                 * @return false Case otherwise.
                 */
                bool operator==( const const_iterator & rhs ) const { 
                    return m_ptr == rhs.m_ptr; 
                }
                /**
                 * @brief The inequality operator.
                 * 
                 * @param rhs Variable on the right side of the operation.
                 * @return true Case the inequality.
                 * @return false Case otherwise.
                 */
                bool operator!=( const const_iterator & rhs ) const {
                    return !(*this == rhs); 
                }

                //=== Other methods that you might want to implement.
                /**
                 * @brief Binary operators, case it += 3, go walk 3 positions within the container. 
                 * 
                 * @param step Variable on the right side of the operation.
                 * @return const_iterator The result of the expression.
                 */
                const_iterator operator+=( difference_type step) {
                    *this = step + *this;
                    return const_iterator(*this); 
                }
                /**
                 * @brief Binary operators, case it -= 3, go back  3 positions within the container. 
                 * 
                 * @param step Variable on the right side of the operation.
                 * @return const_iterator The result of the expression.
                 */
                const_iterator operator-=(  difference_type step ) {
                    *this = *this - step; 
                    return const_iterator(*this); 
                }
                /**
                 * @brief access the members of the structure using pointers.
                 */
                pointer operator->( void ) const {
                    return &(m_ptr->data); 
                }
                /**
                 * @brief The operation of minus between values ​​of type iterator and iterator.
                 * 
                 * @param rhs Variable on the right side of the operation.
                 * @return difference_type the result of the expression minus.
                 */
                difference_type operator-( const const_iterator & rhs ) const {
                    if (m_ptr > rhs.m_ptr) 
                        return std::distance(rhs.m_ptr, m_ptr);
                    return std::distance(m_ptr, rhs.m_ptr);
                }

                // We need friendship so the list<T> class may access the m_ptr field.
                friend class list<T>;
                friend std::ostream & operator<< ( std::ostream & os_, const const_iterator & s_ ){
                    os_ << "[@"<< s_.m_ptr << ", val = " << s_.m_ptr->data << "]";
                    return os_;
                }
        };


        class iterator : public std::bidirectional_iterator_tag 
        {
            //=== Some aliases to help writing a clearer code.
            public:
                using value_type        = T; //!< The type of the value stored in the list.
                using pointer           = T *; //!< Pointer to the value.
                using reference         = T &; //!< reference to the value.
                using const_reference   = const T &; //!< const reference to the value.
                using difference_type   = std::ptrdiff_t;
                using iterator_category = std::bidirectional_iterator_tag;

            private:
                Node * m_ptr; //!< The raw pointer.

            public:
                /**
                 * @brief Construct a new iterator object.
                 */
                iterator( Node * ptr = nullptr ): m_ptr{ptr} {/*empty*/};
                /**
                 * @brief Destroy the const iterator object.
                 */
                ~iterator() = default;
                /**
                 * @brief Construct a new iterator object.
                 */
                iterator( const iterator & ) = default;
                /**
                 * @brief Assignment operator, assign the left variable the right value.
                 * 
                 * @return iterator& the result of the expression.
                 */
                iterator& operator=( const iterator & ) = default;
                /**
                 * @brief The unary indirection operator dereferences a pointer
                 *
                 * @return a value of the type from which the pointer's type is derived
                 */
                reference  operator*() {
                    return m_ptr->data;
                }
                /**
                 * @brief The unary indirection operator dereferences a pointer
                 *
                 * @return a value of the type from which the pointer's type is derived
                 */
                const_reference  operator*() const { 
                    return m_ptr->data;
                }
                /**
                 * @brief The operator postfix increment.
                 * 
                 * @return iterator The result of the expression.
                 */
                iterator operator++() { 
                    m_ptr = m_ptr->next;
                    return iterator(*this); 
                }
                /**
                 * @brief The operator prefix increment.
                 * 
                 * @return iterator The result of the expression.
                 */
                iterator operator++(int) {
                    iterator temp = (*this); 
                    m_ptr = m_ptr->next;
	                return iterator(temp); 
                }
                /**
                 * @brief The operator postfix decrement.
                 * 
                 * @return iterator The result of the expression.
                 */
                iterator operator--() {
                    m_ptr = m_ptr->prev;
                    return iterator(*this); 
                }
                /**
                 * @brief The operator prefix decrement.
                 * 
                 * @return iterator The result of the expression.
                 */
                iterator operator--(int) {
                    iterator temp = (*this); 
                    m_ptr = m_ptr->prev;
	                return temp; 
                }
                /**
                 * @brief The equality operator.
                 * 
                 * @param rhs Variable on the right side of the operation.
                 * @return true Case the equality.
                 * @return false Case otherwise.
                 */
                bool operator==( const iterator & rhs ) const {
                    return m_ptr == rhs.m_ptr; 
                }
                /**
                 * @brief The inequality operator.
                 * 
                 * @param rhs Variable on the right side of the operation.
                 * @return true Case the inequality.
                 * @return false Case otherwise.
                 */
                bool operator!=( const iterator & rhs ) const {
                    return !(*this == rhs); 
                }

                //=== Other methods that you might want to implement.
                /**
                 * @brief Binary operators, case it += 3, go walk 3 positions within the container. 
                 * 
                 * @param step Variable on the right side of the operation.
                 * @return iterator The result of the expression.
                 */
                iterator operator+=( difference_type step) {
                    *this = step + *this;
                    return iterator(*this); 
                }
                /**
                 * @brief Binary operators, case it -= 3, go back  3 positions within the container. 
                 * 
                 * @param step Variable on the right side of the operation.
                 * @return iterator The result of the expression.
                 */
                iterator operator-=(  difference_type step ) {
                    *this = *this - step;
                    return iterator(*this); 
                }
                /**
                 * @brief access the members of the structure using pointers.
                 */
                pointer operator->( void ) const {
                    return &(m_ptr->data);
                }
                /**
                 * @brief The operation of minus between values ​​of type iterator and iterator.
                 * 
                 * @param rhs Variable of the operation.
                 * @return difference_type the result of the expression minus.
                 */
                difference_type operator-( const iterator & rhs ) const {
                    if (m_ptr > rhs.m_ptr) 
                        return std::distance(rhs.m_ptr, m_ptr);
                    return std::distance(m_ptr, rhs.m_ptr);
                }
                // We need friendship so the list<T> class may access the m_ptr field.
                friend class list<T>;
                friend std::ostream & operator<< ( std::ostream & os_, const iterator & s_ )
                {
                    os_ << "[@"<< s_.m_ptr << ", val = " << s_.m_ptr->data << "]";
                    return os_;
                }
        };


        //=== Private members.
        private:
            size_t m_len;    // comprimento da lista.
            Node * m_head; // nó cabeça.
            Node * m_tail; // nó calda.

        public:
        //=== Public interface

        //=== [I] Special members (8).
        /**
         * @brief Default constructor, constructs an empty container.
         * 
         */
        list(){
            m_head = new Node();
            m_tail = new Node();
            m_head->prev = nullptr;
            m_tail->next = nullptr;	
            m_head->next = m_tail;
            m_tail->prev = m_head;
            m_len = 0;
            /*  Head & tail nodes.
             *     +---+    +---+
             *     |   |--->|   |--+
             *     | H |    | T |  |
             *  +--|   |<---|   |  |
             *  |  +---+    +---+  |
             *  |                  |
             * ===                ===
             *  =                  =
             */
        }
        /**
         * @brief Constructs the container with count default-inserted instances of T. No copies are made.
         * 
         */
        explicit list( size_t count ) : list(){
	        for(size_t i = 0; i < count; ++i){
                insert(end(), T{});   
            }    
        }
        /**
         * @brief Constructs the container with the contents of the range [first, last).
         * 
         * @param first Pointer to the beginning of range.
         * @param last Pointer to the location just past the last valid value of the range.
         */
        template< typename InputIt >
        list( InputIt first, InputIt last ): list() {
            for (InputIt i = first; i != last; i++){
                insert( m_tail, *(i));
            }
        }
        /**
         * @brief Copy constructor. Constructs the container with the copy of the contents of clone_.
         * 
         */
        list( const list & clone_ ) : list(){
            Node* tmp = clone_.m_head->next;
            while (tmp->next != nullptr){
                insert(m_tail, tmp->data);
                tmp = tmp->next;
            }
        }
        /**
         * @brief Constructs the container with the contents of the initializer list ilist_.
         * 
         */
        list( std::initializer_list<T> ilist_ ): list(){
            for (auto i = ilist_.begin(); i != ilist_.end(); i++){
                insert( m_tail, *i);
            }
        }
        /**
         * @brief Destructs the list.
         */
        ~list() { 
            Node* temp;
            this->m_len = 0;
            while (m_head != nullptr)
            {
                temp = m_head;
                m_head = m_head->next;
                delete temp;
	        }
         }
        /**
         * @brief Copy assignment operator. Replaces the contents with a copy of the contents of rhs.
         * 
         * @param rhs Another container to use as data source
         * @return *this
         */
        list & operator=( const list & rhs ){
            clear();
            Node* tmp = rhs.m_head->next;
            while (tmp->next != nullptr){
                insert(m_tail,tmp->data);
                tmp = tmp->next;
            }
            return *this;
        }
        /**
         * @brief Replaces the contents with those identified by initializer list ilist_.
         * 
         * @param ilist_ Initializer list to use as data source
         * @return *this
         */
        list & operator=( std::initializer_list<T> ilist_ ) {
            clear();
            for (auto i = ilist_.begin(); i != ilist_.end(); i++){
                insert( m_tail, *i);
            }
            return *this;
        }


        //=== [II] ITERATORS (4).
        /**
         * @brief Returns an iterator to the first element of the list.
         * 
         * @return Iterator to the first element.
         */
        iterator begin() {
            return iterator(m_head->next);
        }
        /**
         * @brief Returns an constant iterator to the first element of the list.
         * 
         * @return Constant iterator to the first element.
         */
        const_iterator cbegin() const  { 
            return const_iterator(m_head->next); 
        }
        /**
         * @brief Returns an iterator to the element following the last element of the list.
         * 
         * @return Iterator to the element following the last element.
         */
        iterator end() {
            return iterator(m_tail); 
        }
        /**
         * @brief Returns an iterator to the element following the last element of the list.
         * 
         * @return Constant iterator to the element following the last element.
         */
        const_iterator cend() const  { 
            return const_iterator(m_tail); 
        }


        //=== [III] Capacity/Status (2).
        /**
         * @brief Checks if the container has no elements.
         * 
         * @return true if the container is empty
         * @return false otherwise.
         */
        bool empty ( void ) const {
            return m_len == 0; 
        }
        /**
         * @brief Returns the number of elements in the container.
         * 
         * @return The number of elements in the container.
         */
        size_t size(void) const {
            return m_len; 
        }
        
        
        //=== [IV] Modifiers (9).
        /**
         * @brief Erases all elements from the container
         */
        void clear()  {
            if (m_head == nullptr) 
                return;

            while( m_tail->prev != m_head ) {
                pop_front();
            }  
	        m_len = 0;
        }
        /**
         * @brief Returns a reference to the first element in the container.
         * 
         * @return Reference to the first element.
         */
        T front( void ) { 
            return (m_head->next)->data; 
        }
        /**
         * @brief Returns a reference to the first element in the container.
         * 
         * @return Reference to the first element.
         */
        T front( void ) const  { 
            return (m_head->next)->data;
        }
        /**
         * @brief Returns a reference to the last element in the container.
         * 
         * @return Reference to the last element.
         */
        T back( void ) {
            return (m_tail->prev)->data; 
        }
        /**
         * @brief Returns a reference to the last element in the container.
         * 
         * @return Reference to the last element.
         */
        T back( void ) const  { 
            return (m_tail->prev)->data; 
        }
        /**
         * @brief Prepends the given element value to the beginning of the container.
         * 
         * @param value_ The value of the element to prepend.
         */
        void push_front( const T & value_ ) {
            insert(begin(), value_);
        }
        /**
         * @brief Appends the given element value to the end of the container.
         * 
         * @param value_ The value of the element to append.
         */
        void push_back( const T & value_ ) {
            insert(end(), value_);            
        }
        /**
         * @brief Removes the first element of the container. If there are no elements in the container, the behavior is undefined.
         */
        void pop_front( ) {
            Node *tmp = m_head;
            m_head = m_head->next;
            m_head->prev = nullptr;
            delete tmp;
            --m_len;
        }
        /**
         * @brief Removes the last element of the container. If there are no elements in the container, the behavior is undefined.
         */
        void pop_back( ) {
            Node *tmp = m_tail;
            m_tail = m_tail->prev;
            m_tail->next = nullptr;
            delete tmp;
            --m_len;
        }


        //=== [IV-a] MODIFIERS W/ ITERATORS (7).
        /**
         * @brief Replaces the contents with copies of those in the range [first, last).
         * 
         * @param first_ Pointer to the beginning of range.
         * @param last_ Pointer to the location just past the last valid value of the range.
         */
        template < class InItr >
        void assign( InItr first_, InItr last_ ){
            for (InItr i = first_; i != last_; i++){
                insert( m_tail, *(first_));
            }
        }
        /**
         * @brief Replaces the contents with the elements from the initializer list ilist.
         * 
         * @param ilist_ Initializer list to copy the values from.
         */
        void assign( std::initializer_list<T> ilist_ ){
            for (auto i = ilist_.begin(); i != ilist_.end(); i++){
                insert( m_tail, *(ilist_.begin()));
            }
        }
        /*!
         *  Inserts a new value in the list before the iterator 'it'
         *  and returns an iterator to the new node.
         *
         *  \param pos_ An iterator to the position before which we want to insert the new data.
         *  \param value_ The value we want to insert in the list.
         *  \return An iterator to the new element in the list.
         */
        iterator insert( iterator pos_, const T & value_ ){
            Node *newNode = new Node(value_, pos_.m_ptr, pos_.m_ptr->prev);
            pos_.m_ptr->prev->next = newNode;
            pos_.m_ptr->prev = newNode;
            m_len++;
            return iterator(newNode); 
        }
        /**
         * @brief Inserts elements from range [first_, last_) before pos_.
         * 
         * @param pos_ iterator before which the content will be inserted
         * @param first_ Pointer to the beginning of range.
         * @param last_ Pointer to the location just past the last valid value of the range.
         * @return Iterator pointing to the first element inserted, or pos_ if first_==last_.
         */
        template < typename InItr >
        iterator insert( iterator pos_, InItr first_, InItr last_ ) {
            for (InItr i = first_; i != last_; i++){
                insert( pos_, *(i));            
            }
            return pos_;
        }
        /**
         * @brief Inserts elements from initializer list ilist_ before cpos_.
         * 
         * @param cpos_ Iterator before which the content will be inserted
         * @param ilist_ Initializer list to insert the values from
         * @return  Iterator pointing to the first element inserted, or cpos_ if ilist is empty.
         */
        iterator insert( iterator cpos_, std::initializer_list<T> ilist_ ) {
            for (auto i = ilist_.begin(); i != ilist_.end(); i++){
                insert( cpos_, *i);
            }
            return cpos_;
        }
        /*!
         *  Erases the node pointed by 'it_' and returns an iterator
         *  to the node just past the deleted node.
         *
         *  \param it_ The node we wish to delete.
         *  \return An iterator to the node following the deleted node.
         */
        iterator erase( iterator it_ ){
            Node *tmp = it_.m_ptr;
            Node *aux_p = tmp->prev;
            Node *aux_n = tmp->next;
            aux_p->next = aux_n;
            aux_n->prev = aux_p;
            delete tmp;
            m_len--;
            return iterator{aux_n};
        }
        /**
         * @brief Removes the elements in the range [start; end) .
         * 
         * @param start Pointer to the beginning of range.
         * @param end Pointer to the location just past the last valid value of the range.
         * @return Iterator following the last removed element.
         */
        iterator erase( iterator start, iterator end ){
            iterator aux_r, aux_s = start;
            while (start != end) {
                start++;
                aux_r = erase(aux_s);
                aux_s = start;
            }
            return iterator{aux_r};
        }


        //=== [V] UTILITY METHODS (5)
        /**
         * @brief Merges two sorted lists into one. The lists should be sorted into ascending order.
         * 
         * @param other Another container to merge
         */
        void merge( list & other ){
            Node* tmp = other.m_head->next;
            while (tmp->next != nullptr){
                insert(m_tail,tmp->data);
                tmp = tmp->next;
            }
            sort();
            other.clear();
            other.m_len = 0;
            return; 
        }
        /**
         * @brief Transfers elements from one list to another.
         * 
         * @param pos Element before which the content will be inserted
         * @param other Another container to transfer the content from
         */
        void splice( const_iterator pos, list & other ){
            Node* tmp = other.m_head->next;
            while (tmp->next != nullptr){
                Node *newNode = new Node(tmp->data, pos.m_ptr, pos.m_ptr->prev);
                pos.m_ptr->prev->next = newNode;
                pos.m_ptr->prev = newNode;
                m_len++;
            
                tmp = tmp->next;
            }
            other.clear();
            other.m_len = 0;
        }
        /**
         * @brief Reverses the order of the elements in the container.
         */
        void reverse( void ){
            Node* left = m_head;
            Node* right = m_head;
  
            while (right->next != nullptr)
                right = right->next;
            
            while (left != right && left->prev != right) {
                std::swap(left->data, right->data);
                left = left->next;
                right = right->prev;
            }
        }
        /**
         * @brief Removes all consecutive duplicate elements from the container.
         */
        void unique( void ){
            Node* right = m_head->next;
            while (right->next != nullptr) {
                if(right->data == right->next->data)
                    erase(right->next);
                if(right->prev->data == right->data){
                    erase(right->prev);
                }    
                
                right = right->next;
            }
            return; 
        }
        /**
         * @brief Sorts the elements in ascending order.
         */
        void sort( void ){  
            Node* right = m_head->next;
            while (right->next != nullptr) {
                Node* left = right;
                while (left->prev != nullptr){   
                    if(left->data < left->prev->data)
                        std::swap(left->data, left->prev->data);
                    left = left->prev;
                }
                right = right->next;
            } 
        }
    };


    //=== [VI] OPETARORS (2).
    /**
     * @brief Checks if the contents of l1_ and l2_ are equal, that is, they have the same number of
     *        elements and each element in l1_ compares equal with the element in l2_ at the same position.
     * 
     * @param l1_ list whose content is compared with `l2_`.
     * @param l2_ list whose content is compared with `l1_`.
     * @return true if the contents of the list are equal, false otherwise.
     */
    template < typename T >
    inline bool operator==(const sc::list<T> & l1_, const sc::list<T> & l2_ ){ 
        if(l1_.size() != l2_.size()) {
            return false;
        }

        auto it2 = l2_.cbegin();
        for (auto it1 = l1_.cbegin(); it1 != l1_.cend(); it1++) {
            if (*(it1) == *(it2)){}  
            else{return false;}
            it2++;
        }

        return true;
    
    }
    /**
     * @brief Checks if the contents of l1_ and l2_ are not equal, that is, they have the same number of
     *        elements and each element in l1_ compares not equal with the element in l2_ at the same position.
     * 
     * @param l1_ list whose content is compared with `l2_`.
     * @param l2_ list whose content is compared with `l1_`.
     * @return true if the contents of the list are not equal, false otherwise.
     */
    template < typename T >
    inline bool operator!=( const sc::list<T> & l1_, const sc::list<T> & l2_ ){
        if(l1_.size() != l2_.size()) {
            return true;
        }

        auto it2 = l2_.cbegin();
        for (auto it1 = l1_.cbegin(); it1 != l1_.cend(); it1++) {
            if (*(it1) != *(it2)){
                return true;
            }  
            it2++;
        }

        return false;
    }
}
#endif