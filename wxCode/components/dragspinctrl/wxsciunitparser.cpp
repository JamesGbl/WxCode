/*
Class: wxSciUnitParser
File: wxsciunitparser.cpp
Author Víctor Alejandro Gil Sepúlveda
From: Spain
Finishing Date: 14-2-02
Definition: See Docs
*/

#include "wxsciunitparser.h" 
#include <wx/listimpl.cpp>





WX_DEFINE_LIST(wxConversionArray)
WX_DEFINE_LIST(wxConversionArrayList)
WX_DEFINE_LIST(wxStatesStack)
WX_DEFINE_LIST(wxStringArray)

inline bool FindInStringArray(wxString s, wxStringArray* sa){
   wxStringArray::Node *node = sa->GetFirst();

        while(node){
            if(s==*(node->GetData())){
                return true;    
            }
            node= node->GetNext();   
        }  
        
        return false;
}
inline bool DeleteInStringArray(wxString s, wxStringArray* sa){
    wxStringArray::Node *node = sa->GetFirst();

        while(node){
            if(s==*(node->GetData())){
                return sa->DeleteNode(node);    
            }
            node= node->GetNext();   
        }  
        
        return false;
}
inline void AppendToStringArray(wxStringArray* one,wxStringArray* two){
     wxStringArray::Node *node = one->GetFirst();
     
     if(one!=NULL && two != NULL){
       while(node){
            two->Append(node->GetData());
            node= node->GetNext();   
        }     
     }
}

inline void PrintStringArray(wxStringArray* as){
    wxStringArray::Node *node;
    node = as->GetFirst();
    cout <<"[String Array:]"<<endl;
     while(node){
            cout<<*(node->GetData())<<" ";
            node= node->GetNext();   
     }
     cout<<endl;
    
}
    
void wxSciUnitParser::print_conversions()
{
     wxConversionArrayList::Node *node1 = conversions.GetFirst();
     wxConversionArray::Node *node2 ; 
     
     while (node1 )
      {
            node2=node1->GetData()->GetFirst();
                    while(node2)
                    {
                      cout<<*(node2->GetData())<<" ";
                      node2 = node2->GetNext();          
                    }    
          cout<<endl;
          node1 = node1->GetNext();
      }
}





// class constructor
wxSciUnitParser::wxSciUnitParser()
{
    options=wxSUP_DEFAULT;
    units.DeleteContents(true);
    conversions.DeleteContents(true);
    
	options=options|wxSUP_USE_COMMA;
	

}

wxSciUnitParser::wxSciUnitParser( char op)
{
    options=op;
    units.DeleteContents(true);
    conversions.DeleteContents(true);
	
	options=options|wxSUP_USE_COMMA;

}

// class destructor
wxSciUnitParser::~wxSciUnitParser()
{

}





void wxSciUnitParser::InsertConversion(wxConversion& unit,wxConversion& equiv)
{

    wxConversion *c1,*c2;
    wxConversionArray* ca;
    wxConversionArrayList::Node *node1 = conversions.GetFirst();
    wxConversionArray::Node *node2 ;
    
    //Preformat the strings
    
    //Any kind of preformating should be written here
    
    if(unit.GetOp()!=cOpInvalid && equiv.GetOp()!=cOpInvalid){
        if(unit.GetOp()!= equiv.GetOp()){ 
            cout<<"ERROR: The ops of two conversions must be the same"<<endl;
            return;
        }
        
        //Prepare new conversions
        c1=new wxConversion(unit);
        c2=new wxConversion(equiv);
        
        // Precondition: unit and equiv aren't the same
        // A unit cannot be converted to itself, well it can,
        // but it's useless.
        if(unit == equiv) return;
        
        
        
        //Insert the conversion in the conversion list
        
          while (node1)
          {
                node2=node1->GetData()->GetFirst();
                
                 if(*(node2->GetData())== *c1)
                 { 
                        //see if the conversion is already defined
                        
                        while(node2)
                        {
                          if(*(node2->GetData())== *c2)
                          {
                          // unit and  equiv were already in the list
                           delete c1;
                           delete c2;
                           return;   
                          }
                          node2 = node2->GetNext();          
                        }    
                        // unit is defined, but equiv isn't 
                        delete c1;
                        node1->GetData()->Append(c2);
                       
                        if(!FindInStringArray(c2->GetUnit(),&units)){
                             units.Append(new wxString(c2->GetUnit()));
                        }
                        
                        return;          
                 }
                 
              node1 = node1->GetNext();
          }
        
           // unit and equiv are undefined, so we have to insert them
           ca = new wxConversionArray;
           ca->DeleteContents(true);
           node1=conversions.Append(ca);
           node1->GetData()->Append(c1);
           node1->GetData()->Append(c2);
           
           
           if(!FindInStringArray(c1->GetUnit(),&units)){
                    units.Append(new wxString(c1->GetUnit()));
           }
           if(!FindInStringArray(c2->GetUnit(),&units)){
                    units.Append(new wxString(c2->GetUnit()));
            }
    }
}

void wxSciUnitParser::SetProperties(char prop){
  options = prop;   
}

bool wxSciUnitParser::IsPropEnabled (char prop)const{
     return (int)(options&prop) !=0;     
}

 int wxSciUnitParser::Parse(wxString astp, wxString& sp,wxString unit){
  int error,i,j,k;
  bool defaulting;
  char c1,c2;
  wxStatesStack states;
  wxOBTNode** obtn;
  
  c1=',';
  c2='.';
  
/**********************************/
//print_conversions();
/*********************************/


  //PREPROCESS:
                
                //Finally a scup without conversions is accepted   
                
                                
                
                
               //eliminate spaces
               i=0;
               while(i<(int)astp.Len())
               {
                if(astp.GetChar(i) == ' ')
                {
                 astp.Remove(i,1);
                }
                else
                {
                 ++i;   
                }                                          
               }
             
               // there is no need on parsing a empty string
               if(astp.IsEmpty())
               {
                    //fix, an empty string is defaulted to 0
                     sp= _T("0")+unit;
                    ////////////////////////////////////////
                    return wxSUP_NO_ERROR;
               }
          
               for(i=0;i<(int)astp.Len();++i)
               {
                 if(astp.GetChar(i)==',')
                 {
                  
                    (astp.GetWritableChar(i))='.';  
                  
                 }
              }
	          
              //A unitless string will be defaulted to the base unit
               j=0;
               for(i=0;i<(int)astp.Len();++i)
               {
                 if(is_letter(astp.GetChar(i)))
                 {
                  j++;                
                 }
               }
               if(j==0) // we have an unitless string
               {
                astp=_T("(")+astp+_T(")*1")+unit;          
               }
               

              // ^2 ^3 beautified cases
               for(i=0;i<(int)astp.Len();++i)
               {
                 if(astp.GetChar(i) == wxChar(0x00B2))
                 {
                    astp= astp.Mid(0,i)+_T("^2")+astp.Mid(i+1);   
                          
                 }
                 if(astp.GetChar(i)== wxChar(0x00B3))
                 {
                    astp= astp.Mid(0,i)+_T("^3")+astp.Mid(i+1);                
                                     
                 }
               }


              // E e issues
              for(i=0;i<(int)astp.Len();++i)
               {
                
                 if(  astp.GetChar(i)=='e' )
                 {
                     k=0;i++;
                     for(j=i; j<(int)astp.Len() && !is_letter(astp.GetChar(j)) ;++j)
                     {
                      if( astp.GetChar(j)=='+' || astp.GetChar(j)=='-'){
                         if(astp.GetChar(j)=='-'){
                               k=1;                         
                         }
                         astp.Remove(j,1);     
                      }      
                     }
                     if(k==0)
                         astp = 
                         astp.Mid(0,i-1)
                         +_T("*(10^")
                         + astp.Mid(i,j-i)
                         +_T(")")
                         +astp.Mid(j)
                         ;
                     else
                         astp = 
                         astp.Mid(0,i-1)
                         +_T("*(1/(10^")
                         + astp.Mid(i,j-i)
                         +_T("))")
                         +astp.Mid(j)
                         ;
                 }
              }     
               
               
               //solve x^a^b^c^... situations :x^a*b*c*...
               if(!shrink_exponents(astp,0))
               {
                return wxSUP_EXPONENT_ERROR;
               }
            
               // there won't be three operators together
               for(i=0;i<(int)astp.Len()-2;++i)
               {
                 if(astp.GetChar(i)=='+' || astp.GetChar(i)=='-'
                    || astp.GetChar(i)=='*' || astp.GetChar(i)=='/')
                 {
                  if(astp.GetChar(i+1)=='+' || astp.GetChar(i+1)=='-'
                     || astp.GetChar(i+1)=='*' || astp.GetChar(i+1)=='/')
                  {
                   if(astp.GetChar(i+2)=='+' || astp.GetChar(i+2)=='-'
                      || astp.GetChar(i+2)=='*' || astp.GetChar(i+2)=='/')
                   {
                      return wxSUP_OP_FORMAT_ERROR;
                   }
                  }
                 }
               }
              
               // an operation can't have doubled operators
               for(i=0;i<(int)astp.Len()-1;++i)
               {
                 if(astp.GetChar(i)=='+' || astp.GetChar(i)=='-'
                 || astp.GetChar(i)=='*' || astp.GetChar(i)=='/')
                 {
                  if(astp.GetChar(i+1)==astp.GetChar(i))
                  {
                   return wxSUP_OP_FORMAT_ERROR;
                  }
                 }
              }
            
              // a+-b or a-+b are in fact a-b 
              for(i=0;i<(int)astp.Len()-1;++i)
               {
                 if(astp.GetChar(i)=='+' || astp.GetChar(i)=='-')
                 {
                  if(astp.GetChar(i+1)=='+')
                  {
                   astp.Remove(i+1,1);
                  }
                  if(astp.GetChar(i+1)=='-')
                  {
                   astp.Remove(i,1);
                  }
                 }
              }
            
              // *+ /+ are in fact * or /
              for(i=0;i<(int)astp.Len()-1;++i)
               {
                 if(astp.GetChar(i)=='*' || astp.GetChar(i)=='/')
                 {
                  if(astp.GetChar(i+1)=='+' )
                  {
                   astp.Remove(i+1,1);
                  }
                 }
              }
             
              //all other combinations are wrong unless /- or *-
              for(i=0;i<(int)astp.Len()-1;++i)
               {
                 if( astp.GetChar(i)=='+' || astp.GetChar(i)=='-'
                    || astp.GetChar(i)=='*' || astp.GetChar(i)=='/')
                 {
                  if(astp.GetChar(i+1)=='*' || astp.GetChar(i+1)=='/')
                  {
                    return wxSUP_OP_FORMAT_ERROR;
                  }
                 }
              }        
                  
              // + in first position has no sense  
              if(astp.GetChar(0)=='+')
              {
                      astp=astp.Remove(0,1);
              }
              
              // however * and / are wrong 
              if(astp.GetChar(0)=='*' || astp.GetChar(0)=='/')
              {
                      return wxSUP_OP_FORMAT_ERROR;
              }
              
              // and any operator at the end has no sense 
             if(is_op(astp.Last())){
               return wxSUP_OP_FORMAT_ERROR;     
             }
              
              //change )^ to )& and ^( to &( and \( for operational purposes
              for(i=0;i<(int)astp.Len()-1;++i)
               {
                 if(astp.GetChar(i)==')')
                 {
                  if(astp.GetChar(i+1)=='^' )
                  {
                   astp.GetWritableChar(i+1)='&';
                  }
                 }
                 if(astp.GetChar(i)=='^')
                 {
                  if(astp.GetChar(i+1)=='(' )
                  {
                   astp.GetWritableChar(i)='#';
                  }
                 }
              }
               //All opening brackets have ending brackets
              for(i=0;i<(int)astp.Len();++i)
               {
                 if(astp.GetChar(i)=='(')
                 {
                    obtn = new wxOBTNode*;
                    *obtn= (wxOBTNode*)waiting_bracket;
                    states.Insert(obtn);
                 }
                 if(astp.GetChar(i)==')')
                 {
                    if(states.GetCount()==0) return wxSUP_BRACKET_FORMAT_ERROR;
                    states.DeleteNode(states.GetFirst());
                 }
              }
              
              if(states.GetCount()!=0) return wxSUP_BRACKET_FORMAT_ERROR;
              
              for(i=0;i<(int)astp.Len()-1;++i)
               {
                 if(  astp.GetChar(i)==')')
                 {
                  if(is_letter(astp.GetChar(i+1)))
                  {
                    astp=astp.Mid(0,i+1)+_T("*")+astp.Mid(i+1);
                    
                  }
                 }
              }     
              
              
             
              
              // something TODO here about brackets?
              //insert needed brackets
              for(i=0;i<(int)astp.Len();++i)
               {
                 if(astp.GetChar(i)=='*' || astp.GetChar(i)=='/' ||
                    astp.GetChar(i)=='&' || astp.GetChar(i)=='#')
                 {
                     
                        ++i; 
                        if(astp.GetChar(i)!='('){
                            for(j=i;j<(int)astp.Len() && !is_op(astp.GetChar(j));++j){} 
                            
                            astp=astp.Mid(0,j)+_T(")")+astp.Mid(j);
                        }
                        else{
                            for(j=i;j<(int)astp.Len() && astp.GetChar(j)!=')';++j){} 
                            
                            astp=astp.Mid(0,j)+_T(")")+astp.Mid(j);
               
                        }   
                        
                        if(astp.GetChar(i-2)!=')'){
                            for(j=i-2;j>=0 && (!is_op(astp.GetChar(j))&& 
                                    astp.GetChar(j)!='(');--j){}
                                     astp=astp.Mid(0,j+1)+_T("(")+astp.Mid(j+1);
                        }
                        else{
                            for(j=i-2;j>=0 && astp.GetChar(j)!='(';--j){}
                                     astp=astp.Mid(0,j+1)+_T("(")+astp.Mid(j+1);
                        }
                 }
             }
            
            // Eliminate () or (number) cases  which are unnecesary
            for(i=0;i<(int)astp.Len();++i)
               {
                 if(astp.GetChar(i)=='(')
                 {
                  k=i;
                  ++i; 
                      
                  for(j=i;j<(int)astp.Len() && !is_op(astp.GetChar(j))
                  && astp.GetChar(j)!=')';++j){}
                  if(astp.GetChar(j)==')'){astp=astp.Remove(j,1);astp=astp.Remove(i-1,1);i=k-1;}
                  
                 }
              }
              
             
              //No 'op)' or '(op' (unless -)  cases are permited
              for(i=0;i<(int)astp.Len()-1;++i)
               {
                 if(is_op(astp.GetChar(i)))
                 {
                  if(astp.GetChar(i+1)==')' )
                  {
                   return wxSUP_OP_FORMAT_ERROR;
                  }
                 }
                 if(astp.GetChar(i)=='(')
                 {
                  if(is_op(astp.GetChar(i+1)) && astp.GetChar(i+1)!='-')
                  {
                   return wxSUP_OP_FORMAT_ERROR;
                  }
                 }
              }
              
              //NEW:defaulting to the unit
              // if all the string is a number
              defaulting = true;
              for(i=0;i<(int)astp.Len();++i)
              {
                    defaulting&=is_number(astp.GetChar(i));
              }
              if(defaulting){
                     astp=astp+unit;
                    
              }
              //ENDNEW
              
              // And , very important
              // Any parsing need to have at least one operation
              // (the tree wouldn't have root then)
              
              astp=_T("0+")+astp;//CHANGED *****
             
           
  //PROCESS:
  error =_Parse(astp,sp,unit);
  
  //POSTPROCESS:
             // eliminate .00 cases 
             delete_point_zeros (sp);
            
             //  eliminate ^1 cases
             for(i=0;i<(int)sp.Len()-1;++i)
             {
                 if(sp.GetChar(i)=='^')
                 {
                  if(sp.GetChar(i+1)=='1')
                  {
                   if(i+2<(int)sp.Len())
                   {
                   
                       if(!is_number(sp.GetChar(i+2)))
                       {
                           sp.Remove(i,2);
                       }
                   }
                   else
                   {
                       sp=sp.Remove(i,2);
                   }
                  }
                 }
             }
            
            
            
             for(i=0;i<(int)sp.Len()-1;++i)
             {
                 if(sp.GetChar(i)=='^')
                 {
                 
                  if(sp.GetChar(i+1)=='2')
                  {
                        
                    if(IsPropEnabled (wxSUP_USE_RICH_OUTPUT)){
                        sp=sp.Remove(i,1);
                        sp.GetWritableChar(i)=wxChar(0x00B2);
                        #if defined (__WXGTK__)
                           wxString str((char*)sp.GetData(), wxConvUTF8);
                           sp=str;
                        #endif
                    }
                  }
                 }
                 
                 if(sp.GetChar(i)=='^')
                 {
                 
                  if(sp.GetChar(i+1)=='3')
                  {
                        
                    if(IsPropEnabled (wxSUP_USE_RICH_OUTPUT)){
                        sp=sp.Remove(i,1);
                        sp.GetWritableChar(i)=wxChar(0x00B3);
                        #if defined (__WXGTK__)
                         wxString str((char*)sp.GetData(), wxConvUTF8);
                         sp=str;
                        #endif
                    }
                  }
                 }
             }
            
  return error; 
 }
  

/*void wxSciUnitParser::printSS(wxStatesStack* s){
 wxStatesStack::Node *node = s->GetFirst(); 
 cout <<"---STACK----"<<endl;
 while(node){
    switch ((int)*(node->GetData())){
      case  waiting_bracket:
        cout<<"Waiting Bracket"<<endl;
      break; 
      case  waiting_operation:
        cout<<"Waiting Operation"<<endl;
      break; 
      case  waiting_operand:
        cout<<"Waiting Operand"<<endl;
      break;
      default:
         cout<<*(node->GetData())<<endl;     
    }       
    node = node->GetNext();    
 }  
 cout<<"----------"<<endl;
}*/

/////////////////////////////////////////////////////////////////////
//Uncommenting the opbt.Print() and PrintSS will print the final tree
//and the stack respectively

 int wxSciUnitParser::_Parse(wxString astp, wxString& sp,wxString unit){
    number n;
    unsigned int i=0;
    number nu;
    wxOperandBinaryTree opbt;
    wxStatesStack states;
    wxOBTNode** obtn;
    wxOBTNode *on,*onp;
    wxString operand;
    
    
     on=NULL;
    // First of all, we are in this state: waiting_operand
    // It's used only to see if any operation was done
    obtn = new wxOBTNode*;
    *obtn= (wxOBTNode*)waiting_operand;
    states.Insert(obtn);
    
    // init the parse
    // it runs like a finite state machine with stack



    while(i<astp.Len() && states.GetCount()!=0){
        if(astp.GetChar(i)=='('){
           
            //printSS(&states);
            obtn = new wxOBTNode*;
            *obtn= (wxOBTNode*)waiting_bracket;
            states.Insert(obtn);
            // and we can start waiting for another operand
        }
        
        if(astp.GetChar(i)==')'){
            
            //printSS(&states);
            obtn = states.GetFirst()->GetData();
            if( (*obtn== (wxOBTNode*)waiting_operand ||*obtn==(wxOBTNode*)waiting_operation)&& states.GetCount()>1){
                 
                // extract the number...
                number_extractor(operand,nu);
                // set operand to an empty value
                operand=_T("");
                // create a node
                on = new wxOBTNode;
                on->father=NULL;on->Left=NULL;on->Right=NULL;on->op=noOp;
                on->nu.value=nu.value;on->nu.unit=nu.unit;on->nu.unit_e=nu.unit_e;
                // or we use one created tree it the state was waiting operation
                if(*obtn==(wxOBTNode*)waiting_operation){
                    states.DeleteNode(states.GetFirst());
                    on = * (states.GetFirst()->GetData());
                    states.DeleteNode(states.GetFirst());
                    obtn = states.GetFirst()->GetData();
                }
                
                while((*obtn== (wxOBTNode*)waiting_operand||*obtn==(wxOBTNode*)waiting_operation) && states.GetCount()>1){
                    
                    states.DeleteNode(states.GetFirst());
                    // extract the pointer and...
                    obtn = states.GetFirst()->GetData();
                    // ...put operand on the right
                    (*obtn)->Right=on; on->father=*obtn;
                    states.DeleteNode(states.GetFirst());
                    // then we work with the extracted operand
                    on=*obtn;
                    obtn = states.GetFirst()->GetData();
                  
                }
                
            }
                        
            if(*obtn==(wxOBTNode*)waiting_bracket ){
                states.DeleteNode(states.GetFirst()); // waiting brackets
                // extract...
                // and insert the pointer of last tree node
                obtn = new wxOBTNode*;
                *obtn= on;
                states.Insert(obtn);
                // insert a waiting_operation 
                obtn = new wxOBTNode*;
                *obtn= (wxOBTNode*)waiting_operation;
                states.Insert(obtn);
            }
            else{
              //we were not waiting for any bracket
              return wxSUP_OP_FORMAT_ERROR;   
            }     
        }
        
        if(is_op(astp.GetChar(i))){
           
            //printSS(&states);
            obtn = states.GetFirst()->GetData();
            
            if( *obtn== (wxOBTNode*)waiting_operand || *obtn== (wxOBTNode*)waiting_bracket){
                // only if is the first one (which has no pointer)
                if(states.GetCount()==1)
                states.DeleteNode(states.GetFirst()); 

                // extract the number
                number_extractor(operand,nu);
                operand=_T("");
                // create the mini tree
                on = new wxOBTNode;
                on->father=NULL;on->Left=NULL;on->Right=NULL;on->op=noOp;
                on->nu.value=nu.value;on->nu.unit=nu.unit;on->nu.unit_e=nu.unit_e;
                
                onp = new wxOBTNode;
                onp->father=NULL;onp->Left=on;onp->Right=NULL;onp->op=(operation)astp.GetChar(i);
                onp->nu.value=0;onp->nu.unit=_T("");onp->nu.unit_e=0;
                
                on->father=onp;
                // insert in the stack the father node direction
                obtn = new wxOBTNode*;
                *obtn= onp;
                states.Insert(obtn);
                
                obtn = new wxOBTNode*;
                *obtn= (wxOBTNode*)waiting_operand;
                states.Insert(obtn);
            
            }
            else{
                if( *obtn== (wxOBTNode*)waiting_operation){
                    states.DeleteNode(states.GetFirst()); 
                    // we were waiting an operand to finish
                    // create the op node
                    
                    onp = new wxOBTNode;
                    onp->father=NULL;onp->Left=on;onp->Right=NULL;onp->op=(operation)astp.GetChar(i);
                    onp->nu.value=0;onp->nu.unit=_T("");onp->nu.unit_e=0;
                    // insert the node on the left...
                    obtn = states.GetFirst()->GetData();
                    onp->Left=*obtn;
                    states.DeleteNode(states.GetFirst());
                    // insert the node in the stack
                    obtn = new wxOBTNode*;
                    *obtn= onp;
                    states.Insert(obtn);
                    // and insert a waiting_operand state
                    obtn = new wxOBTNode*;
                    *obtn= (wxOBTNode*)waiting_operand;
                    states.Insert(obtn);
                }
                else
                {
                    //in fact it's impossible to be here, but...
                   
                    return wxSUP_OP_FORMAT_ERROR;
                             
                }
            }
        }
        
        if(is_number(astp.GetChar(i))||is_letter(astp.GetChar(i))
            ||astp.GetChar(i)=='^'){
                
            operand.Append(astp.GetChar(i));
        }
        
        //any other character is totally ignored or treated as a unit
        ++i;
    }
     // The parsing can end needing a new operand or waiting for a new operator
     // In the first case we provide it, in the second we can just start redoing the tree
     // The tree is redone assembling the nodes to the right child of his father
      
     
      obtn = states.GetFirst()->GetData();
     
      //Parse the last number if it's needed
      //The stack has surely one or more states but we want to avoid any problem
      //If we were waiting for an operand, we provide it
      if(states.GetCount()>1 && *obtn== (wxOBTNode*)waiting_operand){ 
        if(!number_extractor(operand,nu)) return wxSUP_EXTRACTION_ERROR;
        // Delete the operand string
        operand=_T("");
        // Create the node
        on = new wxOBTNode;
        on->father=NULL;on->Left=NULL;on->Right=NULL;on->op=noOp;
        on->nu.value=nu.value;on->nu.unit=nu.unit;on->nu.unit_e=nu.unit_e;
      }
      
     if(states.GetCount()>1){
         
         while(*obtn== (wxOBTNode*)waiting_operand ||
                *obtn== (wxOBTNode*)waiting_operation){
            
            if( *obtn== (wxOBTNode*)waiting_operand){
                states.DeleteNode(states.GetFirst());
                // extract the pointer and...
                obtn = states.GetFirst()->GetData();
                // put the operand on the right
                (*obtn)->Right=on; on->father=*obtn;
                states.DeleteNode(states.GetFirst());
                // then work with the extracted pointer
                on=*obtn;
            }
            else
            {
                //we cannot provide more operations!!
                states.DeleteNode(states.GetFirst());
                states.DeleteNode(states.GetFirst());    
            }
            
            if(states.GetCount()>1)
            obtn = states.GetFirst()->GetData();
        }    
       
       opbt.SetInitial(*obtn);
       /***************************************************************/
       //opbt.Print(opbt.GetInitial()); 
       /***************************************************************/     
     } 
    
    
    
    
    if(!(i>=astp.Len()&& states.GetCount()==0)){
                         //cout<<"error de estados i:"<<i<<" astp.Len():"<<astp.Len()<<" num estados: "<<states.GetCount() <<endl;                       
                         return wxSUP_OP_FORMAT_ERROR;
     }
    
    
    //do operations
        opbt.GetInitial()->father = (wxOBTNode*)0xFFFFFFFF;
        if(!Add(opbt.GetInitial(),unit)){
                
                 return wxSUP_OP_ERROR;
        }
        
        if(IsPropEnabled (wxSUP_USE_RICH_OUTPUT)){
                 sp=wxString::Format(_T("%g"),opbt.GetInitial()->nu.value)+_T(" ")+opbt.GetInitial()->nu.unit;
        }
        else{
                 sp=wxString::Format(_T("%g"),opbt.GetInitial()->nu.value)+opbt.GetInitial()->nu.unit;
        }         
    //
    
    return wxSUP_NO_ERROR;      
 }


wxString withcomma(wxString s){
         if(s.Find('.')!=-1){
              s.Replace(_T("."),_T(","));
         }
         else{
              s.Replace(_T(","),_T("."));
         }                    
         return s;
         
}

//Precondition: The string can't have any of these characters
// { /,*,+,(,),& } and if it has -, it's in the first place

bool wxSciUnitParser::number_extractor(wxString s,number& n)
{
    unsigned int i,j;
    double a,e;
    bool has_number=true;
    
    j=0;
    //cout<<"Extracting..."<<s<<endl;
    //extract the number
    for(i=0;i<s.Len() && is_number(s.GetChar(i));++i){}
    //if there is a number...
    if(i!=0){
        if(i==1 && s.GetChar(0)=='-'){            
            a=-1;
            j=i; 
        }
        else{
	    
            if(!s.Mid(0,i).ToDouble(&a)){
                    if(!(withcomma(s).Mid(0,i).ToDouble(&a))){                    
                                         cout <<"ERROR de extraccion "<<s.Mid(0,i)<<endl; 
	                                     return false;
                    }
            }
            //see if it has an exponent
            if(s.GetChar(i)=='^'){
               for(j=i+1;j<s.Len() && (is_number(s.GetChar(j)) );++j){}
               // if doesn't have the exponent, error
               if(i+1==j){cout <<"ERROR  ^ y no exponente "<<s.Mid(0,i)<<" "<<s.GetChar(j)<<j<<endl;
	       return false;}
               //else extract it
               if(!s.Mid(i+1,j-i-1).ToDouble(&e)){
                   if(!(withcomma(s).Mid(i+1,j-i-1).ToDouble(&a))){                    
                                         cout <<"ERROR al extraer exponente "<<s.Mid(0,i)<<endl;
	                                     return false;
                    }
               }
               //and get the number!!
               a=pow((float)a,(float)e);
               i=j;
            }
            else{
               j=i;
            }
        }
    }
    else{
     has_number=false;
    }
    
    
    //if the next it's a letter we extract the unit   
    for(;i<s.Len() && is_letter(s.GetChar(i));++i){}
    if(i==j && !has_number)return false; // uff, what's that?
    //else, if it has a unit, but not a number
    if(!has_number) a=1;
    
    n.unit = s.Mid(j,i-j);
    ////cout<<"Getting unit:"<<s.Mid(j,i-j)<<endl;
    //and we try to extract the unit exponent if has one
    if(s.GetChar(i)=='^'){
               for(j=i+1;j<s.Len() && is_number(s.GetChar(j));++j){}
               // if doesn't have the exponent, error
               if(i==j-1) return false;
               //else extract it
               if(!s.Mid(i+1,j-i-1).ToDouble(&n.unit_e)){
                     if(!(withcomma(s).Mid(i+1,j-i-1).ToDouble(&a))){                    
                                         cout <<"ERROR al extraer exponente "<<s.Mid(0,i)<<endl;
	                                     return false;
                     } 
               }
    }
    else
    {
        n.unit_e=1;
    }

    n.value=a;
    //if we parsed all the string all was a success :) 
    //cout<<"Number extraction: "<<n.value<<" "<<n.unit<<" "<<n.unit_e<<endl; 
    return j>=s.Len() || i>=s.Len();  
}

bool wxSciUnitParser::shrink_exponents (wxString& x, unsigned int pos )
{
   unsigned int j,h,k;
   double e,e2;
   
  
     for(j = pos;x.GetChar(j)!='^' && j<x.Len();++j){}
     
     // no '^' character was found
     if(j>=x.Len())
      {
       ////cout<<"No hay ningun ^en : "<<x<<" "<<pos<<endl;
       return true;
      }
     
     for(h = j+1;x.GetChar(h)!='^' && h<x.Len();++h){}
     
     // no another '^' character was found, so there's no problem
     // with exponents
      if(h>=x.Len())
      {
       ////cout<<"Hay ^pero no se encuentra otro : "<<x<<" "<<pos<<endl;
       return true;
      }
      
      // check if the string betwen the '^'s is a number or not
      if(!(x.Mid(j+1,h-1-j).ToDouble(&e)))
      {
       // it's not a number (impossible to convert)
       // then we continue parsing the string
          if(!(withcomma(x).Mid(j+1,h-1-j).ToDouble(&e))){
              return shrink_exponents(x,h);
          }
      }
      else
      {
       //it was a number,so we try to solve the power   
       //and then continue parsing
       
       //we try to find the second exponent
       for(k = h+1;x.GetChar(k)!='^' && x.GetChar(k)<58 && k<x.Len();++k){}
       
       //there was no second exponent but there was a '^'
       if(h+1==x.Len())
       {
        ////cout<<"El segundo exponente no tiene numero  : "<<x<<" "<<pos<<endl;
        return false;
       }
       
       if(!(x.Mid(h+1,k-1-h).ToDouble(&e2)))
       {
                     if(!(withcomma(x).Mid(h+1,k-1-h).ToDouble(&e2)))        
                             return false; // there was a problem 
       }
       
       e=e*e2; //the new exponent
       

      
       //and add the new exponent
       
       x=x.Mid(0,j+1)+wxString::wxString::Format(_T("%g"),(float)e)+x.Mid(k);
       
       //and continue shrinking
       ////cout<<"Sin problemas: "<<x<<" "<<pos<<endl;
       return shrink_exponents(x,j);
       
       
      }
      
      ////cout<<"Sin problemas: "<<x<<" "<<pos<<endl;
      return true;
}


void  wxSciUnitParser::simplify_mul  (wxString& x)
{
      wxString dividend,divisor,auxdivisor,auxdividend;
      int pos;
      double e;
      unsigned int i,j,k,h;
      number u;     // we are going to use number as a power,
                    // been value the exponent and unit the... unit 
      
      //extract dividend and divisor(if any)
      pos=x.Find('/');
      dividend=x.Mid(0,pos);
      if(pos>0)
      divisor=x.Mid(pos+1);
      
      k=0;
      //cout <<"Simplificar Mult: "<<x<<" Dividend: "<<dividend<<" Divisor: "<<divisor<<endl;
      while(k<dividend.Len())
      {
           //changed =false;
           for(i=k;is_letter(dividend.GetChar(i)) && i<dividend.Len();++i){}
          
           u.unit=dividend.Mid(k,i-k);
           
           
           
           //if it has exponent
           if(dividend.GetChar(i)=='^')
           {
            j=i;
            for(i=i+1;((dividend.GetChar(i)<58 && dividend.GetChar(i)>47)
                       ||dividend.GetChar(i)==44 ||dividend.GetChar(i)==46)&& i<dividend.Len();++i){}
            if(!dividend.Mid(j+1,i-j+1).ToDouble(&u.value))
                withcomma(dividend).Mid(j+1,i-j+1).ToDouble(&u.value);
           }
           else
           {
             u.value=1;
           }
           
           
           //the unit has been taken
           k=i+1;
           
          //find the other units that match with the one in u 
          while(i+1<dividend.Len())
          {
              h=i;
               for(i=i+1;is_letter(dividend.GetChar(i)) && i<dividend.Len();++i){}
               //if matches...
               //cout<<"Matching... "<<dividend.Mid(h+1,i-h-1)<<endl;
               if(dividend.Mid(h+1,i-h-1).IsSameAs(u.unit))
               {
                   //extract the exponent
                   j=i;
                   if(dividend.GetChar(i)=='^' && !i<dividend.Len())
                   {
                    for(i=i+1;((dividend.GetChar(i)<58 && dividend.GetChar(i)>47)
                       ||dividend.GetChar(i)==44 ||dividend.GetChar(i)==46)&& i<dividend.Len();++i){}
                    if(!dividend.Mid(j+1,i-j+1).ToDouble(&e))
                        withcomma(dividend).Mid(j+1,i-j+1).ToDouble(&e);
                   }
                   else
                   {
                     e=1;
                   }
                    //and add it to u exponent
                    u.value+=e;
                    
                   //delete the whole unit + exponent
                   dividend=dividend.Remove(h,i-h);
               } 
          } 
          
        if(u.value!=1)
            auxdividend+=u.unit+wxString::Format(_T("^%g*"),(double)u.value); 
        else
            auxdividend+=u.unit+_T("*");
        
      }
      //the last don't need the *
      auxdividend.RemoveLast();
      
    
      
      //then if divisor exists...
      if(pos>0)
      {
          k=0;
          while(k<divisor.Len())
          {
               for(i=k;is_letter(divisor.GetChar(i)) && i<divisor.Len();++i){}
              
               u.unit=divisor.Mid(k,i-k);
               
                            
               //if it has exponent
               if(divisor.GetChar(i)=='^')
               {
                j=i;
                for(i=i+1;((divisor.GetChar(i)<58 && divisor.GetChar(i)>47)
                           ||divisor.GetChar(i)==44 ||divisor.GetChar(i)==46)&& i<divisor.Len();++i){}
                if(!divisor.Mid(j+1,i-j+1).ToDouble(&u.value))
                    withcomma(divisor).Mid(j+1,i-j+1).ToDouble(&u.value);
               }
               else
               {
                 u.value=1;
               }
               
               //the unit has been taken
               k=i+1;
               
              //find the other units that match with the one in u 
              while(i+1<divisor.Len())
              {
                  h=i;
                   for(i=i+1;is_letter(divisor.GetChar(i))&& i<divisor.Len();++i){}
                   
                   //if matches...
                   if(divisor.Mid(h+1,i-h-1).IsSameAs(u.unit))
                   {
                       //extract the exponent
                       j=i;
                       if(divisor.GetChar(i)=='^' && !i<divisor.Len())
                       {
                        for(i=i+1;((divisor.GetChar(i)<58 && divisor.GetChar(i)>47)
                           ||divisor.GetChar(i)==44 ||divisor.GetChar(i)==46)&& i<divisor.Len();++i){}
                           if(!divisor.Mid(j+1,i-j+1).ToDouble(&e))
                               withcomma(divisor).Mid(j+1,i-j+1).ToDouble(&e);
                        
                       }
                       else
                       {
                         e=1;
                       }
                      
                       //and add it to u exponent
                        u.value+=e;
                        //cout<<"Deleting..."<<endl;
                       //delete the whole unit + exponent
                       divisor=divisor.Remove(h,i-h);
                      
                   } 
              } 
            if(u.value!=1)
                auxdivisor+=u.unit+wxString::Format(_T("^%g*"),(double)u.value); 
            else
                auxdivisor+=u.unit+_T("*");
          }
          //the last don't need the *
          auxdivisor.RemoveLast();
      
      //and then try to simplify auxdividend with auxdivisor :S
       //cout<<"Simplificar fracciones: "<<auxdividend<<" "<<auxdivisor<<endl;
       simplify_div(auxdividend,auxdivisor);     
      
      }     
      if(pos>0 && !auxdivisor.IsEmpty())
      {
          
          x=auxdividend+wxString::wxString(_T("/"))+auxdivisor;
      }
      else
      {
          x=auxdividend;
      }
      
      //cout<<"Simplificacion terminada: "<<auxdividend<<" "<<auxdivisor<<endl;
      
}


void wxSciUnitParser::simplify_div(wxString& x,wxString& y)
{
      unsigned int i,j,k,h,l;
      double e;
      number u;
      wxString xaux,yaux;
      bool changed;
      
      i=k=0;
      while(k<x.Len())
      {
           changed=false;
           for(i=k;((x.GetChar(i)<91 && x.GetChar(i)>64)
                     ||(x.GetChar(i)<123 && x.GetChar(i)>96)) && i<x.Len();++i){}
          
           u.unit=x.Mid(k,i-k);
           
           //it has exponent
           
            j=i;
            if(x.GetChar(i)=='^')
            {
                for(i=i+1;((x.GetChar(i)<58 && x.GetChar(i)>47)
                           ||x.GetChar(i)==44 ||x.GetChar(i)==46)&& i<x.Len();++i){}
              
                if(!x.Mid(j+1,i-j+1).ToDouble(&u.value))
                   withcomma(x).Mid(j+1,i-j+1).ToDouble(&u.value);
            }
            else 
                u.value=1;
                
            //if the first char is a letter we have a unit
            if((x.GetChar(k)<91 && x.GetChar(k)>64)
            ||(x.GetChar(k)<123 && x.GetChar(k)>96))
            changed=true;
               
           //cout<<"**"<<endl;
           //the unit has been taken
           k=i+1;
           
          //find the other units that match with the one in u 
          //cout<<"Going to search... "<<u.unit<<" "<<u.value<< endl;
          h=j=0;
          while(j<y.Len())
          {
               //cout<<"***"<<j<<endl;          
               for(;((y.GetChar(j)<91 && y.GetChar(j)>64)
                     ||(y.GetChar(j)<123 && y.GetChar(j)>96)) && j<y.Len();++j){}
               
               //if matches...
               //cout<<"Matching... "<<y.Mid(h,j-h)<<endl;
               if(y.Mid(h,j-h).IsSameAs(u.unit))
               {
                   //extract the exponent
                                  
                    l=j;
                    
                    if(y.GetChar(l)=='^'){
                        for(j=j+1;((y.GetChar(j)<58 && y.GetChar(j)>47)
                           ||y.GetChar(j)==44 ||y.GetChar(j)==46)&& j<y.Len();++j){}
                        
                        if(!y.Mid(l+1,j-l+1).ToDouble(&e))
                           withcomma(y).Mid(l+1,j-l+1).ToDouble(&e);
                         
                        //cout<<"\t "<<y.Mid(h,j-h)<<" has exponent: "<<e<<endl;              
                     }  
                    else{
                        e = 1;
                        //cout<<"\t "<<y.Mid(h,j-h)<<" has exponent: "<<e<<endl;
                    }
                        
                    //and substract it to u exponent    
                    u.value-=e;
                    
                    //eliminate operation symbol too
                    
                    if(j<y.Len())
                      y=y.Remove(h,j-h+1);
                    else
                      y=y.Remove(h,j-h);
                      
                    //cout<<"Matched, so remove. Divisor: "<<y<<endl;
                    
                    
               }  
          ++j;
          h=j;
          
          }
          
         if(changed)
          {
              if(u.value>0)
              {
                if(u.value!=1)
                    xaux+=u.unit+wxString::Format(_T("^%g*"),(double)u.value); 
                else
                    xaux+=u.unit+_T("*");
              } 
              if(u.value<0)
              {
                if(u.value!=-1)
                    yaux+=u.unit+wxString::Format(_T("^%g*"),(double)-u.value); 
                else
                    yaux+=u.unit+_T("*");    
              }
          }
          u.value=0;
      }
      
     
      xaux.RemoveLast();
      yaux.RemoveLast();
      yaux+=y;
       //cout <<"That's it"<<xaux <<" "<<yaux<<endl;
      
      x=xaux;
      y=yaux;
}

inline bool  wxSciUnitParser::is_number(char c)
{
    return ((c<58 && c>47)||c==44 ||c==46 ||c==45);
}
inline bool  wxSciUnitParser::is_letter(char c)
{
    return ((c<91 && c>64)||(c<123 && c>96));
}
inline bool  wxSciUnitParser::is_op(char c){
 return  (c==42||c==43 ||c==47 ||c==45 || c==38 || c=='#') ; 
}

bool wxSciUnitParser::Add (wxOBTNode* n,wxString& unit){
    wxConversion c1,c2;
    float aux;
    bool error=true;
            
            switch(n->Left->op){
             case opAdd: case opSub:
                    error=Add(n->Left,unit);
             break;  
             case opDiv:
                    error=Div(n->Left,unit);
             break;
             case opMul:
                    error=Mul(n->Left,unit);
             break;
             case opPow:
                    error=Pow(n->Left,unit);
             break;
             case opPow2:
                    error=Pow2(n->Left,unit);
             break;
             case noOp:
                  if( n->Left->nu.unit_e!=1)
                    n->Left->nu.unit= n->Left->nu.unit +_T("^")+wxString::Format(_T("%g"),n->Left->nu.unit_e);  
             break;
             default:
                    //cout<<"11"<<endl;
                    return false;   
            }
      
            switch(n->Right->op){
             case opAdd: case opSub:
                    error=Add(n->Right,unit);
             break;  
             case opDiv:
                    error=Div(n->Right,unit);
             break;
             case opMul:
                    error=Mul(n->Right,unit);
             break;
             case opPow:
                    error=Pow(n->Right,unit);
             break;
             case opPow2:
                    error=Pow2(n->Right,unit);
             break;
             case noOp:
                  if( n->Right->nu.unit_e!=1)
                    n->Right->nu.unit= n->Right->nu.unit +_T("^")+wxString::Format(_T("%g"),n->Right->nu.unit_e);  
             break;
             default:
                     //cout<<"12"<<endl;
                    return false;   
            }
            
        
        
      if(!(n->Left->nu.unit.IsEmpty() &&  n->Right->nu.unit.IsEmpty())){ 
       
            delete_point_zeros (n->Left->nu.unit);
            
            aux = n->Left->nu.value;
            if(!ComplexSearchConversions(unit,n->Left->nu.unit,&aux,&units)
            && n->Left->nu.value!=0){ 
                //cout<< "SUMA: no se encontraron conversiones para "<<n->Left->nu.unit<<endl;
                return false;
            }
            n->Left->nu.value= aux;
        
        
            delete_point_zeros (n->Right->nu.unit);
            
            aux = n->Right->nu.value;
            if(!ComplexSearchConversions(unit,n->Right->nu.unit,&aux,&units)
            && n->Right->nu.value!=0){ 
                 //cout<< "SUMA: no se encontraron conversiones para "<<n->Right->nu.unit<<endl;                      
                 return false;
            }
            n->Right->nu.value= aux;
             
       
        n->nu.unit=unit;
      }
      else{
         if( (n->Left->nu.unit.IsEmpty() &&  !n->Right->nu.unit.IsEmpty()) ||
         (!n->Left->nu.unit.IsEmpty() &&  n->Right->nu.unit.IsEmpty())){  
            // cout<< "SUMA: no se encontraron conversiones para "<<n->Right->nu.unit<<endl;  
            return false;   
          }
          else{
                
                
          }
        
        
        }
        
        n->nu.value=n->Left->nu.value+n->Right->nu.value;
        n->op=noOp;
        
        
        n->nu.unit_e=1;  
        
        if(n->father == (wxOBTNode*) 0xFFFFFFFF && n->nu.unit!=unit){
          
          // it's the first node, and we must have the unit
          // else it's an error
          
          
          return false;  
        }
        
        //cout << "Suma realizada: "<<n->nu.value<<" "<<n->nu.unit<<" true="<<true<< error<<" hay error: "<<!error<<endl;
    

    return error;
}

bool  wxSciUnitParser::Mul (wxOBTNode* n,wxString& unit){
    int pos1,pos2;
    bool error=true;
    
    
            switch(n->Left->op){
             case opAdd: case opSub:
                    error=Add(n->Left,unit);
             break;  
             case opDiv:
                    error=Div(n->Left,unit);
             break;
             case opMul:
                    error=Mul(n->Left,unit);
             break;
              case opPow:
                    error=Pow(n->Left,unit);
             break;
             case opPow2:
                    error=Pow2(n->Left,unit);
             break;
             case noOp:
                  if( n->Left->nu.unit_e!=1)
                    n->Left->nu.unit= n->Left->nu.unit +_T("^")+wxString::Format(_T("%g"),n->Left->nu.unit_e);  
             break;
             default:
                    //cout<<"*31"<<endl;
                    return false;   
            }
            
        
      
            switch(n->Right->op){
             case opAdd: case opSub:
                    error=Add(n->Right,unit);
             break;  
             case opDiv:
                    error=Div(n->Right,unit);
             break;
             case opMul:
                    error=Mul(n->Right,unit);
             break;
             case opPow:
                    error=Pow(n->Right,unit);
             break;
             case opPow2:
                    error=Pow2(n->Right,unit);
             break;
             case noOp:
                  if( n->Right->nu.unit_e!=1)
                    n->Right->nu.unit= n->Right->nu.unit +_T("^")+wxString::Format(_T("%g"),n->Right->nu.unit_e);  
             break;
             default:
                    //cout<<"*32"<<endl;
                    return false;   
            }
            
        

        n->nu.value=n->Left->nu.value*n->Right->nu.value;
        n->op=noOp;
 
        if(!n->Left->nu.unit.IsEmpty() && !n->Right->nu.unit.IsEmpty()){
                  //Multiply the units
                  pos1=n->Left->nu.unit.Find('/');
                  pos2=n->Right->nu.unit.Find('/');
                  if(pos1==0 || pos2==0){ 
                             return false;
                             
                  }
                  if(pos1>0 && pos2<0)
                  {
                   n->nu.unit=n->Left->nu.unit.Mid(0,pos1)+wxString::wxString(_T("*"))+n->Right->nu.unit+_T("/")+
                   n->Left->nu.unit.Mid(pos1+1);
                  }
                  if(pos1<0 && pos2>0)
                  {
                   n->nu.unit=n->Left->nu.unit+wxString::wxString(_T("*"))+n->Right->nu.unit.Mid(0,pos2)+_T("/")+
                   n->Right->nu.unit.Mid(pos2+1);
                  }
                  if(pos1>0 && pos2>0)
                  {
                   n->nu.unit=n->Left->nu.unit.Mid(0,pos1)+wxString::wxString(_T("*"))+n->Right->nu.unit.Mid(0,pos2)+_T("/")+
                   n->Left->nu.unit.Mid(pos1+1)+wxString::wxString(_T("*"))+n->Right->nu.unit.Mid(pos2+1);
                  }
                  if(pos1<0 && pos2<0)
                  {
                   n->nu.unit=n->Left->nu.unit+wxString::wxString(_T("*"))+n->Right->nu.unit;
                  }
                  //cout<<"Voy a simplificar"<<n->nu.unit<<endl;
                  //try to simplify
                  simplify_mul(n->nu.unit);
          }
          else
          {
              if(n->Left->nu.unit.IsEmpty())
                n->nu.unit=n->Right->nu.unit;
              else
                n->nu.unit=n->Left->nu.unit;
          }
        n->nu.unit_e=1; 
   
    
     //cout << "Multiplicacion realizada: "<<n->nu.value<<" "<<n->nu.unit<<endl;
    return error;
    
}


bool  wxSciUnitParser::Div (wxOBTNode* n,wxString& unit){
    int pos1,pos2;
    bool error=true;
    
    
            switch(n->Left->op){
             case opAdd: case opSub:
                    error=Add(n->Left,unit);
             break;  
             case opDiv:
                    error=Div(n->Left,unit);
             break;
             case opMul:
                    error=Mul(n->Left,unit);
             break;
             case opPow:
                    error=Pow(n->Left,unit);
             break;
             case opPow2:
                    error=Pow2(n->Left,unit);
             break;
             case noOp:
                  if( n->Left->nu.unit_e!=1)
                    n->Left->nu.unit= n->Left->nu.unit +_T("^")+wxString::Format(_T("%g"),n->Left->nu.unit_e);  
             break;
             default:
                    //cout<<"*11"<<endl;
                    return false;   
            }
            
            switch(n->Right->op){
             case opAdd: case opSub:
                    error=Add(n->Right,unit);
             break;  
             case opDiv:
                    error=Div(n->Right,unit);
             break;
             case opMul:
                    error=Mul(n->Right,unit);
             break;
             case opPow:
                    error=Pow(n->Right,unit);
             break;
             case opPow2:
                    error=Pow2(n->Right,unit);
             break;
             case noOp:
                  if( n->Right->nu.unit_e!=1)
                    n->Right->nu.unit= n->Right->nu.unit +_T("^")+wxString::Format(_T("%g"),n->Right->nu.unit_e);  
             break;
             default:
                     //cout<<"*12"<<endl;
                    return false;   
            }
            
        

        n->nu.value=n->Left->nu.value/n->Right->nu.value;
        n->op=noOp;
 
        if(!n->Left->nu.unit.IsEmpty() && !n->Right->nu.unit.IsEmpty()){
              //Multiply the units (cross)
              pos1=n->Left->nu.unit.Find('/');
              pos2=n->Right->nu.unit.Find('/');
              if(pos1==0 || pos2==0){ 
                         return false;
                         
              }          
              if(pos1>0 && pos2<0)
              {
               n->nu.unit=  n->Right->nu.unit+wxString::wxString(_T("*"))+n->Left->nu.unit.Mid(pos1+1)+_T("/")+
               n->Left->nu.unit.Mid(0,pos1);
              }
              if(pos1<0 && pos2>0)
              {
               n->nu.unit=n->Left->nu.unit+wxString::wxString(_T("*"))+n->Right->nu.unit.Mid(pos2+1)+_T("/")+
               n->Right->nu.unit.Mid(0,pos2);
              }
              if(pos1>0 && pos2>0)
              {
               n->nu.unit=n->Left->nu.unit.Mid(0,pos1)+wxString::wxString(_T("*"))+n->Right->nu.unit.Mid(pos2+1)+_T("/")+
               n->Left->nu.unit.Mid(pos1+1)+wxString::wxString(_T("*"))+n->Right->nu.unit.Mid(0,pos2);
               }
              if(pos1<0 && pos2<0)
              {
               n->nu.unit=n->Left->nu.unit+wxString::wxString(_T("/"))+n->Right->nu.unit;
              }
              //cout<<"Trying to simplify division"<<endl;
              //try to simplify
              simplify_mul(n->nu.unit);
      }
      else
      {
        if(!n->Left->nu.unit.IsEmpty() || !n->Right->nu.unit.IsEmpty()){
          if(n->Left->nu.unit.IsEmpty())
            n->nu.unit=_T("/")+n->Right->nu.unit;
          else
            n->nu.unit=n->Left->nu.unit;
        }
        else{
          n->nu.unit = _T(""); 
        }
      }

      n->nu.unit_e=1;
  
     
    
   
    return error;
    
}

bool wxSciUnitParser::Pow (wxOBTNode* n,wxString& unit){
    wxConversion c1,c2;
    bool error=true;
            
            switch(n->Left->op){
             case opAdd: case opSub:
                    error=Add(n->Left,unit);
             break;  
             case opDiv:
                    error=Div(n->Left,unit);
             break;
             case opMul:
                    error=Mul(n->Left,unit);
             break;
             case opPow:
                    error=Pow(n->Left,unit);
             break;
             case opPow2:
                    error=Pow2(n->Left,unit);
             break;
             case noOp:
                  if( n->Left->nu.unit_e!=1)
                    n->Left->nu.unit= n->Left->nu.unit +_T("^")+wxString::Format(_T("%g"),n->Left->nu.unit_e);  
             break;
             default:
                     //cout<<"51"<<endl;
                    return false;   
            }
      
            switch(n->Right->op){
             case opAdd: case opSub:
                    error=Add(n->Right,unit);
             break;  
             case opDiv:
                    error=Div(n->Right,unit);
             break;
             case opMul:
                    error=Mul(n->Right,unit);
             break;
             case opPow:
                    error=Pow(n->Right,unit);
             break;
             case opPow2:
                    error=Pow2(n->Right,unit);
             break;
             case noOp:
                  // An exponent can't have units 
                  if( !(n->Right->nu.unit.IsEmpty()))
                    return false;  
             break;
             default:
                     //cout<<"52"<<endl;
                    return false;   
            }
            
        
        
        
         
        n->nu.value=pow((float)n->Left->nu.value,(float)n->Right->nu.value);
        n->op=noOp;
        
        n->nu.unit=unit;
        //change the unit propperly
        if(!solve_pow (n->nu.unit,n->Right->nu.value,n->nu.unit)){
            //cout<<"Error Solving Power"<<endl;
            return false;
        }
        
        n->nu.unit_e=1;  
   
        //cout << "Potencia realizada: "<<n->nu.value<<" "<<n->nu.unit<<endl;
    return error;
}

bool wxSciUnitParser::Pow2 (wxOBTNode* n,wxString& unit){
    wxConversion c1,c2;
    bool error=true;
            
            switch(n->Left->op){
             case opAdd: case opSub:
                    error=Add(n->Left,unit);
             break;  
             case opDiv:
                    error=Div(n->Left,unit);
             break;
             case opMul:
                    error=Mul(n->Left,unit);
             break;
             case opPow:
                    error=Pow(n->Left,unit);
             break;
             case opPow2:
                    error=Pow2(n->Right,unit);
             break;
             case noOp:
                  if( n->Left->nu.unit_e!=1)
                    n->Left->nu.unit= n->Left->nu.unit +_T("^")+wxString::Format(_T("%g"),n->Left->nu.unit_e);  
             break;
             default:
                     //cout<<"61"<<endl;
                    return false;   
            }
      
            switch(n->Right->op){
             case opAdd: case opSub:
                    error=Add(n->Right,unit);
             break;  
             case opDiv:
                    error=Div(n->Right,unit);
             break;
             case opMul:
                    error=Mul(n->Right,unit);
             break;
             case opPow:
                    error=Pow(n->Right,unit);
             break;
             case opPow2:
                    error=Pow2(n->Right,unit);
             break;
             case noOp:
                  // An exponent can't have units 
                  if( !(n->Right->nu.unit.IsEmpty()))
                    return false;  
             break;
             default:
                     //cout<<"62"<<endl;
                    return false;   
            }
            
        
        
        
         
        n->nu.value=n->Left->nu.value;//pow((float)n->Left->nu.value,(float)n->Right->nu.value);
        n->op=noOp;
        
        n->nu.unit=unit;
        //change the unit propperly
        if(!solve_pow (n->nu.unit,n->Right->nu.value,n->nu.unit)){
            //cout<<"Error Solving Power 2"<<endl;
            return false;
        }
        
        n->nu.unit_e=1;  
   
        //cout << "Potencia realizada 2: "<<n->nu.value<<" "<<n->nu.unit<<endl;
    return error;
}


bool wxSciUnitParser::solve_pow (wxString x, double y, wxString& res)
{
     unsigned int i;
     bool error;
     
      for(i=0; i<x.Len();++i){
        if( is_op(x.GetChar(i))){
            x=  x.Mid(0,i)+ wxString::Format(_T("^%g"),y) + x.Mid(i);     
        }   
      }
       x=x+ wxString::Format(_T("^%g"),y);
     
      // Rejoin the exponents
      error = shrink_exponents(x,0);
      
      res=x;
 
     return error;           
}


void  wxSciUnitParser::delete_point_zeros (wxString& s){
  /*int i;   
    
  for(i=0;i<(int)s.Len()-2;++i)
  {
     if(s.GetChar(i)=='.')
     {
      if(s.GetChar(i+1)=='0')
      {
       if(s.GetChar(i+2)=='0')
       {
         s=s.Remove(i,3);
       }
      }
     }
 }  */
    
}

// NORMA DE CONVERSIONES
// Si se ha de cambiar de un factor multiplicativo a uno aditivo es 
// necesario definir ambos, no se puede insertar una pareja de tipo 
// aditivo/multiplicativo
/*bool wxSciUnitParser::pruebaconv(wxString unit1,wxString unit2,float& val){
    wxStringArray::Node *node;
    cout<<endl<<"Starting search between "<<unit1<<" and "<<unit2<<endl;
    node = units.GetFirst();
    cout <<"[Lista unidades:]"<<endl;
     while(node){
            cout<<*(node->GetData())<<" ";
            node= node->GetNext();   
     }
     cout<<endl;
     cout <<"[Tabla Conversiones:]"<<endl;
     print_conversions();
     cout <<"Entrando en SearchConversion"<<endl;
     
 return    SearchConversion(unit1,unit2,val,&units);
}
*/
bool wxSciUnitParser::find_conversion(wxString x, wxString y,wxConversion& c1,wxConversion& c2)
{
     wxConversionArrayList::Node *node1 = conversions.GetFirst();
     wxConversionArray::Node *node2 ; 
     bool foundc1,foundc2;
     foundc1=foundc2=false;
     
     //only find a suitable conversion in the list
     while (node1 && !(foundc1 && foundc2))
      {
            node2=node1->GetData()->GetFirst();
                    
                    foundc1=foundc2=false;
                    while(node2 && !(foundc1 && foundc2))
                    {
                      if(node2->GetData()->GetUnit().IsSameAs( x))
                      {
                       c1=*(node2->GetData());
                       foundc1=true;
                       
                      }
                      
                      if(node2->GetData()->GetUnit().IsSameAs(y))
                      {
                       foundc2=true;
                       c2=*(node2->GetData());
                      
                      }
                     
                      node2 = node2->GetNext();          
                    }    
          node1 = node1->GetNext();
      }
      
      return foundc1 && foundc2;
}


bool wxSciUnitParser::SearchConversion(wxString unit1,wxString unit2,float* val,wxStringArray* unitlist){
  wxConversion c1,c2;
  wxStringArray unitl;
  wxStringArray::Node *node;
  wxString tosearch;
  float antval;
   
   
  //copy the list (we will work with an auxiliar one) 
  AppendToStringArray(unitlist,&unitl);
  
  //first delete the current unit
  
    if(FindInStringArray(unit1,&unitl)){
        if(!DeleteInStringArray(unit1,&unitl)){
            return false;   
        }
        node = unitl.GetFirst();
    }
    else{
        return false;    
    }
  
  if(find_conversion(unit1,unit2,c1,c2)){
        //cout<<"Conversion found between "<<unit1<<" and "<<unit2<<endl;
        if(c2.GetOp()==cOpAdd){
               *val+=c2.GetFactor()-c1.GetFactor();
        }
        else{
               *val*=(c2.GetFactor()/c1.GetFactor());
        }
        return true;
  }
  else{
        //cout<<"No conversion found between "<<unit1<<" and "<<unit2<<endl;
        while(node){
            antval= *val;
            tosearch=*(node->GetData());
          //  cout<<"Trying conversion between "<<unit1<<" and "<<tosearch<<endl;
            if(find_conversion(unit1,tosearch,c1,c2)){
                //cout<<"OK, conversion found between "<<unit1<<" and "<<tosearch<<endl;
                    if(c2.GetOp()==cOpAdd){
                         *val+=c2.GetFactor()-c1.GetFactor();
                    }
                    else{
                           *val*=(c2.GetFactor()/c1.GetFactor());
                    }
                if(SearchConversion(tosearch,unit2,val,&unitl)){
                    return true;
                }
                else{
                   *val= antval; 
                }
            }
            
            node = node->GetNext(); 
        }
        
        return false;
  }  
    
}

// El progrmador se compromete a que si hay una conversion a/x = b buscando la 
// unidad b, exista alguna conversión que contenga b

wxConversionArray* wxSciUnitParser::genconversions (wxString unit,wxStringArray* unit_list){
 
    wxConversionArray* conv = new wxConversionArray();
    wxStringArray::Node *snode;
    wxConversion c1,c2;
    float val;
    //a unit can be changed to itself :)
    conv->DeleteContents(true);
    conv->Append(new wxConversion(unit,cOpMult,1));
    
     //PrintStringArray(unit_list);
    snode = unit_list->GetFirst();
    while (snode){
        //cout<<"Searching conv between"<<unit<<" and "<<*(snode->GetData())<<endl;
        val=1;
        if(SearchConversion(unit,*(snode->GetData()),&val,unit_list)){
            //We only expect mul conversions in a unit
            conv->Append(new wxConversion(*(snode->GetData()),cOpMult,val));        
        }  
        
        snode=snode->GetNext();
    }
    
    return conv;
}


void wxSciUnitParser::perm(wxStringArray::Node* n,wxString newunit,float factor,wxStringArray* sa){
    wxConversionArray* conv;
    wxConversionArray::Node* cnode;
    wxOBTNode root,left,right;
    number n1,n2,res;
    if(n!= NULL){
        conv =   genconversions (*(n->GetData()),&units); 
       
        cnode = conv->GetFirst();
        while(cnode){ 
            // create the mini tree
                n1.unit = newunit;
                n1.value = factor;
                n1.unit_e = 1;
                
                n2.unit = cnode->GetData()->GetUnit();
                n2.value = cnode->GetData()->GetFactor();
                n2.unit_e = 1;
                
                left.op = noOp;
                left.nu = n1;
                left.Right = NULL;
                left.Left = NULL;
                left.father = &root;
                
                right.op = noOp;
                right.nu = n2;
                right.Right = NULL;
                right.Left = NULL;
                right.father = &root;
                
                root.op = opMul;
                root.nu = res;
                root.Right = &right;
                root.Left = &left;
                root.father = NULL;
                
                Mul(&root,newunit); // da absolutamente igual la unidad que se le ponga
                
           
                perm(n->GetNext(),root.nu.unit,root.nu.value,sa);
            //
            
            
            //perm(n->GetNext(),newunit+(cnode->GetData()->GetUnit())+"*",factor/cnode->GetData()->GetFactor(),sa);
            cnode = cnode->GetNext();
        }
        delete conv;
        
    } 
    else{
        //newunit = newunit.RemoveLast();
        //sa->Append(new wxString(wxString::Format("%.3f",factor)));
        
        if(!FindInStringArray(newunit,sa)){
            sa->Append(new wxString(wxString::Format(_T("%.3f"),factor)));
            sa->Append(new wxString(newunit));
        }
    } 
}

void wxSciUnitParser::Extensions (wxString unit,wxString s){
    wxString dividend,divisor,newunit;
    wxStringArray dividend_unit_list,divisor_unit_list,
                  dividend_permutations,divisor_permutations;
    wxStringArray::Node *node1,*node2 ;
    double factor1,factor2;
    
    wxOBTNode root,left,right;
    number n1,n2,res;
    wxConversion c1,c2;
    
    
    int pos,i,j;
    
    //cout<< "Extendiendo "<<s<<" a "<<unit<<endl;
    pos=s.Find('/');
    
    dividend= s.Mid(0,pos);
    divisor= s.Mid(pos+1);
       
    if(dividend.CompareTo(divisor) == 0) divisor =_T("");
    
    dividend_unit_list.DeleteContents(true);
    divisor_unit_list.DeleteContents(true);
   
    
    //fulfill the lists
     j=0;
     for(i=0;i<=(int)dividend.Len();++i)
     {
        if(i == (int)dividend.Len()){
           
            dividend_unit_list.Append(new wxString(dividend.Mid(j)));
        }
        else{
            if(is_op(dividend.GetChar(i)))
            {
              
                dividend_unit_list.Append(new wxString(dividend.Mid(j,i-j)));
                j=i+1;
            }
        }
     }
    
     j=0;
     for(i=0;i<=(int)divisor.Len();++i)
     {
        if(i == (int)divisor.Len()){
            divisor_unit_list.Append(new wxString(divisor.Mid(j)));
        }
        else{
            if(is_op(divisor.GetChar(i)))
            {
                
                divisor_unit_list.Append(new wxString(divisor.Mid(j,i-j)));
                j=i+1;
            }
        }
     }
     
   
    
   //cout<<"Unidades del dividendo:"<<endl;
     //PrintStringArray(&dividend_unit_list);
   //cout<<"Unidades del divisor:"<<endl;  
     //PrintStringArray(&divisor_unit_list);
   //cout<<endl;
   
    node1 = dividend_unit_list.GetFirst();
    node2 = divisor_unit_list.GetFirst();
    
    perm(node1,wxString(_T("")),(float)1,&dividend_permutations);
    if(divisor!=_T(""))
    perm(node2,wxString(_T("")),(float)1,&divisor_permutations);
    
     //cout<<"Permutaciones del dividendo:"<<endl;
    //PrintStringArray(&dividend_permutations);
      //cout<<"Permutaciones del divisor:"<<endl;
    //PrintStringArray(&divisor_permutations);
    
    //cout<<endl;
    
    node1=dividend_permutations.GetFirst();
    while(node1){
        //factor
        if(!node1->GetData()->ToDouble(&factor1))
           withcomma(*(node1->GetData())).ToDouble(&factor1);

        node1=node1->GetNext();
        //unidad
         
        if(divisor!=_T("")){
            node2=divisor_permutations.GetFirst();
            while(node2){
                //SearchConversion(*(node1->GetData())+"/"+*(node2->GetData())
                if(!node2->GetData()->ToDouble(&factor2))
                    withcomma(*(node2->GetData())).ToDouble(&factor2);
                node2=node2->GetNext();
                
                // create the mini tree
                n1.unit = *(node1->GetData());
                n1.value = factor1;
                n1.unit_e = 1;
                
                n2.unit = *(node2->GetData());
                n2.value = factor2;
                n2.unit_e = 1;
                
                left.op = noOp;
                left.nu = n1;
                left.Right = NULL;
                left.Left = NULL;
                left.father = &root;
                
                right.op = noOp;
                right.nu = n2;
                right.Right = NULL;
                right.Left = NULL;
                right.father = &root;
                
                root.op = opDiv;
                root.nu = res;
                root.Right = &right;
                root.Left = &left;
                root.father = NULL;
                
                Div(&root,divisor); // da absolutamente igual la unidad que se le ponga
                
                delete_point_zeros(root.nu.unit);
                
                //cout <<root.nu.unit<<" factor is "<<root.nu.value<<endl;
                 
                if(unit.CompareTo(root.nu.unit)==0){
                    c1.SetOp(cOpMult);
                    c1.SetFactor(1);
                    c1.SetUnit(s);
                    
                    c2.SetOp(cOpMult);
                    c2.SetFactor(root.nu.value);
                    c2.SetUnit(unit);
                    
                    //cout << "Introduciendo: "<<c1<<" " << c2<<endl;
                    InsertConversion(c2,c1);
                    print_conversions();
                    return;
                }
                /////////////////////////////////////////////////////             
                node2=node2->GetNext();   
            }    
        }
        else{
           if(unit.CompareTo(*(node1->GetData()))==0){
                    c1.SetOp(cOpMult);
                    c1.SetFactor(1);
                    c1.SetUnit(s);
                    
                    c2.SetOp(cOpMult);
                    c2.SetFactor(factor1);
                    c2.SetUnit(unit);
                    
                    //cout << "introduciendo: "<<c1<<" " << c2<<endl;
                    InsertConversion(c2,c1);
                    print_conversions();
                    return;
                }  
        }
     node1=node1->GetNext();    
    }
} 

bool wxSciUnitParser::ComplexSearchConversions(wxString unit1,wxString unit2,float* val,wxStringArray *unitl){
    float aux;
    
    if(unit1.CompareTo(unit2)==0) return true;
    
    aux = *val;
    if(!SearchConversion(unit2,unit1,&aux,unitl)){
        Extensions(unit1,unit2);
        return SearchConversion(unit2,unit1,val,unitl);
    }
    else{
        *val = aux;
        return true;
    }
}



void   wxSciUnitParser::InsertExpansion (wxConversion& unit,const wxString* expset,int numberofchanges, double factor, double inifactor){
    int i;
    wxConversion newConv;
    
    
    for(i=0;i<numberofchanges;i++){
        if(expset[i]!=_T("")){
            newConv.SetOp(cOpMult);
            newConv.SetFactor(1/inifactor);
            newConv.SetUnit(expset[i]+unit.GetUnit());
            InsertConversion(unit,newConv);
        }
        inifactor= inifactor/factor;
    }   
  
}

  
bool wxSciUnitParser::ReadConversionsFromFile (const char* path){
    fstream filestr;
    wxConversion c1,c2;
    
    
    
    filestr.open (path, fstream::in);
    if (filestr.is_open())
    {
      
       while(filestr>>c1>>c2){
          InsertConversion(c1,c2);
       }
    
       filestr.close();
       return true;
    }
    else{
     filestr.close();
     return false;   
    }
}

// ****CORRECTED BUGS*****
//# (3m^3*ca)/(m^2*ca) -->right 3m^3*ca/m^2*ca -->mal// OK!!
//# (1*2)+(3*4)*m  // It should give an error // USING OF BRACKETS ERROR (3*4)*m and not (3*4)m
//# (4m^(1+1) )/ (m) //Create pow2 // OK!!!
//# 2m+(3m^5*ca^5)/(m^4*ca^5) --> totally wrong // OK!!
//# ((2*2 +4)/2)*m OK!!
//# 2ml // OK!!
//#(3m^3*ca)/(m^2*m*ca) -->the operation result is correct but it must
//throw an error  3m^3*ca/(m^2*m*ca) //OK!! 
//# 3*(3m^3)/m^2 // There was a problem deleting () and (number) cases// OK!!

