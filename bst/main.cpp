#include <iostream>
#include <vector>
#include <optional>
#include <stack>
#include <functional>

template <typename K, typename D>
class bst {
private:
	struct node {
		K key; D data; node* parent = nullptr,
			* left = nullptr,
			* right = nullptr;
	} *root = nullptr;
	size_t count = 0;

	node** _find(const K& key) const
	{
		node** p = (node**)&root;
		while ((*p) && (*p)->key != key)
			p = (*p)->key < key ? &((*p)->right) : &((*p)->left);
		return p;
	}

	std::pair<node**, node*> _find_with_parent(const K& key) const 
	{
		node** p = (node**)&root;
		node* parent = nullptr;
		while ((*p) && (*p)->key != key) {
			parent = *p;
			p = (*p)->key < key ? &((*p)->right) : &(*p)->left;
		}
		return { p, parent };
	}

public:
	bst();

	template <typename Iter>
	bst(Iter first, Iter last);
	~bst();

	std::optional <std::reference_wrapper<const D>> find(const K& key) const;
	std::optional <std::reference_wrapper<D>> find(const K& key);
	bool insert(const K& key, const D& data);
	bool insert(K&& key, D&& data);
	bool erase(const K& key);

	std::optional <K> next(const K& key) const;
	std::optional <K> prev(const K& key) const;

	void clear();

	int height();
	int width();

	void visitor(std::function<void (const K&, const D&)> visit);

	size_t size() const;
};




int main() {
	bst<int, int> tree;
	tree.insert(70, 20);
	tree.insert(60, 60);
	tree.insert(99, 99);
	tree.insert(38, 38);
	tree.insert(33, 33);
	tree.insert(50, 50);
	tree.insert(57, 57);
	tree.insert(5, 5);
	tree.insert(45, 45);
	//tree.insert(44, 44);


	//tree.clear();
	std::vector <std::pair<int, int>> dump;
	int prev_data;
	bool result = true, first_flag = true;
	tree.visitor([&](int k, int d) { 
		if (first_flag) {
			prev_data = d;
			first_flag = false;
		}
		else {
			result &= (prev_data < d);
			prev_data = d;
		}
	});

	std::cout << result;
	return 0;
}

template<typename K, typename D>
bst<K, D>::bst() = default;

template<typename K, typename D>
bst<K, D>::~bst()
{
	//this->clear();
	return;
}

template<typename K, typename D>
std::optional <std::reference_wrapper<const D>> bst<K, D>::find(const K & key) const
{
	node** p = _find(key);

	return (*p) ? std::optional<std::reference_wrapper<const D>>((*p)->data) : std::nullopt;
}

template<typename K, typename D>
std::optional <std::reference_wrapper<D>> bst<K, D>::find(const K & key)
{
	node** p = _find(key);

	return (*p) ? std::optional<std::reference_wrapper<D>>((*p)->data) : std::nullopt;
}

template<typename K, typename D>
bool bst<K, D>::insert(const K & key, const D & data)
{
	auto [p, parent] = _find_with_parent(key);
	if (*p) {
		(*p)->data = data;
		return false;
	}
	*p = new node{ key, data, parent };
	count++;
	return true;
}

template<typename K, typename D>
bool bst<K, D>::insert(K && key, D && data)
{
	auto [p, parent] = _find_with_parent(key);
	if (*p) {
		(*p)->data = data;
		return false;
	}
	*p = new node{ key, data, parent };
	count++;
	return true;
}

template<typename K, typename D>
bool bst<K, D>::erase(const K & key)
{
	auto [p, parent] = _find_with_parent(key);

	if (!(*p)) {
		return false;
	}

	if (!(*p)->left && !(*p)->right){
		delete *p;
		*p = nullptr;
	}
	else if (!(*p)->left || !(*p)->right) {

		node* grandson = (*p)->left ? (*p)->left : (*p)->right;

		delete * p;
		*p = grandson;
		grandson->parent = parent;
	}
	else {
		node** min = &(*p)->right;
		while ((*min)->left) {
			min = &(*min)->left;
		}
		node* n = *min;
		*min = (*min)->right;
		if ((*min))
			(*min)->parent = n->parent;
		(*p)->data = n->data;
		(*p)->key = n->key;

		delete n;
	}
	count--;
	return true;
}

template<typename K, typename D>
std::optional<K> bst<K, D>::next(const K& key) const
{
	node** p = (node**)&root;
	node** last_left = nullptr;
	while ((*p) && (*p)->key != key) 
		p = (*p)->key < key ? &((*p)->right) : (last_left = p, &((*p)->left));

	if ((*p) && (*p)->right) {
		p = &((*p)->right);
		while ((*p)->left)
			p = &((*p)->left);
	}
	else {
		p = last_left;
	}

	return p && (*p) ? std::optional<std::reference_wrapper<D>>((*p)->data) : std::nullopt;
}

template<typename K, typename D>
std::optional<K> bst<K, D>::prev(const K& key) const
{
	node** p = (node**)&root;
	node** last_right = nullptr;
	while ((*p) && (*p)->key != key)
		p = (*p)->key < key ? (last_right = p, &((*p)->right)) : &((*p)->left);

	if ((*p) && (*p)->left) {
		p = &((*p)->left);
		while ((*p)->right)
			p = &((*p)->right);
	}
	else {
		p = last_right;
	}

	return p && (*p) ? std::optional<std::reference_wrapper<D>>((*p)->data) : std::nullopt;
}

template<typename K, typename D>
void bst<K, D>::clear()
{
	node* current = root;
	std::stack<node*> s;
	while (current || !s.empty()) {
		if (current) {		
			node* tmp = current->left;
			s.push(current->right);				
			delete current;
			current = nullptr;
			current = tmp;
		}
		else {
			current = s.top();
			s.pop();
		}
	}
	current = nullptr;
	count = 0;
}

template<typename K, typename D>
int bst<K, D>::height()
{
	int result = 0;
	std::stack<std::pair<node*, int>> s;
	s.push({ root, 0 });
	while (!s.empty()) {
		auto [current, level] = s.top();
		s.pop();

		if (current) {
			s.push({ current->left, level + 1 });
			s.push({ current->right, level + 1 });
			result = std::max(result, level);
		}
	}
	return result;
}

template<typename K, typename D>
int bst<K, D>::width()
{
	int min = 0, max = 0;
	std::stack<std::pair<node*, int>> s;
	s.push({ root, 0 });
	while (!s.empty()) {
		auto [current, level] = s.top();
		s.pop();

		if (current) {
			s.push({ current->left, level - 1 });
			s.push({ current->right, level + 1 });
			max = std::max(max, level);
			min = std::min(min, level);
		}
	}
	return max - min;
}

template<typename K, typename D>
void bst<K, D>::visitor(std::function<void(const K&, const D&)> visit)
{
	node* current = root;
	std::stack<node*> s;
	while (current || !s.empty()) {
		if (current) {
			s.push(current);
			current = current->left;
		}
		else {
			current = s.top();
			visit(current->key, current->data);
			s.pop();
			current = current->right;
		}
	}
}

template<typename K, typename D>
size_t bst<K, D>::size() const
{
	return count;
}

template<typename K, typename D>
template<typename Iter>
bst<K, D>::bst(Iter first, Iter last)
{
}
