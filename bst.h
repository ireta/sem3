#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

#include <functional>
#include <cstddef>
#include <ostream>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <cmath>
#include <utility>

#include <iostream>   // you may use it for debugging your code


class DefaultBalanceCondition    // default BST, do not change
{
  public:
    bool operator() (int current_height, int ideal_height) const
    {
      return true;
    }
};


template <typename Key, typename Object,
          typename BalanceCondition=DefaultBalanceCondition,
	  typename Comparator=std::less<Key> >
class BinarySearchTree
{
  public:  /* DO NOT CHANGE */
    struct Node   // core structure of BinarySearchTree
    {
       Key key;         // unique key
       Object data;     // data to be stored, characterized by key
       Node * left;     // pointer to left subtree
       Node * right;    // pointer to right subtree
       size_t height;   // height of the node
       size_t subsize;  // size of the subtree node roots including itself

       Node(const Key &, const Object &,
            Node *, Node *, size_t =0, size_t =1);   // Node constructor
    };

  public:  /* DO NOT CHANGE PROTOTYPES */
    BinarySearchTree();  // zero-parameter constructor
    BinarySearchTree(const std::list<std::pair<Key, Object> > &); // list is always sorted!
    ~BinarySearchTree();  // destructor

  public:  /* DO NOT CHANGE PROTOTYPES */
    void insert(const Key &, const Object &);  // insert new Key-Object
    void remove(const Key &);  // remove Node characterized by Key
    void toCompleteBST();  // convert into a complete BST

  public:  /* DO NOT CHANGE PROTOTYPES */
    Node * find(const Key &) const; // single item
    std::list<Node *> find(const Key &, const Key &) const; // range queries
    int height() const;  // return the height of the tree
    size_t size() const; // return the number of items in the tree
    bool empty() const;  //return whether the tree is empty or not
    Node * getRoot() const; // return a pointer to the root of the tree
    void print(std::ostream &) const;  // print tree structure into an output stream

  private:  /* DO NOT CHANGE DATA TYPES AND IDENTIFIERS */
    Node * root;                     // designated root
    size_t numNodes;                 // size
    Comparator isLessThan;           // operator upon which BST nodes are arranged
    BalanceCondition isBalanced;     // signals whether the signal is balanced

  private:  /* DO NOT CHANGE PROTOTYPES */
    Node * find(const Key &, Node *) const;
    int height(Node *) const;
    int subsize(Node *) const;
    void print(Node *, std::ostream &) const;

    // Define your const private utility functions below this line

  private:
    void makeEmpty(Node * &);       // utility for destructor

    // Define your private utility functions below this line


  private: /* DO NOT CHANGE PROTOTYPES: compiler defaults are blocked */
    BinarySearchTree(const BinarySearchTree &);
    const BinarySearchTree & operator=(const BinarySearchTree &);

  private:  // static utility functions
    template <typename T> //static utility function
    static const T & max(const T &, const T &);

    // Define more below this line if needed
    Node* helper_construct(std::list<std::pair<Key,Object>>);
    typename std::list<std::pair<Key,Object>>::const_iterator mid_point_list(const std::list<std::pair<Key,Object>> &)const;
    int arrange(Node *);
    void to_vector(std::vector<Node*>&, Node*);
    Node* helper_insert(const Key&, const Object&, Node*);
    void helper_toComplete(std::vector<Node*>, Node*&, int);
    Node* mid_point_vector(const std::vector<Node*> &)const;
    int heights(Node*);
    Node* helper_remove(const Key&, Node*);
    Node* leftmost(Node*);
    Node* rightmost(Node*);
    Node* parent(Node*, Node*);
    void helper_find(std::list<Node *>&, Node*, const Key&, const Key&)const;
};

#endif

template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::helper_find(std::list<Node*> &datalist,Node *current, const K &lower, const K &upper)const
{
    if(current == NULL){
        return;
    }
    if(isLessThan(lower,current->key)){
        helper_find(datalist,current->left,lower,upper);
    }
    if(current->key >= lower && current->key <= upper){
        datalist.push_back(current);
    }
    if(isLessThan(current->key,upper)){
        helper_find(datalist,current->right,lower,upper);
    }
    return;
}

template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::parent(Node* current, Node* theOne)
{
    if(!current){
        return NULL;
    }
    if(current->left == theOne || current->right == theOne || current == theOne){
        return current;
    }
    if(isLessThan(current->key,theOne->key)){
        return parent(current->right,theOne);
    }
    return parent(current->left,theOne);
}

template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::rightmost(Node *current)
{
    if(current->right == NULL || current == NULL){
        return current;
    }
    return rightmost(current->right);
}

template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::leftmost(Node *current)
{
    if(current->left == NULL || current == NULL){
        return current;
    }
    return leftmost(current->left);
}

template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::helper_remove(const K &k ,Node* current)
{
    if(current == NULL){
        return NULL;
    }
    if(isLessThan(k,current->key)){
        current->left = helper_remove(k,current->left);
    }else if(isLessThan(current->key,k)){
        current->right = helper_remove(k,current->right);
    }else if(current->left == NULL && current->right == NULL){
        delete current;
        return NULL;
    }else if(current == root){
        if(root->right == NULL){
            Node *tmp = rightmost(root->left);
            Node *p = parent(root,tmp);
            if(p == current){
                p->left = tmp->left;
            }
            else{
                p->right = tmp->left;
            }
            tmp->left = root->left;
            tmp->right = root->right;
            root = tmp;
            delete current;
            arrange(root);
            heights(root);
            if(!isBalanced(tmp->height,floor(log2(tmp->subsize)))){
                //std::cout << "balance " << current->height << " " << current->subsize << std::endl;
                std::vector<Node*> tree;
                to_vector(tree,tmp);
                helper_toComplete(tree,tmp,1);
                arrange(tmp);
                heights(root);
            }
            return tmp;
        }
        else{
            Node *tmp = leftmost(root->right);
            Node *p = parent(root,tmp);
            if(p == current){
                p->right = tmp->right;
            }
            else{
                p->left = tmp->right;
            }
            tmp->left = root->left;
            tmp->right = root->right;
            root = tmp;
            delete current;
            arrange(root);
            heights(root);
            if(!isBalanced(tmp->height,floor(log2(tmp->subsize)))){
                //std::cout << "balance " << current->height << " " << current->subsize << std::endl;
                std::vector<Node*> tree;
                to_vector(tree,tmp);
                helper_toComplete(tree,tmp,1);
                arrange(tmp);
                heights(root);
            }
            return tmp;
        }
    }
    else if(current->right == NULL){
        Node *tmp = rightmost(current->left);
        Node *p1 = parent(current,tmp);
        Node *p2 = parent(root,current);
        if(p1 == current){
            p1->left = tmp->left;
        }
        else{
            p1->right = tmp->left;
        }
        if(p2->left == current){
            p2->left = tmp;
        }
        else{
            p2->right = tmp;
        }
        tmp->left = current->left;
        tmp->right = current->right;
        delete current;
        arrange(root);
        heights(root);
        if(!isBalanced(tmp->height,floor(log2(tmp->subsize)))){
            //std::cout << "balance " << current->height << " " << current->subsize << std::endl;
            std::vector<Node*> tree;
            to_vector(tree,tmp);
            helper_toComplete(tree,tmp,1);
            arrange(tmp);
            heights(root);
        }
        return tmp;
    }
    else{
        Node *tmp = leftmost(current->right);
        Node *p1 = parent(current,tmp);
        Node *p2 = parent(root,current);
        if(p1 == current){
            p1->right = tmp->right;
        }
        else{
            p1->left = tmp->right;
        }
        if(p2->left == current){
            p2->left = tmp;
        }
        else{
            p2->right = tmp;
        }
        tmp->left = current->left;
        tmp->right = current->right;
        delete current;
        arrange(root);
        heights(root);
        if(!isBalanced(tmp->height,floor(log2(tmp->subsize)))){
            //std::cout << "balance " << current->height << " " << current->subsize << std::endl;
            std::vector<Node*> tree;
            to_vector(tree,tmp);
            helper_toComplete(tree,tmp,1);
            arrange(tmp);
            heights(root);
        }
        return tmp;
    }
    heights(root);
    arrange(root);
    if(!isBalanced(current->height,floor(log2(current->subsize)))){
        //std::cout << "balance " << current->height << " " << current->subsize << std::endl;
        std::vector<Node*> tree;
        to_vector(tree,current);
        helper_toComplete(tree,current,1);
        arrange(current);
        heights(root);
    }
    return current;
}

template <typename K, typename O, typename B, typename C>
int
BinarySearchTree<K,O,B,C>::heights(Node* current)
{
    if(!current){
        return 0;
    }
    int l_height = heights(current->left);
    int r_height = heights(current->right);
    current->height = max(l_height,r_height);
    return max(l_height,r_height) + 1;
}

template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::mid_point_vector(const std::vector<Node*> &tree)const
{
    if(tree.size() == 0){
        return NULL;
    }else if(tree.size() == 1){
        return tree.front();
    }else if(tree.size() == 2 || tree.size() == 3){
        return tree[1];
    }
    int h = floor(log2(tree.size()));
    int last_floor = tree.size()-pow(2,h)+1;
    if(last_floor <= pow(2,h-1)){
        last_floor += pow(2,h-1)-1;
        return tree[last_floor];
    }
    last_floor = pow(2,h)-1;
    //std::cout << last_floor << " " << tree.size() << std::endl;
    return tree[last_floor];
}

template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::helper_insert(const K &k, const O &x, Node* tmp)
{
    if(tmp == NULL){
        return new Node(k,x,NULL,NULL);
    }else if(isLessThan(k,tmp->key)){
        tmp->left = helper_insert(k,x,tmp->left);
    }else{
        tmp->right = helper_insert(k,x,tmp->right);
    }
    int ideal_height = floor(log2(++tmp->subsize));
    heights(root);
    if(!isBalanced(tmp->height,ideal_height)){
        //std::cout << "balance" << std::endl;
        std::vector<Node*> tree;
        to_vector(tree,tmp);
        helper_toComplete(tree,tmp,1);
        arrange(tmp);
        heights(root);
    }
    return tmp;
}

template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::helper_toComplete(std::vector<Node*> tree, Node*& tmp, int first)
{
    if(tree.empty()){
        return;
    }
    int i=0;
    std::vector<Node*> left;
    std::vector<Node*> right;
    Node *current = mid_point_vector(tree);
    for(i=0 ; tree[i]!=current ; i++){
        left.push_back(tree[i]);
    }
    for(int j=i+1 ; j<(int)tree.size() ; j++){
        right.push_back(tree[j]);
    }
    if(tree.size() == numNodes){
        root = current;
    }
    if(first){
        tmp = current;
    }
    //std::cout << tmp->data << std::endl;
    current->left = mid_point_vector(left);
    current->right = mid_point_vector(right);
    helper_toComplete(left,tmp->left, 0);
    helper_toComplete(right,tmp->right, 0);
}

template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::to_vector(std::vector<Node*> &tree, Node* tmp)
{
    if(tmp == NULL){
        return;
    }
    to_vector(tree,tmp->left);
    tree.push_back(tmp);
    to_vector(tree,tmp->right);
}

template <typename K, typename O, typename B, typename C>
int
BinarySearchTree<K,O,B,C>::arrange(Node *current)
{
    if(!current){
        return 0;
    }
    int le_size = arrange(current->left);
    int ri_size = arrange(current->right);
    heights(current);
    current->subsize = le_size + ri_size + 1;
    return le_size + ri_size + 1;
}

template <typename K, typename O, typename B, typename C>
typename std::list<std::pair<K,O>>::const_iterator
BinarySearchTree<K,O,B,C>::mid_point_list(const std::list<std::pair<K,O>> & datalist)const
{
    if(datalist.size() == 1){
        return datalist.begin();
    }else if(datalist.size() == 2 || datalist.size() == 3){
        typename std::list<std::pair<K,O>>::const_iterator it = datalist.begin();
        return ++it;
    }
    int h = floor(log2(datalist.size()));
    int last_floor = datalist.size()-pow(2,h)+1;
    if(last_floor <= pow(2,h-1)){
        typename std::list<std::pair<K,O>>::const_iterator it = datalist.begin();
        int i=0;
        last_floor += pow(2,h-1)-1;
        while(i < last_floor){
            ++it;
            ++i;
        }
        return it;
    }
    typename std::list<std::pair<K,O>>::const_iterator it = datalist.begin();
    int i=0;
    last_floor = pow(2,h-1)+1;
    while(i < last_floor){
        ++it;
        ++i;
    }
    return it;
}

template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::helper_construct(std::list<std::pair<K,O>> datalist)
{
    if(datalist.size() == 0){
        return NULL;
    }
    typename std::list<std::pair<K,O>> left;
    typename std::list<std::pair<K,O>> right;
    typename std::list<std::pair<K,O>>::const_iterator it = mid_point_list(datalist);
    left.splice(left.begin(), datalist, datalist.begin(), it);
    Node* current = new Node(it->first, it->second, NULL, NULL);
    datalist.erase(it);
    right.splice(right.begin(), datalist, datalist.begin(), datalist.end());
    current->left = helper_construct(left);
    current->right = helper_construct(right);
    return current;
}

// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
BinarySearchTree<K,O,B,C>::Node::
Node(const K & _k, const O & _d, Node * _l, Node * _r, size_t _h, size_t _s)
  : key(_k), data(_d), left(_l), right(_r), height(_h), subsize(_s)
{
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
BinarySearchTree<K,O,B,C>::BinarySearchTree()
  : root(NULL), numNodes(0)  //comparator(C() ??)
{
}


// IMPLEMENT
template <typename K, typename O, typename B, typename C>
BinarySearchTree<K,O,B,C>::BinarySearchTree(const std::list<std::pair<K,O> > & datalist)
   : root(NULL), numNodes(0)  // change it as you'd like
{
    numNodes = datalist.size();
    root = helper_construct(datalist);
    arrange(root);
}


// IMPLEMENT
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::toCompleteBST()
{
    std::vector<Node*> tree;
    to_vector(tree,root);
    helper_toComplete(tree,root,1);
    heights(root);
    arrange(root);
}


// IMPLEMENT
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::insert(const K & k, const O & x)
{
    Node *current = find(k);
    if(current != NULL){
        current->data = x;
        return;
    }
    std::cout << "i am here" << std::endl;
    numNodes++;
    root = helper_insert(k,x,root);
}

// IMPLEMENT
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::remove(const K & k)
{
    if(!find(k)){
        return;
    }
    numNodes--;
    root = helper_remove(k,root);
}



// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::find(const K & key) const
{
  return find(key, root);
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::find(const K & key, Node * t) const
{
  if (t == NULL)
  {
    return NULL;
  }
  else if (isLessThan(key, t->key))
  {
    return find(key, t->left);
  }
  else if (isLessThan(t->key, key))
  {
    return find(key, t->right);
  }
  else //found
  {
    return t;
  }
}


// IMPLEMENT
template <typename K, typename O, typename B, typename C>
std::list<typename BinarySearchTree<K,O,B,C>::Node *>
BinarySearchTree<K,O,B,C>::find(const K & lower, const K & upper) const
{
    std::list<Node *> ranges;

    // implement
    if(root != NULL){
        helper_find(ranges,root,lower,upper);
    }
    return ranges;
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::getRoot() const
{
  return root;
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
int
BinarySearchTree<K,O,B,C>::height() const
{
  return height(root);
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
int
BinarySearchTree<K,O,B,C>::height(Node * t) const
{
  return (t == NULL) ? -1 : t->height;
}


// private utility
template <typename K, typename O, typename B, typename C>
int
BinarySearchTree<K,O,B,C>::subsize(Node * t) const
{
  return (t == NULL) ? 0 : t->subsize;
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
size_t
BinarySearchTree<K,O,B,C>::size() const
{
  return numNodes;
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
bool
BinarySearchTree<K,O,B,C>::empty() const
{
  return numNodes == 0;
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::makeEmpty(Node * & t)
{
  if (t != NULL)
  {
    makeEmpty(t->left);
    makeEmpty(t->right);
    delete t;

    --numNodes;
  }

  t = NULL;
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
BinarySearchTree<K,O,B,C>::~BinarySearchTree()
{
  makeEmpty(root);
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::print(std::ostream & out) const
{
  print(root, out);
  out << '\n';
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::print(Node * t, std::ostream & out) const
{
  if (t != NULL && t->left != NULL)
  {
    out << '[';
    print( t->left, out );
  }
  else if (t != NULL && t->left == NULL && t->right != NULL)
  {
    out << "[";
  }

  if (t != NULL)
  {
    if (t->left == NULL && t->right == NULL)
    {
      out << '(' << (t->key) << ')';
    }
    else if (t->left != NULL || t->right != NULL)
    {
      out << '{' << (t->key) << ",H" << t->height << ",S" << t->subsize << '}';
    }
  }

  if (t != NULL && t->right != NULL)
  {
    print( t->right, out );
    out << ']';
  }
  else if (t != NULL && t->left != NULL && t->right == NULL)
  {
    out << "]";
  }
}


// do not change
template <typename K, typename O, typename B, typename C>
template <typename T>
const T &
BinarySearchTree<K,O,B,C>::max(const T & el1, const T & el2)
{
  return el1 > el2 ? el1 : el2;
}

