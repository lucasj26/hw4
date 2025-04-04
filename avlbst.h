#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void rotateLeft(AVLNode<Key, Value>* n);
    void rotateRight(AVLNode<Key, Value>* n);
    void removeFix(AVLNode<Key, Value>* n, int diff);

};

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* n)
{
    AVLNode<Key, Value>* c = n->getRight();
    AVLNode<Key, Value>* leftSub = c->getLeft();

    c->setLeft(n);
    n->setRight(leftSub);
    
    if(leftSub!=nullptr){
        leftSub->setParent(n);
    }
    c->setParent(n->getParent());

    if(n==this->root_){
        this->root_=c;
    }
    else{
        AVLNode<Key, Value>* p = c->getParent();
        if(p->getLeft()==n){
            p->setLeft(c);
        }
        else{
            p->setRight(c);
        }

    }
    n->setParent(c);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* n)
{
    AVLNode<Key, Value>* c = n->getLeft();
    AVLNode<Key, Value>* rightSub = c->getRight();

    c->setRight(n);
    n->setLeft(rightSub);

    if(rightSub!=nullptr){
        rightSub->setParent(n);
    }
    c->setParent(n->getParent());

    if(n==this->root_){
        this->root_=c;
    }
    else{
        AVLNode<Key, Value>* p = c->getParent();
        if(p->getLeft()==n){
            p->setLeft(c);
        }
        else{
            p->setRight(c);
        }

    }
    n->setParent(c);
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if (this->root_ == nullptr) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }

    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;

    while(curr!=nullptr){
        if(new_item.first == curr->getKey()){
            curr->setValue(new_item.second);
            return;
        }
        parent = curr;
        if(new_item.first < curr->getKey()){
            curr = curr->getLeft();
        }
        else{
            curr = curr->getRight();
        }

    }
    curr = new AVLNode<Key,Value>(new_item.first,new_item.second,parent);
    int diff = 0;
    if (new_item.first < parent->getKey()) {
        parent->setLeft(curr);
        diff = -1;
    } else {
        parent->setRight(curr);
        diff = 1;
    }

    parent->updateBalance(diff);

    if (parent->getBalance() == 0) {
        return;
    } else {
        this->insertFix(parent, curr);
    }


}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n)
{
    if (p == nullptr || p->getParent() == nullptr) return;

    AVLNode<Key, Value>* g = p->getParent();

    if(g->getLeft()==p){
        g->updateBalance(-1);
        if(g->getBalance()==0) return;
        else if(g->getBalance()==-1) insertFix(g,p);
        else if(g->getBalance()==-2){
            if(p->getLeft()==n){
                rotateRight(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            else{
                rotateLeft(p);
                rotateRight(g);
                if(n->getBalance()==0){
                    p->setBalance(0);
                    g->setBalance(0);
                }
                else if(n->getBalance()==-1){
                    p->setBalance(0);
                    g->setBalance(1);
                }
                else{
                    p->setBalance(-1);
                    g->setBalance(0);
                }
                n->setBalance(0);

            }
        }
    }
    else{
        g->updateBalance(1);
        if(g->getBalance()==0) return;
        else if(g->getBalance()==1) insertFix(g,p);
        else if(g->getBalance()==2){
            if(p->getRight()==n){
                rotateLeft(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            else{
                rotateRight(p);
                rotateLeft(g);
                if(n->getBalance()==0){
                    p->setBalance(0);
                    g->setBalance(0);
                }
                else if(n->getBalance()==-1){
                    p->setBalance(1);
                    g->setBalance(0);
                }
                else{
                    p->setBalance(0);
                    g->setBalance(-1);
                }
                n->setBalance(0);

            }
        }

    }

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* n = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if(n == nullptr) return;

    AVLNode<Key, Value>* p = n->getParent();
    int diff = 0;

    if(n->getLeft() != nullptr && n->getRight() != nullptr) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(n));
        this->nodeSwap(n, pred);
        p = n->getParent();
        
    }
    

    if(p!=nullptr){
        if(p->getLeft()==n){
            diff=1;
        }
        else{
            diff=-1;
        }
    }

    AVLNode<Key, Value>* c = nullptr;
    if(n->getLeft()!=nullptr){
        c = n->getLeft();
    }
    else if(n->getRight()!=nullptr){
        c = n->getRight();
    }

    if(p==nullptr){
        this->root_ = c;
    }
    else{
        if(p->getLeft()==n){
            p->setLeft(c);
        }
        else{
            p->setRight(c);
        }
    }

    if(c!=nullptr){
        c->setParent(p);
    }
    delete n;

    removeFix(p,diff);

}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int diff)
{
    if(n==nullptr) return;

    AVLNode<Key, Value>* p = n->getParent();
    int nextDiff = 0;

    if(p!=nullptr){
        if(p->getLeft()==n){
            nextDiff = 1;
        }
        else{
            nextDiff = -1;
        }
    }

    int b = n->getBalance();
    int newBal = b + diff;
    if(newBal==0){
        n->setBalance(newBal);
        removeFix(p,nextDiff);
    }
    else if(newBal==-1 || newBal==1){
        n->setBalance(newBal);
    }
    else if(newBal==-2){
        AVLNode<Key, Value>* c = n->getLeft();
        int cBal = c->getBalance();
        if(cBal==-1){
            rotateRight(n);
            n->setBalance(0);
            c->setBalance(0);
            removeFix(p,nextDiff);
        }
        else if(cBal == 0){
            rotateRight(n);
            n->setBalance(-1);
            c->setBalance(1);
        }
        else if(cBal==1){
            AVLNode<Key, Value>* g = c->getRight();
            int gBal = g->getBalance();
            rotateLeft(c);
            rotateRight(n);
            if (gBal == 1) {
                n->setBalance(0);
                c->setBalance(-1);
            }
            else if (gBal == 0) {
                n->setBalance(0);
                c->setBalance(0);
    
            }
            else if (gBal == -1) {
                n->setBalance(1);
                c->setBalance(0);
            }
        
            g->setBalance(0);
            removeFix(p, nextDiff);

        }
    }

    else if(newBal==2){
        AVLNode<Key, Value>* c = n->getRight();
        int cBal = c->getBalance();
        if(cBal==1){
            rotateLeft(n);
            n->setBalance(0);
            c->setBalance(0);
            removeFix(p,nextDiff);
        }
        else if(cBal ==0){
            rotateLeft(n);
            n->setBalance(1);
            c->setBalance(-1);
        }
        else if(cBal==-1){
            AVLNode<Key, Value>* g = c->getLeft();
            int gBal = g->getBalance();
            rotateRight(c);
            rotateLeft(n);
            if (gBal == 1) {
                n->setBalance(-1);
                c->setBalance(0);
            }
            else if (gBal == 0) {
                n->setBalance(0);
                c->setBalance(0);
    
            }
            else if (gBal == -1) {
                n->setBalance(0);
                c->setBalance(1);
            }
        
            g->setBalance(0);
            removeFix(p, nextDiff);

        }
    }



}



template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
