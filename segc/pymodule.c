#include <Python.h>

#include "dict.h"
#include "mempool.h"
#include "algor.h"

/**
 * aquire_global_resources:
 * @self: the python self object
 * @args: the python arguments
 *
 * this function initialize all the static resource, thus
 * is NOT thread safe!
 *
 * Returns: a Py_None
 **/
PyObject*
acquire_global_resources(PyObject* self,
                         PyObject* args)
{
    const char* char_dict = NULL;
    const char* word_dict = NULL;

    PyArg_ParseTuple(args, "ss", &char_dict, &word_dict);
    pool_init();
    dict_init();
    dict_load_words(word_dict);
    dict_load_chars(char_dict);

    Py_INCREF(Py_None);

    return Py_None;
}

/**
 * free_global_resources:
 * @self: the python self object
 * @args: the python arguments
 *
 * release the static data aquired by
 * aquire_global_resources(), like aquire_global_resources()
 * it's not thread safe!
 *
 * Returns: a Py_None
 **/
PyObject*
free_global_resources(PyObject* self,
                      PyObject* args)
{
    dict_deinit();
    pool_deinit();

    Py_INCREF(Py_None);
    return Py_None;
}

/**
 * create_algor_object:
 * @self: the python self object
 * @args: the python arguments
 *
 * create the algorithm object containing the target string
 * to split into chinese words, this method mainly allocate
 * memory, so it's thread safe.
 *
 * Returns: a PyCObject containing a algor_t pointer
 **/
PyObject*
create_algor_object(PyObject* self,
                    PyObject* args)
{
    const char* buf = NULL;
    PyArg_ParseTuple(args, "s", &buf);
    algor_t* al = algor_new(buf);

    //this will create a new ref, so we don't have to incref
    return PyCObject_FromVoidPtr(al, NULL);
}

/**
 * destroy_algor_object:
 * @self: the python self object
 * @args: the python arguments
 *
 * destroy the algor object, it's thread safe
 *
 * Returns: a Py_None
 **/
PyObject*
destroy_algor_object(PyObject* self,
                     PyObject* args)
{
    PyObject* cobj = NULL;
    PyArg_ParseTuple(args, "O", &cobj);

    algor_t* al = PyCObject_AsVoidPtr(cobj);
    algor_destroy(al);

    Py_INCREF(Py_None);
    return Py_None;
}

/**
 * get_algor_next_token:
 * @self: the python self object
 * @args: the python arguments
 *
 * parse the next token
 *
 * Returns: a int, but wrapped into python object
 **/
PyObject*
get_algor_next_token(PyObject* self,
                     PyObject* args)
{
    PyObject* cobj = NULL;
    PyArg_ParseTuple(args, "O", &cobj);

    algor_t* al = PyCObject_AsVoidPtr(cobj);
    size_t val =  algor_get_next_token(al);

    return Py_BuildValue("i", val);
}


/* modules methods */
PyMethodDef methods[] =
    {
        {"acquire_global_resources", acquire_global_resources, METH_VARARGS},
        {"free_global_resources", free_global_resources, METH_VARARGS},
        {"create_algor_object", create_algor_object, METH_VARARGS},
        {"destroy_algor_object", destroy_algor_object, METH_VARARGS},
        {"get_algor_next_token", get_algor_next_token, METH_VARARGS},
        {NULL, NULL},
    };

/* init the python module */
void
initcsegc(void)
{
    Py_InitModule("csegc", methods);
}
