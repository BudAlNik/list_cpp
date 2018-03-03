#ifndef BUDIN_H
#define BUDIN_H

#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

namespace budin {

	template<typename T>
	struct list;

	
	template<typename T>
	void swap(list<T>&, list<T>&);

	template<typename T>
	struct list {
		struct iterator;
	private:
		struct node {
			friend list;
			friend iterator;
			friend void swap<T>(list&, list&);
		private:
			std::vector<iterator*> iterators;
			T value;
			node *prev, *next;
			list<T> *parent;

			node() : prev(NULL), next(NULL) {}

			node(list<T>* _parent) : prev(NULL), next(NULL), parent(_parent) {}

			node(list<T>* _parent, T _value) : value(_value), prev(NULL), next(NULL), parent(_parent) {}

			node(list<T>* _parent, T _value, node* _prev, node* _next) : value(_value), prev(_prev), next(_next), parent(_parent) {}

			~node() {
				for (auto it : iterators) {
					it -> flag = false;
				}
			}

			void erase_iterator(iterator* it) {
				iterators.erase(find(iterators.begin(), iterators.end(), it));
			}

			void insert_iterator(iterator* it) {
				iterators.push_back(it);
			}
		};

		node *first, *last;

	public:
		struct iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
			friend list;

		protected:
			node *link;
			bool flag;

		public:
			iterator() : link(NULL), flag(false) {}

			iterator(node* _link) : link(_link), flag(true) {
				link -> insert_iterator(this);
			}

			iterator(iterator const& other) : link(other.link), flag(other.flag) {
				if (flag) {
					link -> insert_iterator(this);
				}
			}

			~iterator() {
				if (flag) {
					link -> erase_iterator(this);
				}
			}

			T& operator*() {
				assert(flag && link -> next != NULL);
				return link -> value;
			}

			iterator& operator=(iterator const& other) {
				if (flag) {
					link -> erase_iterator(this);
				}
				link = other.link;
				flag = other.flag;
				if (flag) {
					link -> insert_iterator(this);
				}
				return *this;
			}

			iterator& operator++() {
				assert(link -> next != NULL && flag);
				link -> erase_iterator(this);
				link = link -> next;
				link -> insert_iterator(this);
				return *this;
			}

			iterator& operator--() {
				assert(link -> prev != NULL && flag);
				link -> erase_iterator(this);
				link = link -> prev;
				link -> insert_iterator(this);
				return *this;
			}

			iterator operator++(int) {
				assert(link -> next != NULL && flag);
				iterator ret = *this;
				link -> erase_iterator(this);
				link = link -> next;
				link -> insert_iterator(this);
				return ret;
			}

			iterator operator--(int) {
				assert(link -> prev != NULL && flag);
				iterator ret = *this;
				link -> erase_iterator(this);
				link = link -> prev;
				link -> insert_iterator(this);
				return ret;
			}

			
			friend bool operator==(iterator const& it1, iterator const& it2) {
				assert(it1.flag && it2.flag);
				return it1.link == it2.link;
			}

			friend bool operator!=(iterator const& it1, iterator const& it2) {
				assert(it1.flag && it2.flag);
				return it1.link != it2.link;
			}
		};

		struct const_iterator : public iterator {
			friend list;
		public:
			const_iterator() : iterator() {}

			const_iterator(node* _link) : iterator(_link) {}

			const_iterator(iterator const& other) : iterator(other) {}

			T const& operator*() const {
				assert(this->flag && this->link->next != NULL);
				return this->link -> value;
			}
		};

		struct reverse_iterator : public iterator {
			friend list;
		public:
			reverse_iterator() : iterator() {}

			reverse_iterator(node* _link) : iterator(_link) {}

			reverse_iterator(reverse_iterator const& other) : iterator(other) {}

			reverse_iterator& operator++() {
				iterator::operator--();
				return *this;
			}
			reverse_iterator& operator--() {
				iterator::operator++();
				return *this;
			}
			reverse_iterator operator++(int) {
				return iterator::operator--(0);
			}
			reverse_iterator operator--(int) {
				return iterator::operator++(0);
			}
			T& operator*() {
				assert(this->flag && this->link -> prev != NULL);
				return this->link -> prev -> value;
			}
		};

		struct const_reverse_iterator : public reverse_iterator {
			friend list;
		public:
			const_reverse_iterator() : reverse_iterator() {}

			const_reverse_iterator(node* _link) : reverse_iterator(_link) {}

			const_reverse_iterator(reverse_iterator const& other) : reverse_iterator(other) {}

			T const& operator*() const {
				assert(this->flag && this->link -> prev != NULL);
				return this->link -> prev -> value;
			}
		};
		
		list() {
			first = new list<T>::node(this);
			last = first;
		}
		
		list(list<T> const& other) {
			auto el = other.first;
			node* prev = NULL;
			node** prev_next = NULL;
			while (el != NULL) {
				auto cur = new list<T>::node(this, el -> value, prev, NULL);
				if (el == other.first) {
					first = cur;
				}
				if (el == other.last) {
					last = cur;
				}
				if (prev_next != NULL) {
					*prev_next = cur;
				}
				prev_next = &(cur -> next);
				prev = cur;
				el = el -> next;
			}
		}
		
		~list() {
			auto cur = first;
			while (cur != NULL) {
				auto tmp = cur;
				cur = cur -> next;
				delete tmp;
			}
		}

		list& operator=(list<T> const& other) {
			list<T> tmp(other);
			swap(*this, tmp);
			return *this;
		}

		bool empty() const {
			return first == last;
		}

		void clear() {
			while (first != last) {
				auto tmp = first;
				first = first -> next;
				delete tmp;
			}
			first -> prev = NULL;
		}
		
		T& back() {
			assert(!empty());
			return last -> prev -> value;
		}

		T const& back() const {
			assert(!empty());
			return last -> prev -> value;
		}

		void pop_back() {
			assert(!empty());
			auto tmp = last;
			last = last -> prev;
			last -> next = NULL;
			delete tmp;
		}

		void push_back(T const& value) {
			auto tmp = new list<T>::node(this, 0, last, NULL);
			last -> next = tmp;
			last -> value = value;
			last = tmp;
		}
		

		T& front() {
			assert(!empty());
			return first -> value;
		}

		T const& front() const {
			assert(!empty());
			return first -> value;
		}

		void pop_front() {
			assert(!empty());
			auto tmp = first;
			first = first -> next;
			first -> prev = NULL;
			delete tmp;
		}

		void push_front(T const& value) {
			auto tmp = new list<T>::node(this, value, NULL, first);
			first -> prev = tmp;
			first = tmp;
		}
		
		iterator begin() {
			return list<T>::iterator(first);
		}

		iterator end() {
			return list<T>::iterator(last);
		}

		const_iterator begin() const {
			return list<T>::const_iterator(first);
		}

		const_iterator end() const {
			return list<T>::const_iterator(last);
		}

		reverse_iterator rbegin() {
			return list<T>::reverse_iterator(last);
		}

		reverse_iterator rend() {
			return list<T>::reverse_iterator(first);
		}

		const_reverse_iterator rbegin() const {
			return list<T>::const_reverse_iterator(last);
		}

		const_reverse_iterator rend() const {
			return list<T>::const_reverse_iterator(first);
		}

		iterator erase(const_iterator it) {
			assert(it.flag && it.link -> parent == this && it.link -> next != NULL);
			auto l = it.link -> prev;
			auto r = it.link -> next;
			r -> prev = l;
			if (l != NULL) {
				l -> next = r;
			} else {
				first = r;
			}
			
			delete it.link;
			return r;
		}

		iterator insert(const_iterator it, T const& value) {
			assert(it.flag && it.link -> parent == this);
			auto tmp = new list<T>::node(this, value, it.link -> prev, it.link);

			it.link -> prev -> next = tmp;
			it.link -> prev = tmp;
			return tmp;
		}

		friend void swap<T>(list&, list&);
		void splice(const_iterator, list&, const_iterator, const_iterator);
	};

	template<typename T>
	void swap(list<T>& l1, list<T>& l2) {
		std::swap(l1.first, l2.first);
		std::swap(l1.last, l2.last);
		auto tmp = l1.first;
		while (tmp != NULL) {
			tmp->parent = &l1;
			tmp = tmp->next;
		}
		tmp = l2.first;
		while (tmp != NULL) {
			tmp->parent = &l2;
			tmp = tmp-> next;
		}
	}

	template<typename T>
	void list<T>::splice(const_iterator pos, list& other, const_iterator from, const_iterator to) {
		//cerr << (void*) this << " " << (void*) (pos.link -> parent) << endl;
		assert(pos.flag && from.flag && to.flag);
		assert(pos.link -> parent == this && from.link -> parent == &other && to.link -> parent == &other && from.link != to.link);

		auto tmp = from;
		while (tmp != to) {
			tmp.link -> parent = this;
			++tmp;
		}
		auto v1 = pos.link -> prev;
		auto v2 = pos.link;
		auto v3 = from.link -> prev;
		auto v4 = from.link;
		auto v5 = to.link -> prev;
		auto v6 = to.link;
		if (v1 != NULL) {
			v1 -> next = v4;
		} else {
			first = v4;
		}
		v4 -> prev = v1;
		v5 -> next = v2;
		v2 -> prev = v5;
		if (v3 != NULL) {
			v3 -> next = v6;
		} else {
			other.first = v6;
		}
		v6 -> prev = v3;
	}
}

#endif