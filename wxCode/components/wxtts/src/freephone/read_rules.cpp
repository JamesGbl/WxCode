/////////////////////////////////////////////////////////////////////////////
// Name:        read_rules.cpp
// Purpose:     ??
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: read_rules.cpp,v 1.1.1.1 2003-12-28 21:08:19 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "regexp.h"
#include "t2s.h"

int tsFreePhoneImplementation::read_rule_file(char *filename, RULE *rules)
{
        int reading_rules = 0;
	FILE *xfd;
	char *cptr, *tptr;
	char **bits;
	char *key[50], *cvalue[50];  /* BAD BAD BAD arbitrary  */
	char s[100];
	int j=0;
	int k=0;
	int i;
	int num_rules;
	int num_classes;

        if((xfd=fopen(filename,"r")) == NULL) {
		(void)fprintf(stderr,"Can't open file: %s\n",filename);
		exit(1);
	}
	while(fgets(s,100,xfd) != NULL) {
	       if(s[0] == ';') { /* comment character now ;  */
		       continue;
	       }
               tptr = s;
	       while(*tptr==' ' || *tptr=='\t' || *tptr=='\n') {
		       tptr++;
	       }
	       if(strlen(tptr) == 0) {
		       continue;
	       }
	       cptr = tptr;
	       if((tptr = strchr(s,';'))!=NULL) {
	               *tptr = '\0';
	       }
	       if(s[strlen(s)-1]=='\n') {
		       s[strlen(s)-1] = '\0';
	       }

               /* now have something, a statement or a rule  */

               if(!strncmp(cptr,"RULE",4)) {
		       if(reading_rules==0) {
			       reading_rules = 1;
         		       continue;
		       } else {
                               fprintf(stderr,"RULES appears too often: %s\n",filename);       
                               exit(1);
                       }
	       } else if (!strncmp(cptr,"CLASS",5)) {       
		       if(reading_rules==0) {
                               bits = split(cptr);
			       key[k] = strdup(bits[1]);
			       cvalue[k] = strdup(cptr);
			       cvalue[k][0] = '\0';
			       i=2;
			       while(bits[i]!=NULL) {
				     if(i!=2)
				             strcat(cvalue[k]," ");
				     strcat(cvalue[k],bits[i]);
				     i++;
			       }
			       k++;
			       tidy_split(bits);
         		       continue;
	               } else {
                               fprintf(stderr,"RULES appears too often: %s\n",filename);       
                               exit(1);
                       }

	       } else if(reading_rules==1) {
		       process_rule(cptr,&rules[j]);
		       j++;
	       }		       
	       
        }
	num_rules = j;
	num_classes = k;
        replace_classes(num_rules, rules, num_classes, key, cvalue);

	return(num_rules);
}


void tsFreePhoneImplementation::process_rule(char *input,RULE *rule)
{
        int target_flag = 0;
	int output_flag = 0;
	int i,ir;
	char *left_context;
	char *target;
	char *right_context;
	char *output;
	char **bits;
	char *tmp;

        bits = split(input);
	left_context = strdup(input);
	left_context[0] = '\0';
	target = strdup(input);
	target[0] = '\0';
	right_context = strdup(input);
	right_context[0] = '\0';
	output = strdup(input);
	output[0] = '\0';
	
	i=0;
	while((bits[i]!=NULL)&&(strcmp(bits[i],"[["))) {
	        if(i!=0)
		        strcat(left_context," ");
	        strcat(left_context,bits[i]);
		i++;
	}
	strcat(left_context,"$");
	/* extra bit for not compacting  */
	if((left_context[0]=='\'') && (left_context[strlen(left_context)-2]=='\'')) {
	        tmp = strdup(&left_context[1]);
		free(left_context);
		left_context = tmp;
		left_context[strlen(left_context)-2] = '$';
		left_context[strlen(left_context)-1] = '\0';
	}
	if(bits[i]!=NULL)
        	i++;  /* to skip over "[["  */

	ir = i;
	while((bits[i]!=NULL)&&(strcmp(bits[i],"]]"))) {
	        target_flag = 1;
	        if(i!=ir)
		        strcat(target," ");
	        strcat(target,bits[i]);
		i++;
	}
	/* extra bit for not compacting  */
	if((target[0]=='\'') && (target[strlen(target)-1]=='\'')) {
	        tmp = strdup(&target[1]);
		free(target);
		target = tmp;
		target[strlen(target)-1] = '\0';
	}
	if(bits[i]!=NULL)
        	i++;  /* to skip over "]]"  */

	strcat(right_context,"^");
	ir = i;
	while((bits[i]!=NULL)&&(strcmp(bits[i],"->"))) {
	        if(i!=ir)
		        strcat(right_context," ");
	        strcat(right_context,bits[i]);
		i++;
	}
	/* extra bit for not compacting  */
	if((right_context[1]=='\'') && (right_context[strlen(right_context)-1]=='\'')) {
	        tmp = strdup(&right_context[1]);
		free(right_context);
		right_context = tmp;
		right_context[0] = '^';
		right_context[strlen(right_context)-1] = '\0';
	}
	if(bits[i]!=NULL)
        	i++;  /* to skip over "->"  */

	ir = i;
	while(bits[i]!=NULL) {
	        output_flag = 1;
	        if(i!=ir)
		        strcat(output," ");
	        strcat(output,bits[i]);
		i++;
        }
	/* extra bit for not compacting  */
	if((output[0]=='\'') && (output[strlen(output)-1]=='\'')) {
	        tmp = strdup(&output[1]);
		free(output);
		output = tmp;
		output[strlen(output)-1] = '\0';
	}
	if(!(target_flag && output_flag)) {
                fprintf(stderr,"Malformed RULE: %s\n",input);
		exit(1);
        }

	rule->left_context = strdup(left_context);
	rule->target = strdup(target);
	rule->right_context = strdup(right_context);
	rule->output = strdup(output);

	/* now free up all that malloced space  */
	free(left_context);
	free(target);
	free(right_context);
	free(output);

	tidy_split(bits);

}

void tsFreePhoneImplementation::replace_classes(int num_rules, RULE *rules, int num_classes, char **key, char **value)
{
       int i;

       for(i=0;i<num_rules;i++) {
	       rules[i].left_context = apply_class(num_classes,rules[i].left_context,key,value);
	       rules[i].target = apply_class(num_classes,rules[i].target,key,value);
      	       rules[i].right_context = apply_class(num_classes,rules[i].right_context,key,value);
       }
}

char *tsFreePhoneImplementation::apply_class(int num_classes, char *string, char **key, char **value)
{
       char *buffer;
       char *output;
       int i;

       buffer = (char *)malloc(sizeof(char)*8000);

       strcpy(buffer,string);
       free(string);

       /* now run the classes in buffer  */
       for(i=0;i<num_classes;i++) {
	       use_class(buffer,key[i],value[i]);
       }

       output = strdup(buffer);
       free(buffer);
       return(output);
}

void tsFreePhoneImplementation::use_class(char *buffer, char *key, char *value)
{
       char trigger;
       char *ptr;

       trigger = key[0];
       while((ptr=strchr(buffer,trigger))!=NULL) {
	       if(!strncmp(ptr,key,strlen(key))) {
		       replace_class(buffer,ptr,key,value);
	       }
       }
}

void tsFreePhoneImplementation::replace_class(char *buffer, char *ptr, char *key, char *value)
{
        char *tmp;
	
	tmp = strdup(ptr);

	if((strlen(buffer)+strlen(value)) > 8000) {
	        fprintf(stderr,"BIG regular expression\n");
	}
        strcpy(tmp,ptr+strlen(key));
	strcat(ptr,value);
	strcat(ptr+strlen(value),tmp);

	free(tmp);
}
