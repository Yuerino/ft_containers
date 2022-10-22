#pragma once

#include "utility.hpp"
#include "functional.hpp"
#include "iterator.hpp"

#include <memory>

#ifdef _TREE_DEBUG
#include <iostream>
#define TREE_DEBUG(x) (std::cout << x << std::endl)
#else
#define TREE_DEBUG(x)
#endif

namespace ft {
	template<typename T, typename TreeNode>
	struct binary_tree_node;

	template<typename T, typename TreeNode = binary_tree_node<T, void> >
	struct binary_tree_node {
		typedef T				value_type;
		typedef T*				pointer;
		typedef TreeNode*		node_pointer;
		typedef const TreeNode*	const_node_pointer;

		value_type value;
		node_pointer parent;
		node_pointer left;
		node_pointer right;

		binary_tree_node() : value(), parent(), left(), right() {}

		binary_tree_node(value_type _value, node_pointer _parent) : value(_value), parent(_parent), left(), right() {}

		binary_tree_node(const binary_tree_node& other) : value(other.value), parent(other.parent), left(other.left), right(other.right) {}

		~binary_tree_node() {}

		static node_pointer getMinimum(node_pointer node) {
			if (node == NULL)
				return NULL;
			while (node->left != NULL)
				node = node->left;
			return node;
		}

		static node_pointer getMaximum(node_pointer node) {
			if (node == NULL)
				return NULL;
			while (node->right != NULL)
				node = node->right;
			return node;
		}

		static bool hasChildren(node_pointer node) {
			return (node->left != NULL || node->right != NULL);
		}

		static node_pointer getSuccessor(node_pointer node) {
			if (node->right != NULL)
				return binary_tree_node::getMinimum(node->right);
			else {
				node_pointer parent = node->parent;
				while (parent != NULL && node == parent->right) {
					node = parent;
					parent = parent->parent;
				}
				node = parent;
			}
			return node;
		}

		static node_pointer getPredecessor(node_pointer node) {
			if (node->left != NULL)
				return binary_tree_node::getMaximum(node->left);
			else {
				node_pointer parent = node->parent;
				while (parent != NULL && node == parent->left) {
					node = parent;
					parent = parent->parent;
				}
				node = parent;
			}
			return node;
		}

		static bool swapNodeValue(node_pointer node1, node_pointer node2) {
			node_pointer parent1 = node1->parent;
			node_pointer node1Left = node1->left;
			node_pointer node1Right = node1->right;
			node_pointer parent2 = node2->parent;
			node_pointer node2Left = node2->left;
			node_pointer node2Right = node2->right;

			if (parent1 != NULL && parent1->left == node1)
				parent1->left = node2;
			else if (parent1 != NULL)
				parent1->right = node2;
			node2->parent = parent1;

			if (parent2 != NULL && parent2->left == node2)
				parent2->left = node1;
			else if (parent2 != NULL)
				parent2->right = node1;
			if (parent2 != node1)
				node1->parent = parent2;
			else
				node1->parent = node2;

			if (node1Left == node2) {
				node2->left = node1;
				node2->right = node1Right;
			} else if (node1Right == node2) {
				node2->right = node1;
				node2->left = node1Left;
			} else {
				node2->left = node1Left;
				node2->right = node1Right;
			}

			if (node1Right && node1Right != node2)
				node1Right->parent = node2;
			if (node1Left && node1Left != node2)
				node1Left->parent = node2;

			node1->left = node2Left;
			if (node2Left)
				node2Left->parent = node1;
			node1->right = node2Right;
			if (node2Right)
				node2Right->parent = node1;

			if (parent1 == NULL)
				return false;
			return true;
		}
	};

	enum rb_tree_color { RED, BLACK };

	template<typename T>
	struct rb_tree_node : public binary_tree_node<T, rb_tree_node<T> > {
		typedef typename binary_tree_node<T, rb_tree_node>::value_type		value_type;
		typedef typename binary_tree_node<T, rb_tree_node>::pointer			pointer;
		typedef typename binary_tree_node<T, rb_tree_node>::node_pointer	node_pointer;

		rb_tree_color color;

		rb_tree_node() : binary_tree_node<T, rb_tree_node>() {}

		rb_tree_node(value_type _value, node_pointer _parent) : binary_tree_node<T, rb_tree_node>(_value, _parent) {
			if (this->parent == NULL)
				this->color = BLACK;
			else
				this->color = RED;
		}

		rb_tree_node(const rb_tree_node& other) : binary_tree_node<T, rb_tree_node>(other), color(other.color) {}

		~rb_tree_node() {}

		static void swapNodeColor(node_pointer node1, node_pointer node2) {
			rb_tree_color tmp = (node1 != NULL) ? node1->color : BLACK;
			if (node1)
				node1->color = (node2 != NULL) ? node2->color : BLACK;
			if (node2)
				node2->color = tmp;
		}

		static bool swapNodeValue(node_pointer node1, node_pointer node2) {
			rb_tree_node::swapNodeColor(node1, node2);
			return binary_tree_node<T, rb_tree_node>::swapNodeValue(node1, node2);
		}

		static bool isBlack(node_pointer node) {
			return (node == NULL || node->color == BLACK);
		}
	};

	template<typename T,
			typename Compare = ft::less<T>,
			typename Allocator = std::allocator<T> >
	class red_black_tree {
	public:
		typedef ft::rb_tree_node<T>								node_type;
		typedef T												value_type;
		typedef Compare											compare_type;
		typedef Allocator										allocator_type;
		typedef typename Allocator::reference					reference;
		typedef typename Allocator::const_reference				const_reference;
		typedef typename Allocator::pointer						pointer;
		typedef typename Allocator::const_pointer				const_pointer;
		typedef ft::bst_iterator<pointer, node_type>			iterator;
		typedef ft::bst_iterator<const_pointer, node_type>		const_iterator;
		typedef ft::bd_reverse_iterator<iterator>				reverse_iterator;
		typedef ft::bd_reverse_iterator<const_iterator>			const_reverse_iterator;
		typedef std::ptrdiff_t									difference_type;
		typedef	std::size_t										size_type;

		typedef typename allocator_type::template rebind<node_type>::other	node_allocator_type;
		typedef typename node_type::node_pointer							node_pointer;
		typedef typename node_type::const_node_pointer						const_node_pointer;

	public:
		// construct/copy/destroy

		explicit red_black_tree(const compare_type& comp = compare_type(), const allocator_type& alloc = allocator_type()) :
			_comp(comp),
			_allocator(alloc),
			_node_allocator(node_allocator_type()),
			_size(0),
			_root(0) {
			TREE_DEBUG("red_black_tree default iterator called");
		}

		template<typename InputIterator>
		red_black_tree(InputIterator first, InputIterator last, const compare_type& comp = compare_type(), const allocator_type& alloc = allocator_type(), typename ft::iterator_traits<InputIterator>::iterator_category* = 0) :
			_comp(comp),
			_allocator(alloc),
			_node_allocator(node_allocator_type()),
			_size(0),
			_root(0) {
			TREE_DEBUG("red_black_tree range constructor called");
			this->insert(first, last);
		}

		red_black_tree(const red_black_tree& copy) :
			_comp(copy._comp),
			_allocator(copy.get_allocator()),
			_node_allocator(copy.get_node_allocator()),
			_size(0),
			_root(0) {
			TREE_DEBUG("red_black_tree copy constructor called");
			this->operator=(copy);
		};

		~red_black_tree() {
			TREE_DEBUG("red_black_tree deconstructor called");
			this->deleteAll();
		}

		red_black_tree& operator=(const red_black_tree& other) {
			TREE_DEBUG("red_black_tree assignment operator called");
			if (this == &other) return *this;
			this->deleteAll();
			this->insert(other.begin(), other.end());
			return *this;
		}

		// iterators

		iterator begin() {
			return iterator(node_type::getMinimum(this->_root), node_type::getMaximum(this->_root));
		}

		const_iterator begin() const {
			return const_iterator(node_type::getMinimum(this->_root), node_type::getMaximum(this->_root));
		}

		iterator end() {
			return iterator(NULL, node_type::getMaximum(this->_root));
		}

		const_iterator end() const {
			return const_iterator(NULL, node_type::getMaximum(this->_root));
		}

		reverse_iterator rbegin() {
			return reverse_iterator(iterator(NULL, node_type::getMaximum(this->_root), true));
		}

		const_reverse_iterator rbegin() const {
			return const_reverse_iterator(iterator(NULL, node_type::getMaximum(this->_root), true));
		}

		reverse_iterator rend() {
			return reverse_iterator(iterator(node_type::getMinimum(this->_root), node_type::getMaximum(this->_root), true));
		}

		const_reverse_iterator rend() const {
			return const_reverse_iterator(iterator(node_type::getMinimum(this->_root), node_type::getMaximum(this->_root), true));
		}

		// capacity

		bool empty() const {
			return this->_size == 0;
		}

		size_type size() const {
			return this->_size;
		}

		size_type max_size() const {
			return this->_node_allocator.max_size();
		}

		// modifiers

		ft::pair<iterator, bool> insert(const value_type& val) {
			if (this->empty())
				return ft::make_pair<iterator, bool>(this->insertRoot(val), true);
			node_pointer tmp = this->_root;
			node_pointer parent = NULL;
			while (tmp != NULL) {
				parent = tmp;
				if (this->_comp(val, parent->value))
					tmp = tmp->left;
				else if (this->_comp(parent->value, val))
					tmp = tmp->right;
				else
					return ft::make_pair<iterator, bool>(iterator(tmp, node_type::getMaximum(this->_root)), false);
			}
			tmp = this->addNewNode(val, parent);
			if (this->_comp(val, parent->value))
				parent->left = tmp;
			else
				parent->right = tmp;
			this->balanceOnInsert(tmp);
			return ft::make_pair<iterator, bool>(iterator(tmp, node_type::getMaximum(this->_root)), true);
		}

		template<typename InputIterator>
		void insert(InputIterator first, InputIterator last, typename ft::iterator_traits<InputIterator>::iterator_category* = 0) {
			for (; first != last; ++first)
				this->insert(*first);
		}

		void erase(iterator position) {
			this->destroyNode(position.base());
		}

		size_type erase(const value_type& val) {
			node_pointer to_delete = this->getNode(val);
			if (to_delete == NULL)
				return 0;
			this->destroyNode(to_delete);
			return 1;
		}

		void erase(iterator first, iterator last) {
			while (first != last)
				this->destroyNode((first++).base());
		}

		void swap(red_black_tree& other) {
			if (this == &other) return;
			std::swap(this->_root, other._root);
			std::swap(this->_size, other._size);
		}

		void clear() {
			this->deleteAll();
		}

		iterator find(const value_type& val) {
			return iterator(this->getNode(val), node_type::getMaximum(this->_root));
		}

		const_iterator find(const value_type& val) const {
			return const_iterator(this->getNode(val), node_type::getMaximum(this->_root));
		}

		// allocator

		allocator_type get_allocator() const {
			return allocator_type(this->_allocator);
		}

		node_allocator_type get_node_allocator() const {
			return node_allocator_type(this->_node_allocator);
		}

		// draw tree
	#ifdef _TREE_DEBUG
		void printNode(const std::string& prefix, node_pointer node, bool isLeft) const {
			if (node != NULL)
			{
				std::cout << prefix;
				std::cout << (isLeft ? "├──" : "└──" );
				std::cout << ((node->color == RED) ? "\033[1;31m" : "") << node->value.first << "\033[0m\n";
				this->printNode(prefix + (isLeft ? "│   " : "    "), node->right, true);
				this->printNode(prefix + (isLeft ? "│   " : "    "), node->left, false);
			}
		}

		void printTree() const {
			this->printNode("", this->_root, false);
		}
	#endif

	private:
		/**
		 * 1: Root always as black (handled in insertRoot)
		 * 2: New node as red (handled in add new node)
		 * 3: Parent of new node is black then no need to balance
		 * 4: Parent of new node is red then:
		 * 	a) sibling is black or null then rotate and recolor (handled in rotateAndRecolor)
		 * 	b) sibling is red then recolor parent + sibling + grand parent (if not root)
		 * 	   and recheck with grane parent as start node
		 */
		void balanceOnInsert(node_pointer new_node) {
			if (node_type::isBlack(new_node->parent))
				return;
			node_pointer parent = new_node->parent;
			node_pointer grand_parent = parent->parent;
			node_pointer parent_sibling = grand_parent->left == parent ? grand_parent->right : grand_parent->left;
			if (node_type::isBlack(parent_sibling))
				this->rotateAndRecolor(new_node);
			else {
				parent_sibling->color = parent->color = BLACK;
				if (grand_parent->parent != NULL) {
					grand_parent->color = RED;
					this->balanceOnInsert(grand_parent);
				}
			}
		}

		void rotateAndRecolor(node_pointer new_node) {
			node_pointer parent = new_node->parent;
			node_pointer grand_parent = parent->parent;
			if (grand_parent->left == parent) {
				if (parent->right == new_node) {
					new_node = parent;
					rotateNodeLeft(new_node);
				}
				new_node->parent->color = BLACK;
				grand_parent->color = RED;
				rotateNodeRight(grand_parent);
			} else {
				if (parent->left == new_node) {
					new_node = parent;
					rotateNodeRight(new_node);
				}
				new_node->parent->color = BLACK;
				grand_parent->color = RED;
				rotateNodeLeft(grand_parent);
			}
		}

		void rotateNodeLeft(node_pointer parent) {
			node_pointer right = parent->right;
			parent->right = right->left;
			if (right->left != NULL)
				right->left->parent = parent;
			right->parent = parent->parent;
			if (parent == this->_root)
				this->_root = right;
			else if (parent == parent->parent->right)
				parent->parent->right = right;
			else
				parent->parent->left = right;
			right->left = parent;
			parent->parent = right;
		}

		void rotateNodeRight(node_pointer parent) {
			node_pointer left = parent->left;
			parent->left = left->right;
			if (left->right != NULL)
				left->right->parent = parent;
			left->parent = parent->parent;
			if (parent == this->_root)
				this->_root = left;
			else if (parent == parent->parent->right)
				parent->parent->right = left;
			else
				parent->parent->left = left;
			left->right = parent;
			parent->parent = left;
		}

		node_pointer addNewNode(const value_type& val, node_pointer parent_node) {
			node_pointer node = _node_allocator.allocate(1);
			_node_allocator.construct(node, node_type(val, parent_node));
			this->_size++;
			return node;
		}

		iterator insertRoot(const value_type& val) {
			this->_root = addNewNode(val, NULL);;
			return iterator(this->_root, this->_root);
		}

		void destroyNode(node_pointer node) {
			if (node == NULL)
				return;
			if (!node_type::hasChildren(node) && !node_type::isBlack(node)) {
				this->deleteNode(node);
			} else if (!node_type::hasChildren(node) && node_type::isBlack(node)) {
				this->resolveDoubleBlack(node);
				this->deleteNode(node);
			} else {
				node_pointer next = node->left != NULL ? node_type::getPredecessor(node) : node_type::getSuccessor(node);
				if (!node_type::swapNodeValue(node, next))
					this->_root = next;
				this->destroyNode(node);
			}
		}

		void deleteNode(node_pointer node) {
			if (node == NULL)
				return;
			node_pointer parent = node->parent;
			if (parent == NULL)
				this->_root = NULL;
			else if (parent->left == node)
				parent->left = NULL;
			else if (parent->right == node)
				parent->right = NULL;
			_node_allocator.destroy(node);
			_node_allocator.deallocate(node, 1);
			this->_size--;
		}

		void resolveDoubleBlack(node_pointer node) {
			if (node == this->_root)
				return;
			node_pointer parent = node->parent;
			node_pointer sibling = parent->left == node ? parent->right : parent->left;
			if (!node_type::isBlack(sibling)) {
				node_type::swapNodeColor(parent, sibling);
				if (parent->left == node)
					this->rotateNodeLeft(parent);
				else
					this->rotateNodeRight(parent);
				return this->resolveDoubleBlack(node);
			} else {
				if (sibling == NULL || (node_type::isBlack(sibling->left) && node_type::isBlack(sibling->right))) {
					if (sibling != NULL)
						sibling->color = RED;
					if (!node_type::isBlack(parent))
						parent->color = BLACK;
					else
						return this->resolveDoubleBlack(parent);
				} else if ((parent->left == node && !node_type::isBlack(sibling->left) && node_type::isBlack(sibling->right))
						|| (parent->right == node && !node_type::isBlack(sibling->right) && node_type::isBlack(sibling->left))) {
					if (parent->left == node) {
						node_type::swapNodeColor(sibling, sibling->left);
						this->rotateNodeRight(sibling);
					} else {
						node_type::swapNodeColor(sibling, sibling->right);
						this->rotateNodeLeft(sibling);
					}
					return this->resolveDoubleBlack(node);
				} else if ((parent->left == node && !node_type::isBlack(sibling->right))
						|| (parent->right == node && !node_type::isBlack(sibling->left))) {
					if (parent->left == node) {
						node_type::swapNodeColor(sibling, parent);
						this->rotateNodeLeft(parent);
						sibling->right->color = BLACK;
					} else {
						node_type::swapNodeColor(sibling, parent);
						this->rotateNodeRight(parent);
						sibling->left->color = BLACK;
					}
				}
			}
		}

		void deleteAll(node_pointer node = NULL) {
			if (node == NULL)
				node = this->_root;
			if (node == NULL)
				return;
			if (node->left != NULL)
				this->deleteAll(node->left);
			if (node->right != NULL)
				this->deleteAll(node->right);
			this->deleteNode(node);
		}

		node_pointer getNode(const value_type& val) const {
			node_pointer tmp = this->_root;
			while (tmp != NULL) {
				if (this->_comp(val, tmp->value))
					tmp = tmp->left;
				else if (this->_comp(tmp->value, val))
					tmp = tmp->right;
				else
					return tmp;
			}
			return NULL;
		}

	protected:
		compare_type _comp;
		allocator_type _allocator;
		node_allocator_type _node_allocator;
		size_type _size;
		node_pointer _root;
	};
}
