/*
Class: wxOperandBinaryTree
File: wxoperandbinarytree.cpp
Author Víctor Alejandro Gil Sepúlveda
From: Spain
Finishing Date: 14-2-02
Definition: See Docs
*/
#include "wxoperandbinarytree.h" 


wxOperandBinaryTree::wxOperandBinaryTree()
{
    initial_node=NULL;
    current=NULL;
}


wxOperandBinaryTree::~wxOperandBinaryTree()
{
    //delete all nodes
    Delete(initial_node);
}

void wxOperandBinaryTree::Delete(wxOBTNode* n){
    if(n!=NULL){
        Delete(n->Right);
        Delete(n->Left);       
        delete n;
    }
}

wxOBTNode* wxOperandBinaryTree::NavigateLeft(void){
    if(current->Left!=NULL)
        current=current->Left;
    return current;   
}

wxOBTNode* wxOperandBinaryTree::NavigateRight(void){
    if(current->Left!=NULL)
        current=current->Right;
    return current;   
}

wxOBTNode* wxOperandBinaryTree::NavigateUp(void){
    if(current->father!=NULL)
        current=current->father;
    return current;   
}

wxOBTNode* wxOperandBinaryTree::GetInitial(void) {
 current=initial_node;   
 return initial_node;   
}

wxOBTNode* wxOperandBinaryTree::SetInitial (operation op){
 wxOBTNode* n= new wxOBTNode;
 n->father=NULL;
 n->nu.value=0;
 n->nu.unit=_T("");
 n->nu.unit_e=0;
 n->op=op;
 n->Left=NULL;
 n->Right=NULL;
 initial_node=n;
 current=n;
 return initial_node;   
}
wxOBTNode* wxOperandBinaryTree::SetInitial (wxOBTNode* n){
 Delete(initial_node);
 initial_node=n;
 return initial_node;   
}
wxOBTNode* wxOperandBinaryTree::InsertLNode (number& nu){
 wxOBTNode* n= new wxOBTNode;
 n->father=current;
 current->Left=n;
 n->nu.value=nu.value;
 n->nu.unit=nu.unit;
 n->nu.unit_e=nu.unit_e;
 n->op=noOp;
 n->Left=NULL;
 n->Right=NULL;
  return n;   
}

wxOBTNode* wxOperandBinaryTree::InsertRNode (number& nu){
 wxOBTNode* n= new wxOBTNode;
 n->father=current;
 current->Right=n;
 n->nu.value=nu.value;
 n->nu.unit=nu.unit;
 n->nu.unit_e=nu.unit_e;
 n->op=noOp;
 n->Left=NULL;
 n->Right=NULL;
  return n;   
}

void wxOperandBinaryTree::Print(wxOBTNode* n)
{
    if(n!=NULL){   
        cout<<"("<<(char)n->op<<" "<<n->nu.value<<" "<<n->nu.unit<<n->nu.unit_e<<")";
        Print(n->Left);
        Print(n->Right);
    }
    else
    cout<<"(0)";
}
