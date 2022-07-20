#pragma once

#include "utility.hpp"
#include "functional.hpp"
#include "iterator.hpp"

#include <memory>

#ifdef _TREE_DEBUG
#include <iostream>
#define DEBUG(x) (std::cout << x << std::endl)
#else
#define DEBUG(x)
#endif

namespace ft {
	template<typename T, typename TreeNode>
	struct binary_tree_node;

	template<typename T, typename TreeNode = binary_tree_node<T, void> >
	struct binary_tree_node {
		typedef T				value_type;
		typedef TreeNode*		pointer;
		typedef const TreeNode*	const_pointer;

		value_type value;
		pointer parent;
		pointer left;
		pointer right;

		static pointer getMinimum(pointer node) {
			while (node->left)
				node = node->left;
			return node;
		}

		static pointer getMaximum(pointer node) {
			while (node->right)
				node = node->right;
			return node;
		}

		static bool hasChildren(pointer node) {
			if (node->left || node->right)
				return true;
			return false;
		}

		static pointer getSuccessor(pointer node) {
			if (node->right != NULL)
				return binary_tree_node::getMinimum(node->right);
			else {
				pointer parent = node->parent;
				while (parent != NULL && node == parent->right) {
					node = parent;
					parent = parent->parent;
				}
				node = parent;
			}
			return node;
		}

		static pointer getPredecessor(pointer node) {
			if (node->left != NULL)
				return binary_tree_node::getMaximum(node->left);
			else {
				pointer parent = node->parent;
				while (parent != NULL && node == parent->left) {
					node = parent;
					parent = parent->parent;
				}
				node = parent;
			}
			return node;
		}

		static void swapNodeValue(pointer node1, pointer node2) {
			value_type tmp = node1->value;
			node1->value = node2->value;
			node2->value = tmp;
		}
	};

	template<typename T,
			typename Compare = ft::less<T>,
			typename Allocator = std::allocator<T> >
	class red_black_tree {
	protected:
		enum rb_tree_color { RED, BLACK };

		struct rb_tree_node : public binary_tree_node<T, rb_tree_node> {
			rb_tree_color color;

			static void swapNodeColor(typename rb_tree_node::pointer node1, typename rb_tree_node::pointer node2) {
				rb_tree_color tmp = node1->color;
				node1->color = node2->color;
				node2->color = tmp;
			}

			static bool isBlack(typename rb_tree_node::pointer node) {
				if (node == NULL || node->color == BLACK)
					return true;
				return false;
			}
		};

	public:
		typedef T												value_type;
		typedef Compare											compare_type;
		typedef Allocator										allocator_type;
		typedef typename Allocator::reference					reference;
		typedef typename Allocator::const_reference				const_reference;
		typedef typename Allocator::pointer						pointer;
		typedef typename Allocator::const_pointer				const_pointer;
		typedef ft::bst_iterator<pointer, red_black_tree>		iterator;
		typedef ft::bst_iterator<const_pointer, red_black_tree>	const_iterator;
		typedef ft::reverse_iterator<iterator>					reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>			const_reverse_iterator;
		typedef std::ptrdiff_t									difference_type;
		typedef	std::size_t										size_type;

		typedef rb_tree_node												node_type;
		typedef typename allocator_type::template rebind<node_type>::other	node_allocator_type;
		typedef typename node_type::pointer									node_pointer;
		typedef typename node_type::const_pointer							const_node_pointer;

	public:
		// construct/copy/destroy

		explicit red_black_tree(const compare_type& comp = compare_type(), const allocator_type& alloc = allocator_type()) :
			_comp(comp),
			_allocator(alloc),
			_node_allocator(node_allocator_type()),
			_size(0),
			_root(0) {
			DEBUG("red_black_tree default iterator called");
		}

		template<typename InputIterator>
		red_black_tree(InputIterator first, InputIterator last, const compare_type& comp = compare_type(), const allocator_type& alloc = allocator_type(), typename ft::iterator_traits<InputIterator>::iterator_catergory* = 0) :
			_comp(comp),
			_allocator(alloc),
			_node_allocator(node_allocator_type()),
			_size(0),
			_root(0) {
			DEBUG("red_black_tree range constructor called");
			this->insert(first, last);
		}

		red_black_tree(const red_black_tree& copy) :
			_comp(copy._comp),
			_allocator(copy.get_allocator()),
			_node_allocator(copy.get_node_allocator()),
			_size(0),
			_root(0) {
			DEBUG("red_black_tree copy constructor called");
			this->operator=(copy);
		};

		~red_black_tree() {
			DEBUG("red_black_tree deconstructor called");
			this->deleteAll();
		}

		red_black_tree& operator=(const red_black_tree& other) {
			DEBUG("red_black_tree assignment operator called");
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

		iterator rbegin() {
			return reverse_iterator(iterator(node_type::getMaximum(this->_root), node_type::getMinimum(this->_root)));
		}

		const_iterator rbegin() const {
			return const_reverse_iterator(const_iterator(node_type::getMaximum(this->_root), node_type::getMinimum(this->_root)));
		}

		iterator rend() {
			return reverse_iterator(iterator(NULL, node_type::getMinimum(this->_root)));
		}

		const_iterator rend() const {
			return const_reverse_iterator(const_iterator(NULL, node_type::getMinimum(this->_root)));
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
			else if (this->_comp(parent->value, val))
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
			this->destroyNode(position.address());
		}

		size_type erase(const value_type& val) {
			node_pointer to_delete = this->getNode(val);
			if (to_delete == NULL)
				return 0;
			this->destroyNode(to_delete);
			return 1;
		}

		void erase(iterator first, iterator last) {
			for (; first != last; ++first)
				this->destroyNode(first.address());
		}

		void clear() {
			this->destroyAll();
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
		void printNode(const std::string& prefix, node_pointer node, bool isLeft)
		{
			if (node != NULL)
			{
				std::cout << prefix;
				std::cout << (isLeft ? "├──" : "└──" );
				std::cout << ((node->color == RED) ? "\033[1;31m" : "") << node->value << "\033[0m\n";
				this->printNode(prefix + (isLeft ? "│   " : "    "), node->right, true);
				this->printNode(prefix + (isLeft ? "│   " : "    "), node->left, false);
			}
		}

		void printTree() {
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
			node_pointer parent_sibling;
			if (grand_parent->left == parent)
				parent_sibling = grand_parent->right;
			else
				parent_sibling = grand_parent->left;
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
				this->_root = parent;
			else if (parent == parent->parent->right)
				parent->parent->right = left;
			else
				parent->parent->left = left;
			left->right = parent;
			parent->parent = left;
		}

		node_pointer addNewNode(const value_type& val, node_pointer parent_node) {
			node_pointer node = _node_allocator.allocate(1);
			_allocator.construct(&node->value, val);
			node->left = NULL;
			node->right = NULL;
			node->parent = parent_node;
			if (parent_node == NULL)
				node->color = BLACK;
			else
				node->color = RED;
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
			if (!node_type::hasChildren(node) && !node_type::isBlack(node))
				this->deleteNode(node);
			else if (!node_type::hasChildren(node) && node_type::isBlack(node)) {
				this->resolveDoubleBlack(node);
				this->deleteNode(node);
			} else {
				node_pointer next;
				if (node->left != NULL)
					next = node_type::getPredecessor(node);
				else
					next = node_type::getSuccessor(node);
				node_type::swapNodeValue(node, next);
				this->destroyNode(next);
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
			_allocator.destroy(&node->value);
			_node_allocator.destroy(node);
			_node_allocator.deallocate(node, 1);
			this->_size--;
		}

		void resolveDoubleBlack(node_pointer node) {
			if (node == this->_root)
				return;
			node_pointer parent = node->parent;
			node_pointer sibling;
			if (parent->left == node)
				sibling = parent->right;
			else
				sibling = parent->left;
			if (!node_type::isBlack(sibling)) {
				node_type::swapNodeColor(parent, sibling);
				if (parent->left == node)
					this->rotateNodeLeft(parent);
				else
					this->rotateNodeRight(parent);
				return this->resolveDoubleBlack(node);
			} else {
				if (node_type::isBlack(sibling->left) && node_type::isBlack(sibling->right)) {
					sibling->color = RED;
					if (!node_type::isBlack(parent))
						parent->color = BLACK;
					else
						return this->resolveDoubleBlack(parent);
				} else if ((parent->left == node && !node_type::isBlack(sibling->left) && node_type::isBlack(sibling->right))
						|| (parent->right == node && !node_type::isBlack(sibling->right) && node_type::isBlack(sibling->left))) {
					if (parent->left == node && !node_type::isBlack(sibling->left)) {
						node_type::swapNodeColor(sibling, sibling->left);
						this->rotateNodeRight(sibling);
					} else {
						node_type::swapNodeColor(sibling, sibling->right);
						this->rotateNodeLeft(sibling);
					}
					return this->resolveDoubleBlack(node);
				} else if ((parent->left == node && !node_type::isBlack(sibling->right))
						|| (parent->right == node && !node_type::isBlack(sibling->left))) {
					if (parent->left == node && !node_type::isBlack(sibling->right)) {
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
			if (node->left)
				this->deleteAll(node->left);
			if (node->right)
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
