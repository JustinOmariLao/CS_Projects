// $Id: listmap.tcc,v 1.15 2019-10-30 12:44:53-07 - - $

#include "listmap.h"
#include "debug.h"
#include <iostream>

// listmap::node::node (link*, link*, const value_type&)
template <typename key_t, typename mapped_t, class less_t>
listmap<key_t,mapped_t,less_t>::node::node (node* next, node* prev, const value_type& value):
         link (next, prev), value (value) 
{}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename key_t, typename mapped_t, class less_t>
listmap<key_t, mapped_t, less_t>::~listmap() {
   if (this->size > 0) {
      auto it = begin();
      while (it != end()) {
         this->erase(it);
         it = begin();
      }

      this->back->next = nullptr;
      this->back->prev = nullptr;
      delete back;
   }
}

//
// iterator listmap::insert (const value_type&)
//
template <typename key_t, typename mapped_t, class less_t>
void listmap<key_t,mapped_t,less_t>::insert (const value_type& pair) {
   if (find(pair.first)) //overwrite pair.second if it already exists
      (*it).second = pair.second;

   else {

      if (size == 0) {
         back = new node(nullptr, nullptr, pair); //uses dummy pair
         node *new_node = new node(back, back, pair);
         front = new_node;
         back->prev = new_node;
         back->next = new_node;
         size++;
      }
      else { //append to list in lexographic order
         node *ptr = front;

         while (less(ptr->value.first, pair.first) && ptr != back)
            ptr = ptr->next;

         //inserting based off keys
         if (ptr == back) { //if the largest node in the list
            node *new_node = new node(back, back->prev, pair);
            back->prev->next = new_node;
            back->prev = new_node;
         }
         else if (ptr->prev == back) { //if less than first node in the list
            node *new_node = new node(ptr, back, pair);
            ptr->prev = new_node;
            back->next = new_node;
            front = new_node;
         }

         else { //if somewhere in between
            node *new_node = new node(ptr, ptr->prev, pair);
            ptr->prev->next = new_node;
            ptr->prev = new_node;
         }
         size++;
      }
   }
}

//
// listmap::find(const key_type&)
//
template <typename key_t, typename mapped_t, class less_t>
bool listmap<key_t,mapped_t,less_t>::find (const key_type& that) {
   node* ptr = this->front;
   for (int num = 0; num < size; num++) {
      if ((ptr->value).first == that) {
         it.init_where(ptr);
         return true;
      }

      ptr = ptr->next;
   }

   return false;
}

//
// iterator listmap::erase (iterator position)
//
template <typename key_t, typename mapped_t, class less_t>
void listmap<key_t,mapped_t,less_t>::erase (iterator position) {
   auto begin = this->begin();
   node *temp;

   if (position == begin) { //The node being deleted is in the front of list
      temp = front;
      front = front->next;
      front->prev = back;
      back->next = front;
      temp->next = nullptr;
      temp->prev = nullptr;
      delete temp;
      size--;
   }

   else { //the key being deleted is in the middle somewhere not including anchor
      temp = position.where;
      temp->prev->next = temp->next;
      temp->next->prev = temp->prev;
      temp->next = nullptr;
      temp->prev = nullptr;
      delete temp;
      size--;
   }
}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::iterator.
/////////////////////////////////////////////////////////////////
//

//
// listmap::value_type& listmap::iterator::operator*()
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::value_type&
listmap<key_t,mapped_t,less_t>::iterator::operator*() {

   return where->value;
}

//
// listmap::value_type* listmap::iterator::operator->()
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::value_type*
listmap<key_t,mapped_t,less_t>::iterator::operator->() {
   TRACE ('l', where);
   return &(where->value);
}

//
// listmap::iterator& listmap::iterator::operator++()
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator&
listmap<key_t,mapped_t,less_t>::iterator::operator++() {
  
   where = where->next;

   return *this;
}

//
// listmap::iterator& listmap::iterator::operator--()
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator&
listmap<key_t,mapped_t,less_t>::iterator::operator--() {
   
   if (map->back == where) //causes circling 
   where = where->prev;

   where = where->prev;
   return *this;
}

//
// bool listmap::iterator::operator== (const iterator&)
//
template <typename key_t, typename mapped_t, class less_t>
inline bool listmap<key_t,mapped_t,less_t>::iterator::operator==
            (const iterator& that) const {

   return this->where == that.where;
}

//
// bool listmap::iterator::operator!= (const iterator&)
//
template <typename key_t, typename mapped_t, class less_t>
inline bool listmap<key_t,mapped_t,less_t>::iterator::operator!=
            (const iterator& that) const {
   return this->where != that.where;
}

template <typename key_t, typename mapped_t, class less_t>
void listmap<key_t,mapped_t,less_t>::iterator::init_where(node* input) {
   
   this->where = input;
}

template <typename key_t, typename mapped_t, class less_t>
void listmap<key_t,mapped_t,less_t>::print_list() {

  for (auto it = this->begin(); it != this->end(); ++it)
  {
     cout << *it << endl;
  }
}

template <typename key_t, typename mapped_t, class less_t>
void listmap<key_t,mapped_t,less_t>::print_iterator() {

     cout << *it << endl;
}

////////~PERSONAL FUNCTIONS~////////

template <typename key_t, typename mapped_t, class less_t>
listmap<key_t,mapped_t,less_t> listmap<key_t,mapped_t,less_t>::search_for_keys(const key_type& value) {

   listmap out;
   node* ptr = front;
   for (int num = 0; num < size; num++) {
      if ((ptr->value).second == value)
      out.insert(ptr->value);
   
      ptr = ptr->next;
  }

  return out;

}