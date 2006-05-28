/*
Class: wxOperandBinaryTree
File: wxoperandbinarytree.h
Author Víctor Alejandro Gil Sepúlveda
From: Spain
Finishing Date: 14-2-02
Definition: See Docs
*/

#ifndef WXOPERANDBINARYTREE_H
#define WXOPERANDBINARYTREE_H

#include <wx/string.h>
#include <iostream>

using namespace std;
     
    enum operation
    {
         noOp,
         opAdd ='+',
         opMul ='*',
         opSub='-',
         opDiv='/',
         opPow='&',
         opPow2='#'
    };
    
     struct number
     {
        double value;
        wxString unit;
        double unit_e;
     }; 
     
     struct wxOBTNode
     {
        number nu;
        operation op;
        wxOBTNode* Right;
        wxOBTNode* Left;
        wxOBTNode* father;   
     };   
              
class wxOperandBinaryTree
{
	public:
	
		                wxOperandBinaryTree      (void);
	                    ~wxOperandBinaryTree     (void);
	   wxOBTNode*       NavigateLeft             (void);
	   wxOBTNode*       NavigateRight            (void);
	   wxOBTNode*       NavigateUp               (void);
	   wxOBTNode*       SetInitial               (operation op);
	   wxOBTNode*       SetInitial               (wxOBTNode* n);
	   wxOBTNode*       InsertLNode              (number& nu);
	   wxOBTNode*       InsertRNode              (number& nu);
	   wxOBTNode*       GetInitial               (void);
	   void             Print                    (wxOBTNode* n);
	   
	private:
         void           Delete                    (wxOBTNode* n);
         
         wxOBTNode* initial_node;
         wxOBTNode* current;  
};

#endif 
