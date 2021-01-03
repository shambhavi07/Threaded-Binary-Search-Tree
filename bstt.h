/*bstt.h*/
// Shambhavi Danayak
// University of Illinois at Chicago - CS 
//
//
// Project 4
//
// Threaded binary search tree


#pragma once

#include <iostream>

using namespace std;

template<typename KeyT, typename ValueT>
class bstt
{
private:
  struct NODE
  {
    KeyT   Key;
    ValueT Value;
    NODE*  Left;
    NODE*  Right;
    bool   isThreaded;
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size;  // # of nodes in the tree (0 if empty)
  NODE* curStateNode; //This is used to maintain the internal state for begin and next

NODE* leftMost(NODE *node) 
{ 
    if (node == nullptr) {
       return nullptr; 
    }
    while (node->Left != nullptr) {
        node = node->Left; 
     }
  
    return node; 
} 
  
  //This is used to define Operator function
  void insByPreOrder(NODE* otherNode)
 {
      
      if (otherNode== nullptr) //Nothing to copy from otherNode
      {
        return; 
      }    
  if(otherNode != nullptr)
{
          /* insert othernode's key and value in current node*/
          insert(otherNode->Key,otherNode->Value);

          /* call recursively with otherNode's Left sub Node*/
          insByPreOrder(otherNode->Left);  

          /* call recursively with otherNode's Right Node*/
          if(!(otherNode->isThreaded)) // insert only if its not threaded
         {    
              insByPreOrder(otherNode->Right); 
          }
          else //Return when its threaded
          { 
              return;
          }
      }  
  }


  // printNodes_InOrder:
  //
  // prints the tree in inorder
  void printNodes_InOrder(NODE* curNode, ostream& output)const 
{

      if(curNode == nullptr){ 
          return; //nothing to print when there is no nodes in tree
      }
      else{
          printNodes_InOrder(curNode->Left, output);
          
          if(curNode->isThreaded  && curNode->Right != nullptr){
              output <<"(" << curNode->Key << "," << curNode->Value << "," << curNode->Right->Key << ")" << endl;
              curNode->Right = nullptr;
          }else{
              output <<"(" << curNode->Key << "," << curNode->Value << ")" << endl;
          }
          
          printNodes_InOrder(curNode->Right, output);
      }
}

  //
  //clearAllNodes:
  //
  //This function is used to freeup memory used by all nodes.
  //Nod
  void clearAllNodes(NODE* curNode)
  {
      if(curNode== nullptr) 
          return; //Nothing to clear
      else{
          clearAllNodes(curNode->Left); //Cleanup Left node
          if(curNode->isThreaded == true){ 
              curNode->Right = nullptr; //cleanup Rightnode pointer for threaded node
          }
          clearAllNodes(curNode->Right); //Cleanup right node
          delete curNode; // cleanup the memory for current node
      }
      
  }


public:
  //
  // default constructor:
  //
  // Creates an empty tree.
  //
  bstt()
  {
    Root = nullptr;
    Size = 0;
  }

  //
  // copy constructor
  //
  bstt(const bstt& other)
  {
    //
    Root = nullptr;
    Size=0;
    insByPreOrder(other.Root);
	
  }

	//
  // destructor:
  //
  // Called automatically by system when tree is about to be destroyed;
  // this is our last chance to free any resources / memory used by
  // this tree.
  //
  virtual ~bstt()
  {
     clearAllNodes(Root);   
  }
  

  //
  // operator=
  //
  // Clears "this" tree and then makes a copy of the "other" tree.
  //
  bstt& operator=(const bstt& other)
  {
    //
    clear();
	NODE *otherNode = other.Root;       
     insByPreOrder(otherNode);
	Size=other.Size;
    return *this;
  }

  //
  // clear:
  //
  // Clears the contents of the tree, resetting the tree to empty.
  //
  void clear()
  {
    //
    clearAllNodes(Root);
    Root=nullptr;
	Size=0;
  }

  // 
  // size:
  //
  // Returns the # of nodes in the tree, 0 if empty.
  //
  // Time complexity:  O(1) 
  //
  int size() const
  {
    return Size;
  }

  // 
  // search:
  //
  // Searches the tree for the given key, returning true if found
  // and false if not.  If the key is found, the corresponding value
  // is returned via the reference parameter.
  //
  // Time complexity:  O(lgN) on average
  //
  bool search(KeyT key, ValueT& value) const
  {
    //
    NODE* curNode = Root;

    while (curNode !=nullptr)
    {	  
      if (key == curNode->Key)  // If key already exists return current node's value
	  {
		value = curNode->Value;
        return true;
      }
      if (key < curNode->Key)  // compare key with left node
      {
        curNode = curNode->Left;  //assign current node's left node as current node
      }
      else //compare with right nodes
      {
		  if(curNode->isThreaded)
		  {
			  curNode=nullptr;  //assign nullpointer to current node if its threaded node
	      }
		  else{				
				curNode = curNode->Right; //assign current node's right node as current node
			}
		}
    }//while
 
    return false;	//return false , when key is not found
  }



  //
  // insert
  //
  // Inserts the given key into the tree; if the key has already been insert then
  // the function returns without changing the tree.
  //
  // Time complexity:  O(lgN) on average
  //
  //
  void insert(KeyT key, ValueT value)
  {
    //
    NODE* prev = nullptr;
    NODE* curNode = Root;

    // Before insert, Search new key is already exists in Tree
    while (curNode !=nullptr)
    {
      if (key == curNode->Key)  // new Key already exists in Tree
         return;

      if (key < curNode->Key)  // if new key is less than current key, search left:
      {
        prev = curNode;
        curNode= curNode->Left;
      }
      else // if new key is less than current key, search right:
      {
		  prev=curNode;
		  if(curNode->isThreaded)
		  {
			  curNode=nullptr;
	      }
		  else{				
				curNode= curNode->Right;
			}
		}
    }//while

   //create new node and assign new key, values
      
   NODE* newNode = new NODE;
   newNode->Key = key;
   newNode->Value = value;

   newNode->Right = nullptr;
   newNode->Left = nullptr; 
   newNode->isThreaded=true;  //assign threaded node flag as true for new node

    //
    // link the new node
    //
      if(prev == nullptr)
      {
         Root = newNode;  //Make newnode as Root node
      }
      else if(key < prev->Key)
      {
	     newNode->Right= prev;
         prev->Left = newNode; //make new node as left node 
      }
      else
      {
	     newNode->Right=prev->Right; 
		 prev->isThreaded=false;
         prev->Right = newNode; //make new node as right node 
      }
	Size++;
}


  //
  // []
  //
  // Returns the value for the given key; if the key is not found,
  // the default value ValueT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  ValueT operator[](KeyT key) const
  {
     //return ValueT{ };
    //
    NODE* cur = Root;

    while (cur != nullptr)
    {
      if (key == cur->Key)  // Key already exists
        return cur->Value;

      if (key < cur->Key)  // if the given key is smaller then search left node
      {
        cur = cur->Left;
      }
      else
      {
		  if(cur->isThreaded)
		  {
			  cur=nullptr;
	      }
		  else{
				cur = cur->Right;
			}
		}
    }//while  

    return ValueT{ };
  }

  //
  // ()
  //
  // Finds the key in the tree, and returns the key to the "right".
  // If the right is threaded, this will be the next inorder key.
  // if the right is not threaded, it will be the key of whatever
  // node is immediately to the right.
  //
  // If no such key exists, or there is no key to the "right", the
  // default key value KeyT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  KeyT operator()(KeyT key) const
  {
    NODE* curNode = Root;
    
    while (curNode != nullptr)
    {
      if (key == curNode->Key) 
	  {  
           // found Key in Tree
           if(curNode->isThreaded && curNode->Right == nullptr)
		   {
              return KeyT{ }; //return default value when no right key exists
          }
          else if(curNode->isThreaded && curNode->Right != nullptr)
		  {
              curNode = curNode->Right; 
              return curNode->Key; //return threaded link's key
          }
          else if (curNode->isThreaded == false)
		  {
              curNode= curNode->Right;
              return curNode->Key; //return right node's key
          }
      }

      if (key < curNode->Key)  // search from left side node:
      {
          curNode= curNode->Left;
      }
      else //search from right side node
      {
          if(curNode->isThreaded) //check if current node is threaded
{
              curNode= nullptr; //stop traversal.
}          
else //traverse right node
{
              curNode= curNode->Right;
}
      }
    }//while

    return KeyT{ };
  }

  //
  // begin
  //
  // Resets internal state for an inorder traversal.  After the 
  // call to begin(), the internal state denotes the first inorder
  // key; this ensure that first call to next() function returns
  // the first inorder key.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  void begin()
  {
    NODE* curNode = Root;
     
    if(curNode == nullptr){ //No nodes in tree
        curStateNode = curNode; //copy current node as current state node
        return;
    }
    
    while(curNode->Left != nullptr){
        curNode = curNode->Left;
    }
    
    curStateNode = curNode; //copy current node as current state node
  }

  //
  // next
  //
  // Uses the internal state to return the next inorder key, and 
  // then advances the internal state in anticipation of future
  // calls.  If a key is in fact returned (via the reference 
  // parameter), true is also returned.
  //
  // False is returned when the internal state has reached null,
  // meaning no more keys are available.  This is the end of the
  // inorder traversal.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  bool next(KeyT& key)
  {
    NODE* curNode = curStateNode;
    if(curNode == nullptr){
        return false;
    }else{
    
        key = curNode->Key; //Assign current node's key to reference variable 
        
        //Check if the current node is threaded and there is no Left node
        if(curNode->Left == nullptr && curNode->isThreaded )
{
            curNode = curNode->Right;
        }
        else if(curNode->Left == nullptr && curNode->isThreaded == false) //Check if the current node is not threaded and there is no Left node

{
            curNode = curNode->Right;
            while(curNode->Left != nullptr)//traverse left 
			{                 
				curNode= curNode->Left;
            }
        }
        else if(curNode->Left != nullptr && curNode->isThreaded == false)  //Check if the current node is not threaded and there exists Left node
{
                curNode= curNode->Right; 
                while(curNode->Left != nullptr){ //traverse left
                    curNode= curNode->Left;
                }
        }
        else if(curNode->Left != nullptr && curNode->isThreaded )  //Check if the current node is threaded and there exists Left node
{
                curNode= curNode->Right;
        }

        curStateNode = curNode; //assign current node as current state node
        return true;
        
    }
    return false;
  }

  //
  // dump
  // 
  // Dumps the contents of the tree to the output stream, using a
  // recursive inorder traversal.
  //
  void dump(ostream& output) const
  {
    output << "**************************************************" << endl;
    output << "********************* BSTT ***********************" << endl;

    output << "** size: " << this->size() << endl;

    //
    // inorder traversal, with one output per line: either 
    // (key,value) or (key,value,THREAD)
    //
    // (key,value) if the node is not threaded OR thread==nullptr
    // (key,value,THREAD) if the node is threaded and THREAD denotes the next inorder key
    //

    printNodes_InOrder(Root,output);
    output << "**************************************************" << endl;
  }
	
};