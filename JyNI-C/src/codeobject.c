/* This File is based on codeobject.c from CPython 2.7.6 release.
 * It has been modified to suit JyNI needs.
 *
 * Copyright of the original file:
 * Copyright (c) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
 * 2011, 2012, 2013, 2014 Python Software Foundation.  All rights reserved.
 *
 * Copyright of JyNI:
 * Copyright (c) 2013, 2014 Stefan Richthofer.  All rights reserved.
 *
 *
 * This file is part of JyNI.
 *
 * JyNI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JyNI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with JyNI.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Linking this library statically or dynamically with other modules is
 * making a combined work based on this library.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this library with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module.  An independent module is a module which is not derived from
 * or based on this library.  If you modify this library, you may extend
 * this exception to your version of the library, but you are not
 * obligated to do so.  If you do not wish to do so, delete this
 * exception statement from your version.
 */


#include "JyNI.h"
#include "code_JyNI.h"
#include "structmember_JyNI.h"

//#define NAME_CHARS \
//	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz"
//
///* all_name_chars(s): true iff all chars in s are valid NAME_CHARS */
//
//static int
//all_name_chars(unsigned char *s)
//{
//	static char ok_name_char[256];
//	static unsigned char *name_chars = (unsigned char *)NAME_CHARS;
//
//	if (ok_name_char[*name_chars] == 0) {
//		unsigned char *p;
//		for (p = name_chars; *p; p++)
//			ok_name_char[*p] = 1;
//	}
//	while (*s) {
//		if (ok_name_char[*s++] == 0)
//			return 0;
//	}
//	return 1;
//}

//static void
//intern_strings(PyObject *tuple)
//{
//	Py_ssize_t i;
//
//	for (i = PyTuple_GET_SIZE(tuple); --i >= 0; ) {
//		PyObject *v = PyTuple_GET_ITEM(tuple, i);
//		if (v == NULL || !PyString_CheckExact(v)) {
//			Py_FatalError("non-string found in code slot");
//		}
//		PyString_InternInPlace(&PyTuple_GET_ITEM(tuple, i));
//	}
//}


PyCodeObject *
PyCode_New(int argcount, int nlocals, int stacksize, int flags,
		   PyObject *code, PyObject *consts, PyObject *names,
		   PyObject *varnames, PyObject *freevars, PyObject *cellvars,
		   PyObject *filename, PyObject *name, int firstlineno,
		   PyObject *lnotab)
{

//	Types:
//	PyObject *code			something supporting buffer
//	PyObject *consts		Tuple of PyObject
//	PyObject *names			Tuple of PyString
//	PyObject *varnames		Tuple of PyString
//	PyObject *freevars		Tuple of PyString
//	PyObject *cellvars		Tuple of PyString
//	PyObject *filename		PyString
//	PyObject *name			PyString
//	PyObject *lnotab		PyString

//	PyCodeObject *co;
	/* Check argument types */
	if (argcount < 0 || nlocals < 0 ||
		code == NULL ||
		consts == NULL || !PyTuple_Check(consts) ||
		names == NULL || !PyTuple_Check(names) ||
		varnames == NULL || !PyTuple_Check(varnames) ||
		freevars == NULL || !PyTuple_Check(freevars) ||
		cellvars == NULL || !PyTuple_Check(cellvars) ||
		name == NULL || !PyString_Check(name) ||
		filename == NULL || !PyString_Check(filename) ||
		lnotab == NULL || !PyString_Check(lnotab) ||
		!PyObject_CheckReadBuffer(code)) {
		PyErr_BadInternalCall();
		return NULL;
	}

	env(NULL);
	//Try to get code from a buffer into a jstring...
	char* code_cstr;
	Py_ssize_t i;
	jstring code_jstr;
	if (PyObject_AsCharBuffer(code, &code_cstr, &i) == 0)
	{
		char code_cstr0[i+1]; //code_cstr might not be null-terminated
		strcpy(code_cstr0, code_cstr);
		code_jstr = (*env)->NewStringUTF(env, code_cstr0);
	} else
	{
		PyErr_BadInternalCall();
		return NULL;
	}

//	jobject jConsts = NULL;
//	if (PyTuple_GET_SIZE(consts))
//	{
//		(*env)->NewObjectArray(env, PyTuple_GET_SIZE(consts), pyObjectClass, NULL);
//		for (i = 0; i < PyTuple_GET_SIZE(consts); ++i)
//			(*env)->SetObjectArrayElement(env, jConsts, i,
//				JyNI_JythonPyObject_FromPyObject(PyTuple_GET_ITEM(consts, i)));
//	}
	pyTuple2jArray(consts, pyObjectClass, jConsts);

//	jobject jNames = (*env)->NewObjectArray(env, PyTuple_GET_SIZE(names), pyStringClass, NULL);
//	for (i = 0; i < PyTuple_GET_SIZE(names); ++i)
//		(*env)->SetObjectArrayElement(env, jNames, i,
//			JyNI_JythonPyObject_FromPyObject(PyTuple_GET_ITEM(names, i)));
	pyTuple2jArray(names, pyStringClass, jNames);

//	jobject jVarnames = (*env)->NewObjectArray(env, PyTuple_GET_SIZE(varnames), pyStringClass, NULL);
//	for (i = 0; i < PyTuple_GET_SIZE(varnames); ++i)
//		(*env)->SetObjectArrayElement(env, jVarnames, i,
//			JyNI_JythonPyObject_FromPyObject(PyTuple_GET_ITEM(varnames, i)));
	pyTuple2jArray(varnames, pyStringClass, jVarnames);

//	jobject jFreevars = (*env)->NewObjectArray(env, PyTuple_GET_SIZE(freevars), pyStringClass, NULL);
//	for (i = 0; i < PyTuple_GET_SIZE(freevars); ++i)
//		(*env)->SetObjectArrayElement(env, jFreevars, i,
//			JyNI_JythonPyObject_FromPyObject(PyTuple_GET_ITEM(freevars, i)));
	pyTuple2jArray(freevars, pyStringClass, jFreevars);

//	jobject jCellvars = (*env)->NewObjectArray(env, PyTuple_GET_SIZE(cellvars), pyStringClass, NULL);
//	for (i = 0; i < PyTuple_GET_SIZE(cellvars); ++i)
//		(*env)->SetObjectArrayElement(env, jCellvars, i,
//			JyNI_JythonPyObject_FromPyObject(PyTuple_GET_ITEM(cellvars, i)));
	pyTuple2jArray(cellvars, pyStringClass, jCellvars);

	jobject jFilename = JyNI_JythonPyObject_FromPyObject(filename);
	jobject jName = JyNI_JythonPyObject_FromPyObject(name);
	jobject jLnotab = JyNI_JythonPyObject_FromPyObject(lnotab);

	jobject result = (*env)->NewObject(env, pyBytecodeClass, pyBytecodeConstructor,
		argcount, nlocals, stacksize, flags, code_jstr, jConsts, jNames, jVarnames,
		jFilename, jName, firstlineno, jLnotab, jCellvars, jFreevars);

	return (PyCodeObject*) JyNI_PyObject_FromJythonPyObject(result);

//	intern_strings(names);
//	intern_strings(varnames);
//	intern_strings(freevars);
//	intern_strings(cellvars);
	/* Intern selected string constants */
//	for (i = PyTuple_Size(consts); --i >= 0; ) {
//		PyObject *v = PyTuple_GetItem(consts, i);
//		if (!PyString_Check(v))
//			continue;
//		if (!all_name_chars((unsigned char *)PyString_AS_STRING(v)))
//			continue;
//		PyString_InternInPlace(&PyTuple_GET_ITEM(consts, i));
//	}
//	co = PyObject_NEW(PyCodeObject, &PyCode_Type);
//	if (co != NULL) {
//		co->co_argcount = argcount;
//		co->co_nlocals = nlocals;
//		co->co_stacksize = stacksize;
//		co->co_flags = flags;
//		Py_INCREF(code);
//		co->co_code = code;
//		Py_INCREF(consts);
//		co->co_consts = consts;
//		Py_INCREF(names);
//		co->co_names = names;
//		Py_INCREF(varnames);
//		co->co_varnames = varnames;
//		Py_INCREF(freevars);
//		co->co_freevars = freevars;
//		Py_INCREF(cellvars);
//		co->co_cellvars = cellvars;
//		Py_INCREF(filename);
//		co->co_filename = filename;
//		Py_INCREF(name);
//		co->co_name = name;
//		co->co_firstlineno = firstlineno;
//		Py_INCREF(lnotab);
//		co->co_lnotab = lnotab;
//		co->co_zombieframe = NULL;
//		co->co_weakreflist = NULL;
//	}
//	return co;
}

PyCodeObject *
PyCode_NewEmpty(const char *filename, const char *funcname, int firstlineno)
{
	static PyObject *emptystring = NULL;
	static PyObject *nulltuple = NULL;
	PyObject *filename_ob = NULL;
	PyObject *funcname_ob = NULL;
	PyCodeObject *result = NULL;
	if (emptystring == NULL) {
		emptystring = PyString_FromString("");
		if (emptystring == NULL)
			goto failed;
	}
	if (nulltuple == NULL) {
		nulltuple = PyTuple_New(0);
		if (nulltuple == NULL)
			goto failed;
	}
	funcname_ob = PyString_FromString(funcname);
	if (funcname_ob == NULL)
		goto failed;
	filename_ob = PyString_FromString(filename);
	if (filename_ob == NULL)
		goto failed;

	result = PyCode_New(0,					  /* argcount */
				0,							  /* nlocals */
				0,							  /* stacksize */
				0,							  /* flags */
				emptystring,					/* code */
				nulltuple,					  /* consts */
				nulltuple,					  /* names */
				nulltuple,					  /* varnames */
				nulltuple,					  /* freevars */
				nulltuple,					  /* cellvars */
				filename_ob,					/* filename */
				funcname_ob,					/* name */
				firstlineno,					/* firstlineno */
				emptystring					 /* lnotab */
				);

failed:
	Py_XDECREF(funcname_ob);
	Py_XDECREF(filename_ob);
	return result;
}


PyObject* PyCode_Get_co_argcount(PyObject* code)
{
	env(NULL);
	jobject jCode = JyNI_JythonPyObject_FromPyObject(code);
	jint argcount = (*env)->GetIntField(env, jCode, pyBaseCode_co_argcount);
	return PyInt_FromLong(argcount);
}

PyObject* PyCode_Get_co_nlocals(PyObject* code)
{
	env(NULL);
	jobject jCode = JyNI_JythonPyObject_FromPyObject(code);
	jint nlocals = (*env)->GetIntField(env, jCode, pyBaseCode_co_nlocals);
	return PyInt_FromLong(nlocals);
}

PyObject* PyCode_Get_co_stacksize(PyObject* code)
{
	env(NULL);
	jobject jCode = JyNI_JythonPyObject_FromPyObject(code);
	if ((*env)->IsInstanceOf(env, jCode, pyBytecodeClass))
	{
		jint stacksize = (*env)->GetIntField(env, jCode, pyBytecode_co_stacksize);
		return PyInt_FromLong(stacksize);
	} else
	{
		return NULL;
	}
}

PyObject* PyCode_Get_co_flags(PyObject* code)
{
	env(NULL);
	jobject jCode = JyNI_JythonPyObject_FromPyObject(code);
	jint flags = (*env)->CallStaticIntMethod(env, JyNIClass, JyNI_pyCode_co_flags, jCode);
	return PyInt_FromLong(flags);
}

PyObject* PyCode_Get_co_code(PyObject* code)
{
	env(NULL);
	jobject jCode = JyNI_JythonPyObject_FromPyObject(code);
	jobject result = (*env)->CallStaticObjectMethod(env, JyNIClass, JyNI_pyCode_co_code, jCode);
	return JyNI_PyObject_FromJythonPyObject(result);
}

PyObject* PyCode_Get_co_consts(PyObject* code)
{
	env(NULL);
	jobject jCode = JyNI_JythonPyObject_FromPyObject(code);
	jobject jArray = (*env)->GetObjectField(env, jCode, pyBytecode_co_consts);
	Py_ssize_t size = 0;
	if (jArray) size = (*env)->GetArrayLength(env, jArray);
	PyTupleObject* result = PyTuple_New(size);
	Py_ssize_t i;
	for (i = 0; i < size; ++i)
		PyTuple_SET_ITEM(result, i, JyNI_PyObject_FromJythonPyObject(
			(*env)->GetObjectArrayElement(env, jArray, i)));
	return result;
}

PyObject* PyCode_Get_co_names(PyObject* code)
{
	env(NULL);
	jobject jCode = JyNI_JythonPyObject_FromPyObject(code);
	jobject jArray = (*env)->GetObjectField(env, jCode, pyBytecode_co_consts);
	jStringArray2pyTuple(jArray, result);
//	Py_ssize_t size = 0;
//	if (jArray) size = (*env)->GetArrayLength(env, jArray);
//	PyTupleObject* result = PyTuple_New(size);
//	if (size)
//	{
//		Py_ssize_t i;
//		char* utf_string;
//		jobject jstr;
//		for (i = 0; i < size; ++i)
//		{
//			jstr = (*env)->GetObjectArrayElement(env, jArray, i);
//			utf_string = (*env)->GetStringUTFChars(env, jstr, NULL);
//			char cstr[strlen(utf_string)+1];
//			strcpy(cstr, utf_string);
//			(*env)->ReleaseStringUTFChars(env, jstr, utf_string);
//			PyTuple_SET_ITEM(result, i, PyString_FromString(cstr));
//		}
//	}
	return result;
}

PyObject* PyCode_Get_co_varnames(PyObject* code)
{
	env(NULL);
	jobject jCode = JyNI_JythonPyObject_FromPyObject(code);
	jobject jArray = (*env)->GetObjectField(env, jCode, pyBaseCode_co_varnames);

	jStringArray2pyTuple(jArray, result);
//	Py_ssize_t size = 0;
//	if (jArray) size = (*env)->GetArrayLength(env, jArray);
//	PyTupleObject* result = PyTuple_New(size);
//	if (size)
//	{
//		Py_ssize_t i;
//		char* utf_string;
//		jobject jstr;
//		for (i = 0; i < size; ++i)
//		{
//			jstr = (*env)->GetObjectArrayElement(env, jArray, i);
//			utf_string = (*env)->GetStringUTFChars(env, jstr, NULL);
//			char cstr[strlen(utf_string)+1];
//			strcpy(cstr, utf_string);
//			(*env)->ReleaseStringUTFChars(env, jstr, utf_string);
//			PyTuple_SET_ITEM(result, i, PyString_FromString(cstr));
//		}
//	}
	return result;
}

PyObject* PyCode_Get_co_freevars(PyObject* code)
{
	env(NULL);
	jobject jCode = JyNI_JythonPyObject_FromPyObject(code);
	jobject jArray = (*env)->GetObjectField(env, jCode, pyBaseCode_co_freevars);
	jStringArray2pyTuple(jArray, result);
//	Py_ssize_t size = (*env)->GetArrayLength(env, jArray);
//	PyTupleObject* result = PyTuple_New(size);
//	Py_ssize_t i;
//	char* utf_string;
//	jobject jstr;
//	for (i = 0; i < size; ++i)
//	{
//		jstr = (*env)->GetObjectArrayElement(env, jArray, i);
//		utf_string = (*env)->GetStringUTFChars(env, jstr, NULL);
//		char cstr[strlen(utf_string)+1];
//		strcpy(cstr, utf_string);
//		(*env)->ReleaseStringUTFChars(env, jstr, utf_string);
//		PyTuple_SET_ITEM(result, i, PyString_FromString(cstr));
//	}
	return result;
}

PyObject* PyCode_Get_co_cellvars(PyObject* code)
{
	env(NULL);
	jobject jCode = JyNI_JythonPyObject_FromPyObject(code);
	jobject jArray = (*env)->GetObjectField(env, jCode, pyBaseCode_co_cellvars);
	jStringArray2pyTuple(jArray, result);
//	Py_ssize_t size = (*env)->GetArrayLength(env, jArray);
//	PyTupleObject* result = PyTuple_New(size);
//	Py_ssize_t i;
//	char* utf_string;
//	jobject jstr;
//	for (i = 0; i < size; ++i)
//	{
//		jstr = (*env)->GetObjectArrayElement(env, jArray, i);
//		utf_string = (*env)->GetStringUTFChars(env, jstr, NULL);
//		char cstr[strlen(utf_string)+1];
//		strcpy(cstr, utf_string);
//		(*env)->ReleaseStringUTFChars(env, jstr, utf_string);
//		PyTuple_SET_ITEM(result, i, PyString_FromString(cstr));
//	}
	return result;
}

PyObject* PyCode_Get_co_filename(PyObject* code)
{
	env(NULL);
	jobject jCode = JyNI_JythonPyObject_FromPyObject(code);
	jobject jstr = (*env)->GetObjectField(env, jCode, pyBaseCode_co_filename);
	cstr_from_jstring(cstr, jstr);
	return PyString_FromString(cstr);
}

PyObject* PyCode_Get_co_name(PyObject* code)
{
	env(NULL);
	jobject jCode = JyNI_JythonPyObject_FromPyObject(code);
	jobject jstr = (*env)->GetObjectField(env, jCode, pyCode_co_name);
	cstr_from_jstring(cstr, jstr);
	return PyString_FromString(cstr);
}

PyObject* PyCode_Get_co_firstlineno(PyObject* code)
{
	env(NULL);
	jobject jCode = JyNI_JythonPyObject_FromPyObject(code);
	jint firstlineno = (*env)->GetIntField(env, jCode, pyBaseCode_co_firstlineno);
	return PyInt_FromLong(firstlineno);
}

PyObject* PyCode_Get_co_lnotab(PyObject* code)
{
	env(NULL);
	jobject jCode = JyNI_JythonPyObject_FromPyObject(code);
	if ((*env)->IsInstanceOf(env, jCode, pyBytecodeClass))
	{
		jobject jstr = (*env)->CallStaticObjectMethod(env, JyNIClass, JyNI_pyCode_co_lnotab, jCode);
		cstr_from_jstring(cstr, jstr);
		return PyString_FromString(cstr);
	} else
	{
		return NULL;
	}
}

//#define OFF(x) offsetof(PyCodeObject, x)
//
//static PyMemberDef code_memberlist[] = {
//	{"co_argcount",		T_INT,		OFF(co_argcount),	READONLY},
//	{"co_nlocals",		T_INT,		OFF(co_nlocals),	READONLY},
//	{"co_stacksize",	T_INT,		OFF(co_stacksize),	READONLY},
//	{"co_flags",		T_INT,		OFF(co_flags),		READONLY},
//	{"co_code",			T_OBJECT,	OFF(co_code),		READONLY},
//	{"co_consts",		T_OBJECT,	OFF(co_consts),		READONLY},
//	{"co_names",		T_OBJECT,	OFF(co_names),		READONLY},
//	{"co_varnames",		T_OBJECT,	OFF(co_varnames),	READONLY},
//	{"co_freevars",		T_OBJECT,	OFF(co_freevars),	READONLY},
//	{"co_cellvars",		T_OBJECT,	OFF(co_cellvars),	READONLY},
//	{"co_filename",		T_OBJECT,	OFF(co_filename),	READONLY},
//	{"co_name",			T_OBJECT,	OFF(co_name),		READONLY},
//	{"co_firstlineno",	T_INT,		OFF(co_firstlineno),READONLY},
//	{"co_lnotab",		T_OBJECT,	OFF(co_lnotab),		READONLY},
//	{NULL}	  /* Sentinel */
//};

static PyGetSetDef code_getsets[] = {
	{"co_argcount",		(getter)PyCode_Get_co_argcount},
	{"co_nlocals",		(getter)PyCode_Get_co_nlocals},
	{"co_stacksize",	(getter)PyCode_Get_co_stacksize},
	{"co_flags",		(getter)PyCode_Get_co_flags},
	{"co_code",			(getter)PyCode_Get_co_code},
	{"co_consts",		(getter)PyCode_Get_co_consts},
	{"co_names",		(getter)PyCode_Get_co_names},
	{"co_varnames",		(getter)PyCode_Get_co_varnames},
	{"co_freevars",		(getter)PyCode_Get_co_freevars},
	{"co_cellvars",		(getter)PyCode_Get_co_cellvars},
	{"co_filename",		(getter)PyCode_Get_co_filename},
	{"co_name",			(getter)PyCode_Get_co_name},
	{"co_firstlineno",	(getter)PyCode_Get_co_firstlineno},
	{"co_lnotab",		(getter)PyCode_Get_co_lnotab},
	{NULL}
};

/* Helper for code_new: return a shallow copy of a tuple that is
   guaranteed to contain exact strings, by converting string subclasses
   to exact strings and complaining if a non-string is found. */
static PyObject*
validate_and_copy_tuple(PyObject *tup)
{
	PyObject *newtuple;
	PyObject *item;
	Py_ssize_t i, len;

	len = PyTuple_GET_SIZE(tup);
	newtuple = PyTuple_New(len);
	if (newtuple == NULL)
		return NULL;

	for (i = 0; i < len; i++) {
		item = PyTuple_GET_ITEM(tup, i);
		if (PyString_CheckExact(item)) {
			Py_INCREF(item);
		}
		else if (!PyString_Check(item)) {
			PyErr_Format(
				PyExc_TypeError,
				"name tuples must contain only "
				"strings, not '%.500s'",
				item->ob_type->tp_name);
			Py_DECREF(newtuple);
			return NULL;
		}
		else {
			item = PyString_FromStringAndSize(
				PyString_AS_STRING(item),
				PyString_GET_SIZE(item));
			if (item == NULL) {
				Py_DECREF(newtuple);
				return NULL;
			}
		}
		PyTuple_SET_ITEM(newtuple, i, item);
	}

	return newtuple;
}

PyDoc_STRVAR(code_doc,
"code(argcount, nlocals, stacksize, flags, codestring, constants, names,\n\
	  varnames, filename, name, firstlineno, lnotab[, freevars[, cellvars]])\n\
\n\
Create a code object.  Not for the faint of heart.");

static PyObject *
code_new(PyTypeObject *type, PyObject *args, PyObject *kw)
{
	int argcount;
	int nlocals;
	int stacksize;
	int flags;
	PyObject *co = NULL;
	PyObject *code;
	PyObject *consts;
	PyObject *names, *ournames = NULL;
	PyObject *varnames, *ourvarnames = NULL;
	PyObject *freevars = NULL, *ourfreevars = NULL;
	PyObject *cellvars = NULL, *ourcellvars = NULL;
	PyObject *filename;
	PyObject *name;
	int firstlineno;
	PyObject *lnotab;

	if (!PyArg_ParseTuple(args, "iiiiSO!O!O!SSiS|O!O!:code",
						  &argcount, &nlocals, &stacksize, &flags,
						  &code,
						  &PyTuple_Type, &consts,
						  &PyTuple_Type, &names,
						  &PyTuple_Type, &varnames,
						  &filename, &name,
						  &firstlineno, &lnotab,
						  &PyTuple_Type, &freevars,
						  &PyTuple_Type, &cellvars))
		return NULL;

	if (argcount < 0) {
		PyErr_SetString(
			PyExc_ValueError,
			"code: argcount must not be negative");
		goto cleanup;
	}

	if (nlocals < 0) {
		PyErr_SetString(
			PyExc_ValueError,
			"code: nlocals must not be negative");
		goto cleanup;
	}

	ournames = validate_and_copy_tuple(names);
	if (ournames == NULL)
		goto cleanup;
	ourvarnames = validate_and_copy_tuple(varnames);
	if (ourvarnames == NULL)
		goto cleanup;
	if (freevars)
		ourfreevars = validate_and_copy_tuple(freevars);
	else
		ourfreevars = PyTuple_New(0);
	if (ourfreevars == NULL)
		goto cleanup;
	if (cellvars)
		ourcellvars = validate_and_copy_tuple(cellvars);
	else
		ourcellvars = PyTuple_New(0);
	if (ourcellvars == NULL)
		goto cleanup;

	co = (PyObject *)PyCode_New(argcount, nlocals, stacksize, flags,
								code, consts, ournames, ourvarnames,
								ourfreevars, ourcellvars, filename,
								name, firstlineno, lnotab);
  cleanup:
	Py_XDECREF(ournames);
	Py_XDECREF(ourvarnames);
	Py_XDECREF(ourfreevars);
	Py_XDECREF(ourcellvars);
	return co;
}

static void
code_dealloc(PyCodeObject *co)
{
//	Py_XDECREF(co->co_code);
//	Py_XDECREF(co->co_consts);
//	Py_XDECREF(co->co_names);
//	Py_XDECREF(co->co_varnames);
//	JyNI-Note: restore actual size of the dummy-tuple.
	PyTuple_GET_SIZE(co->co_freevars) = 0;
	Py_XDECREF(co->co_freevars);
//	Py_XDECREF(co->co_cellvars);
//	Py_XDECREF(co->co_filename);
//	Py_XDECREF(co->co_name);
//	Py_XDECREF(co->co_lnotab);
//	if (co->co_zombieframe != NULL)
//		PyObject_GC_Del(co->co_zombieframe);
//	if (co->co_weakreflist != NULL)
//		PyObject_ClearWeakRefs((PyObject*)co);
	PyObject_DEL(co);
//	JyNI_Del(co);
}

static PyObject *
code_repr(PyCodeObject *co)
{
	env(NULL);
	jobject jCode = JyNI_JythonPyObject_FromPyObject(co);
	jobject repr_str = (*env)->CallObjectMethod(env, jCode, pyObject__repr__);
	return JyNI_PyObject_FromJythonPyObject(repr_str);

//	char buf[500];
//	int lineno = -1;
//	char *filename = "???";
//	char *name = "???";
//
//	if (co->co_firstlineno != 0)
//		lineno = co->co_firstlineno;
//	if (co->co_filename && PyString_Check(co->co_filename))
//		filename = PyString_AS_STRING(co->co_filename);
//	if (co->co_name && PyString_Check(co->co_name))
//		name = PyString_AS_STRING(co->co_name);
//	PyOS_snprintf(buf, sizeof(buf),
//				  "<code object %.100s at %p, file \"%.300s\", line %d>",
//				  name, co, filename, lineno);
//	return PyString_FromString(buf);
}

static int
code_compare(PyCodeObject *co, PyCodeObject *cp)
{
	env(NULL);
	jobject jCode = JyNI_JythonPyObject_FromPyObject(co);
	jobject cmpCode = JyNI_JythonPyObject_FromPyObject(cp);
	return (*env)->CallObjectMethod(env, jCode, pyObject__cmp__, cmpCode);
//	int cmp;
//	cmp = PyObject_Compare(co->co_name, cp->co_name);
//	if (cmp) return cmp;
//	cmp = co->co_argcount - cp->co_argcount;
//	if (cmp) goto normalize;
//	cmp = co->co_nlocals - cp->co_nlocals;
//	if (cmp) goto normalize;
//	cmp = co->co_flags - cp->co_flags;
//	if (cmp) goto normalize;
//	cmp = co->co_firstlineno - cp->co_firstlineno;
//	if (cmp) goto normalize;
//	cmp = PyObject_Compare(co->co_code, cp->co_code);
//	if (cmp) return cmp;
//	cmp = PyObject_Compare(co->co_consts, cp->co_consts);
//	if (cmp) return cmp;
//	cmp = PyObject_Compare(co->co_names, cp->co_names);
//	if (cmp) return cmp;
//	cmp = PyObject_Compare(co->co_varnames, cp->co_varnames);
//	if (cmp) return cmp;
//	cmp = PyObject_Compare(co->co_freevars, cp->co_freevars);
//	if (cmp) return cmp;
//	cmp = PyObject_Compare(co->co_cellvars, cp->co_cellvars);
//	return cmp;
//
// normalize:
//	if (cmp > 0)
//		return 1;
//	else if (cmp < 0)
//		return -1;
//	else
//		return 0;
}

//static PyObject *
//code_richcompare(PyObject *self, PyObject *other, int op)
//{
//	PyCodeObject *co, *cp;
//	int eq;
//	PyObject *res;
//
//	if ((op != Py_EQ && op != Py_NE) ||
//		!PyCode_Check(self) ||
//		!PyCode_Check(other)) {
//
//		/* Py3K warning if types are not equal and comparison
//		isn't == or !=  */
//		if (PyErr_WarnPy3k("code inequality comparisons not supported "
//						   "in 3.x", 1) < 0) {
//			return NULL;
//		}
//
//		Py_INCREF(Py_NotImplemented);
//		return Py_NotImplemented;
//	}
//
//	co = (PyCodeObject *)self;
//	cp = (PyCodeObject *)other;
//
//	eq = PyObject_RichCompareBool(co->co_name, cp->co_name, Py_EQ);
//	if (eq <= 0) goto unequal;
//	eq = co->co_argcount == cp->co_argcount;
//	if (!eq) goto unequal;
//	eq = co->co_nlocals == cp->co_nlocals;
//	if (!eq) goto unequal;
//	eq = co->co_flags == cp->co_flags;
//	if (!eq) goto unequal;
//	eq = co->co_firstlineno == cp->co_firstlineno;
//	if (!eq) goto unequal;
//	eq = PyObject_RichCompareBool(co->co_code, cp->co_code, Py_EQ);
//	if (eq <= 0) goto unequal;
//	eq = PyObject_RichCompareBool(co->co_consts, cp->co_consts, Py_EQ);
//	if (eq <= 0) goto unequal;
//	eq = PyObject_RichCompareBool(co->co_names, cp->co_names, Py_EQ);
//	if (eq <= 0) goto unequal;
//	eq = PyObject_RichCompareBool(co->co_varnames, cp->co_varnames, Py_EQ);
//	if (eq <= 0) goto unequal;
//	eq = PyObject_RichCompareBool(co->co_freevars, cp->co_freevars, Py_EQ);
//	if (eq <= 0) goto unequal;
//	eq = PyObject_RichCompareBool(co->co_cellvars, cp->co_cellvars, Py_EQ);
//	if (eq <= 0) goto unequal;
//
//	if (op == Py_EQ)
//		res = Py_True;
//	else
//		res = Py_False;
//	goto done;
//
//  unequal:
//	if (eq < 0)
//		return NULL;
//	if (op == Py_NE)
//		res = Py_True;
//	else
//		res = Py_False;
//
//  done:
//	Py_INCREF(res);
//	return res;
//}

static long
code_hash(PyCodeObject *co)
{
	env(NULL);
	jobject jCode = JyNI_JythonPyObject_FromPyObject(co);
	return (*env)->CallObjectMethod(env, jCode, pyObjectHashCode);
//	long h, h0, h1, h2, h3, h4, h5, h6;
//	h0 = PyObject_Hash(co->co_name);
//	if (h0 == -1) return -1;
//	h1 = PyObject_Hash(co->co_code);
//	if (h1 == -1) return -1;
//	h2 = PyObject_Hash(co->co_consts);
//	if (h2 == -1) return -1;
//	h3 = PyObject_Hash(co->co_names);
//	if (h3 == -1) return -1;
//	h4 = PyObject_Hash(co->co_varnames);
//	if (h4 == -1) return -1;
//	h5 = PyObject_Hash(co->co_freevars);
//	if (h5 == -1) return -1;
//	h6 = PyObject_Hash(co->co_cellvars);
//	if (h6 == -1) return -1;
//	h = h0 ^ h1 ^ h2 ^ h3 ^ h4 ^ h5 ^ h6 ^
//		co->co_argcount ^ co->co_nlocals ^ co->co_flags;
//	if (h == -1) h = -2;
//	return h;
}

/* XXX code objects need to participate in GC? */

PyTypeObject PyCode_Type = {
	PyVarObject_HEAD_INIT(&PyType_Type, 0)
	"code",
	sizeof(PyCodeObject),
	0,
	(destructor)code_dealloc,				/* tp_dealloc */
	0,										/* tp_print */
	0,										/* tp_getattr */
	0,										/* tp_setattr */
	(cmpfunc)code_compare,					/* tp_compare */
	(reprfunc)code_repr,					/* tp_repr */
	0,										/* tp_as_number */
	0,										/* tp_as_sequence */
	0,										/* tp_as_mapping */
	(hashfunc)code_hash,					/* tp_hash */
	0,										/* tp_call */
	0,										/* tp_str */
	PyObject_GenericGetAttr,				/* tp_getattro */
	0,										/* tp_setattro */
	0,										/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,						/* tp_flags */
	code_doc,								/* tp_doc */
	0,										/* tp_traverse */
	0,										/* tp_clear */
	0,//code_richcompare,						/* tp_richcompare */
	offsetof(PyCodeObject, co_weakreflist),	/* tp_weaklistoffset */
	0,										/* tp_iter */
	0,										/* tp_iternext */
	0,										/* tp_methods */
	0,//code_memberlist,					/* tp_members */
	code_getsets,							/* tp_getset */
	0,										/* tp_base */
	0,										/* tp_dict */
	0,										/* tp_descr_get */
	0,										/* tp_descr_set */
	0,										/* tp_dictoffset */
	0,										/* tp_init */
	0,										/* tp_alloc */
	code_new,								/* tp_new */
};

/* Use co_lnotab to compute the line number from a bytecode index, addrq.  See
   lnotab_notes.txt for the details of the lnotab representation.
*/

int
PyCode_Addr2Line(PyCodeObject *co, int addrq)
{
	env(NULL);
	jobject jCode = JyNI_JythonPyObject_FromPyObject(co);
	if (!(*env)->IsInstanceOf(env, jCode, pyBytecodeClass)) return -1;
	int line = (*env)->GetIntField(env, jCode, pyBaseCode_co_firstlineno);
	jobject jstr = (*env)->CallStaticObjectMethod(env, JyNIClass, JyNI_pyCode_co_lnotab, jCode);

	char* utf_string = (*env)->GetStringUTFChars(env, jstr, NULL);
	int size = strlen(utf_string);
	char cstr[size+1];
	strcpy(cstr, utf_string);
	(*env)->ReleaseStringUTFChars(env, jstr, utf_string);

//	int size = PyString_Size(co->co_lnotab) / 2;
	size = size/2;
	unsigned char *p = (unsigned char*) cstr;//PyString_AsString(co->co_lnotab);
	//int line = co->co_firstlineno;
	int addr = 0;
	while (--size >= 0) {
		addr += *p++;
		if (addr > addrq)
			break;
		line += *p++;
	}
	return line;
}

/* Update *bounds to describe the first and one-past-the-last instructions in
   the same line as lasti.  Return the number of that line. */
int
_PyCode_CheckLineNumber(PyCodeObject* co, int lasti, PyAddrPair *bounds)
{
	int size, addr, line;
	unsigned char* p;

	env(NULL);
	jobject jCode = JyNI_JythonPyObject_FromPyObject(co);
	if (!(*env)->IsInstanceOf(env, jCode, pyBytecodeClass)) return -1;
	line = (*env)->GetIntField(env, jCode, pyBaseCode_co_firstlineno);
	jobject jstr = (*env)->CallStaticObjectMethod(env, JyNIClass, JyNI_pyCode_co_lnotab, jCode);

	char* utf_string = (*env)->GetStringUTFChars(env, jstr, NULL);
	size = strlen(utf_string);
	char cstr[size+1];
	strcpy(cstr, utf_string);
	(*env)->ReleaseStringUTFChars(env, jstr, utf_string);


	p = (unsigned char*) cstr;//PyString_AS_STRING(co->co_lnotab);
	size = size/2;//PyString_GET_SIZE(co->co_lnotab) / 2;

	addr = 0;
	//line = co->co_firstlineno;
	assert(line > 0);

	/* possible optimization: if f->f_lasti == instr_ub
	   (likely to be a common case) then we already know
	   instr_lb -- if we stored the matching value of p
	   somwhere we could skip the first while loop. */

	/* See lnotab_notes.txt for the description of
	   co_lnotab.  A point to remember: increments to p
	   come in (addr, line) pairs. */

	bounds->ap_lower = 0;
	while (size > 0) {
		if (addr + *p > lasti)
			break;
		addr += *p++;
		if (*p)
			bounds->ap_lower = addr;
		line += *p++;
		--size;
	}

	if (size > 0) {
		while (--size >= 0) {
			addr += *p++;
			if (*p++)
				break;
		}
		bounds->ap_upper = addr;
	}
	else {
		bounds->ap_upper = INT_MAX;
	}

	return line;
}
