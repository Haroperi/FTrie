// ftrie.h
#ifndef FTRIE_H
#define FTRIE_H

#include <vector>
#include <algorithm>

#define DEBUG

using namespace std;

template < class t_data > class CNode
{
public:
  vector < CNode * >child;
  t_data data;
};

template < class t_data > class CFtrie
{
public:
	CFtrie()
	{
		m_root = new CNode<t_data>;
		m_leaf = new CNode<t_data>;
	}
	~CFtrie()
	{
	}

	bool search(const vector<t_data> &v) const
	{
		CNode<t_data> *node = m_root;
		for (typename vector<t_data>::const_iterator itr = v.begin(); itr != v.end(); itr++) {
			node = search_child(node, *itr);
			if (node == NULL) {
				return false;
			}
		}
		return find(node->child.begin(), node->child.end(), m_leaf) != node->child.end();
	}

	bool has_leaf(const CNode<t_data> *node) const
	{
		return find(node->child.begin(), node->child.end(), m_leaf) != node->child.end();
	}

	bool fuzzysearch_child(const CNode<t_data> *node, vector<t_data> v, int geta, int cost) const
	{
		if (cost < 0)
			return false;
		if (v.size()-geta == 1) {
			// 最後の1文字
			for (size_t i = 0; i < node->child.size(); i++) {
				if (node->child[i]->data == v[geta])
					return has_leaf(node->child[i]);
			}
			return false;
		}
		for (typename vector<CNode<t_data> *>::const_iterator itr = node->child.begin();
				itr != node->child.end(); itr++) {
			if ((*itr)->data == v[geta]) {
				if (fuzzysearch_child(*itr, v, geta+1, cost))
					return true;
			}
			else {
				if (fuzzysearch_child(*itr, v, geta, cost-1) ||
						fuzzysearch_child(node, v, geta+1, cost-1) ||
						fuzzysearch_child(*itr, v, geta+1, cost-1)) {
					return true;
				}
			}
		}
		return false;
	}

	bool fuzzysearch(const vector<t_data> &v, int cost) const
	{
		int idx = -1;
		if (v.size() == 0)
			return false;
		for (typename vector<CNode<t_data> *>::const_iterator itr = m_root->child.begin();
				itr != m_root->child.end(); itr++) {
			if ((*itr)->data == v[0]) {
				if (v.size() == 1)
					return has_leaf(*itr);
				return fuzzysearch_child(*itr, v, 1, cost);
			}
		}
		return false;
	}

	void insert(const vector<t_data> &v)
	{
		CNode<t_data> *node = m_root;
		for (typename vector<t_data>::const_iterator itr = v.begin(); itr != v.end(); itr++) {
			CNode<t_data> *child = search_child(node, *itr);
			if (child == NULL) {
				child = new CNode<t_data>;
				child->data = *itr;
				node->child.push_back(child);
			}
			node = child;
		}
		if (find(node->child.begin(), node->child.end(), m_leaf) == node->child.end()) {
			node->child.push_back(m_leaf);
		}
	}

	void remove(const vector<t_data> &v)
	{
		CNode<t_data> *node = m_root;
		for (typename vector<t_data>::const_iterator itr = v.begin(); itr != v.end(); itr++) {
			node = search_child(node, *itr);
			if (node == NULL) {
				return;
			}
		}
		typename vector<CNode<t_data> *>::iterator itr = find(node->child.begin(), node->child.end(), m_leaf);
		if (itr != node->child.end()) {
			node->child.erase(itr);
		}
	}

#ifdef DEBUG
  void show ()
  {
    printf ("[ ");
    show2 (m_root);
    printf ("] \n");
  }
#endif

private:
  CNode < t_data > *m_root;
  CNode < t_data > *m_leaf;

  CNode < t_data > *search_child (CNode < t_data > *n, const t_data & d) const
  {
    for (typename vector < CNode < t_data > *>::iterator itr =
	 n->child.begin (); itr != n->child.end (); itr++)
      {
	if ((*itr)->data == d)
	  {
	    return *itr;
	  }
      }
    return NULL;
  }

#ifdef DEBUG
  void show2 (CNode < t_data > *node)
  {
    printf ("%c [", node->data);
    for (typename vector < CNode < t_data > *>::const_iterator itr =
	 node->child.begin (); itr != node->child.end (); itr++)
      {
	if (*itr == m_leaf)
	  printf ("leaf ");
	else
	  show2 (*itr);
      }
    printf ("]");
  }
#endif
};

#endif	/* FTRIE_H */
