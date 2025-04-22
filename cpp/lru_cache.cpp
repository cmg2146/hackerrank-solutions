// Solves: https://www.hackerrank.com/challenges/abstract-classes-polymorphism/problem?isFullScreen=true

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>
using namespace std;

/* START HACKERRANK PROVIDED CODE - CANT EDIT */
struct Node{
   Node* next;
   Node* prev;
   int value;
   int key;
   Node(Node* p, Node* n, int k, int val):prev(p),next(n),key(k),value(val){};
   Node(int k, int val):prev(NULL),next(NULL),key(k),value(val){};
};

class Cache{
   
   protected: 
   map<int,Node*> mp; //map the key to the node in the linked list
   int cp;  //capacity
   Node* tail; // double linked list tail pointer
   Node* head; // double linked list head pointer
   virtual void set(int, int) = 0; //set function
   virtual int get(int) = 0; //get function

};
/* END HACKERRANK PROVIDED CODE - CANT EDIT */

class LRUCache : public Cache {
public:
    LRUCache(int capacity) {
        mp = {};
        cp = capacity;
        head = NULL;
        tail = NULL;
    }

    ~LRUCache()
    {
        auto node = head;
        while (node != NULL)
        {
            auto nodeToDelete = node;
            node = node->next;
            delete nodeToDelete;
        }
    }

    void set(int key, int value)
    {
        auto item = mp.find(key);

        // key not found, will add new item to cache
        if (item == mp.end())
        {
            // remove oldest item if cache is full
            if (mp.size() == cp)
            {
                mp.erase(head->key);
                remove_node(head, true);
            }

            // add a new node to the end of the list
            Node* newNode = new Node(key, value);
            add_node(newNode);
            mp[key] = newNode;
        }
        else
        {
            // update existing item
            auto node = item->second;
            node->value = value;
            // touch the node to indicate it's the most recently accessed cache item
            touch(node);
        }
    }
    
    int get(int key)
    {
        int result = -1;

        auto item = mp.find(key);
        // key is found, set result to value contained in node
        if (item != mp.end())
        {
            auto node = item->second;
            result = node->value;
            // touch the node to indicate it's the most recently accessed cache item
            touch(node);
        }

        return result;
    }

private:
    void remove_node(Node* node, bool deallocate)
    {
        if (node == head)
        {
            head = head->next;
        }
        if (node == tail)
        {
            tail = tail->prev;
        }
        if (node->prev != NULL)
        {
            node->prev->next = node->next;
        }
        if (node->next != NULL)
        {
            node->next->prev = node->prev;
        }
        
        if (deallocate)
        {
            delete node;
        }
    }
    
    void add_node(Node* node)
    {
        node->prev = tail;
        node->next = NULL;

        if (head == NULL)
        {
            head = node;
        }
        if (tail != NULL)
        {
            tail->next = node;
        }

        tail = node;
    }

    void touch(Node* node)
    {
        // move node to end of the list  
        remove_node(node, false);
        add_node(node);
    }
};