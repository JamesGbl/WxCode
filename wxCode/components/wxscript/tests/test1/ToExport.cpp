/*
** Lua binding: toexport
** Generated automatically by tolua 5.0a on 06/09/04 11:42:20.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua.h"

/* Exported function */
TOLUA_API int tolua_toexport_open (lua_State* tolua_S);

#include "Test.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_myClass (lua_State* tolua_S)
{
 myClass* self = (myClass*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"myClass");
}

/* method: new of class  myClass */
static int tolua_toexport_myClass_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"myClass",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  myClass* tolua_ret = (myClass*)  new myClass();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"myClass");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: delete of class  myClass */
static int tolua_toexport_myClass_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"myClass",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  myClass* self = (myClass*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
 delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}

/* method: set of class  myClass */
static int tolua_toexport_myClass_set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"myClass",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  myClass* self = (myClass*)  tolua_tousertype(tolua_S,1,0);
  bool value = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'",NULL);
#endif
 {
  self->set(value);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set'.",&tolua_err);
 return 0;
#endif
}

/* Open function */
TOLUA_API int tolua_toexport_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"myClass","myClass","",tolua_collect_myClass);
#else
 tolua_cclass(tolua_S,"myClass","myClass","",NULL);
#endif
 tolua_beginmodule(tolua_S,"myClass");
 tolua_function(tolua_S,"new",tolua_toexport_myClass_new00);
 tolua_function(tolua_S,"delete",tolua_toexport_myClass_delete00);
 tolua_function(tolua_S,"set",tolua_toexport_myClass_set00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}
