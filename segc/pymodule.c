#include <Python.h>

#include "dict.h"
#include "mempool.h"
#include "algor.h"

static PyObject*
pydict_load_words(PyObject* self,
                  PyObject* args)
{
    char* fn;
    if(!PyArg_ParseTuple(args, "s", &fn))
        return NULL;

    dict_load_words(fn);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject*
pydict_load_chars(PyObject* self,
                  PyObject* args)
{
    char* fn;
    if(!PyArg_ParseTuple(args, "s", &fn))
        return NULL;

    dict_load_chars(fn);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject*
pyalgo_next_token(PyObject* self,
                  PyObject* args)
{
    char* buf;
    if(!PyArg_ParseTuple(args, "s", &buf))
        return NULL;
    size_t psize = complex_algo_next_token(buf);
    return Py_BuildValue("i", psize);
}

#define DEF_VOID_METHOD(name, content)       \
    static PyObject*                         \
    name(PyObject* self, PyObject* args)     \
    {                                        \
        content();                           \
        Py_INCREF(Py_None);                  \
        return Py_None;                      \
    }                                        \


DEF_VOID_METHOD(pyalgo_init, complex_algo_init)
DEF_VOID_METHOD(pyalgo_deinit, complex_algo_deinit)
DEF_VOID_METHOD(pydict_init, dict_init)
DEF_VOID_METHOD(pydict_deinit, dict_deinit)
DEF_VOID_METHOD(pypool_init, pool_init)
DEF_VOID_METHOD(pypool_deinit, pool_deinit)


/*
 * Python module method defination
 */
static PyMethodDef CSegcMethods[] =
{
    {"dict_load_words", pydict_load_words,
     METH_VARARGS, "load words to the dict"},
    {"dict_load_chars", pydict_load_chars,
     METH_VARARGS, "load chars to the dict"},
    {"algo_init",       pyalgo_init,
     METH_VARARGS, "init the algorithm"},
    {"algo_next_token", pyalgo_next_token,
     METH_VARARGS, "get the next token"},
    {"algo_deinit",     pyalgo_deinit,
     METH_VARARGS, "deinit the algorithm"},
    {"dict_init",       pydict_init,
     METH_VARARGS, "init the dictionary"},
    {"dict_deinit",     pydict_deinit,
     METH_VARARGS, "deinit the dictionary"},
    {"pool_init",       pypool_init,
     METH_VARARGS, "init the memory pool"},
    {"pool_deinit",     pypool_deinit,
     METH_VARARGS, "free all the memory pool"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initcsegc()
{
    (void)Py_InitModule("csegc", CSegcMethods);
}


