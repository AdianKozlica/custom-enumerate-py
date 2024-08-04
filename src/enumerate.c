#include <Python.h>
#include <structmember.h>

typedef struct {
    PyObject_HEAD
    long long current;
    PyObject* iterable;
    PyObject* iter;
} EnumerateObject;

static PyTypeObject EnumerateObject_Type;

static EnumerateObject*
new_enumerate_object(PyObject* args) {
    EnumerateObject* self;
    self = PyObject_NEW(EnumerateObject, &EnumerateObject_Type);
    return self;
}

static int
enumerate_object_init(EnumerateObject* self, PyObject* args) {
    PyObject* iterable_obj;
    
    if(!PyArg_ParseTuple(args, "O", &iterable_obj)) {
        PyErr_SetString(PyExc_TypeError, "Invalid type");
        return -1;
    }

    if (PyIter_Check(iterable_obj) != 0) {
        PyErr_SetString(PyExc_TypeError, "Object is not iterable");
        return -1;
    }
    
    self->iterable= iterable_obj;
    self->iter = PyObject_GetIter(self->iterable);
    self->current = 0;
    return 0;
}

static void
enumerate_dealloc(EnumerateObject* self) {
    PyObject_Free(self);
}

static PyObject*
enumerate_iter(EnumerateObject* self) {
    Py_INCREF(self);
    self->current = 0;
    return self;
}

static PyObject*
enumerate_next(EnumerateObject* self) {
    PyObject* val = PyIter_Next(self->iter);

    if (val == NULL) {
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }
    
    PyObject* tuple = PyTuple_New(2);
    
    PyTuple_SetItem(tuple, 0, PyLong_FromLong(self->current++));
    PyTuple_SetItem(tuple, 1, val);

    return tuple;
}

static PyTypeObject EnumerateObject_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "enumerate_module.EnumerateObject", /*tp_name*/
    sizeof(EnumerateObject),            /*tp_basicsize*/
    0,                                  /*tp_itemsize*/
    /* methods */   
    (destructor)enumerate_dealloc,     /*tp_dealloc*/
    0,                                  /*tp_vectorcall_offset*/
    (getattrfunc)0,                     /*tp_getattr*/
    (setattrfunc)0,                     /*tp_setattr*/
    0,                                  /*tp_as_async*/
    0,                                  /*tp_repr*/
    0,                                  /*tp_as_number*/
    0,                                  /*tp_as_sequence*/
    0,                                  /*tp_as_mapping*/
    0,                                  /*tp_hash*/
    0,                                  /*tp_call*/
    0,                                  /*tp_str*/
    0,                                  /*tp_getattro*/     
    0,                                  /*tp_setattro*/
    0,                                  /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,                 /*tp_flags*/
    0,                                  /*tp_doc*/
    0,                                  /*tp_traverse*/
    0,                                  /*tp_clear*/
    0,                                  /*tp_richcompare*/
    0,                                  /*tp_weaklistoffset*/
    enumerate_iter,                     /*tp_iter*/
    (iternextfunc)enumerate_next,       /*tp_iternext*/
    0,                                  /*tp_methods*/
    0,                                  /*tp_members*/
    0,                                  /*tp_getset*/
    0,                                  /*tp_base*/
    0,                                  /*tp_dict*/
    0,                                  /*tp_descr_get*/
    0,                                  /*tp_descr_set*/
    0,                                  /*tp_dictoffset*/
    enumerate_object_init,              /*tp_init*/
    0,                                  /*tp_alloc*/
    (newfunc)new_enumerate_object,      /*tp_new*/
    0,                                  /*tp_free*/
    0,                                  /*tp_is_gc*/
};

static struct PyModuleDef enumerate_module = {
    PyModuleDef_HEAD_INIT,
    "enumerate_module",
    "Custom enumerate_module",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_enumerate_module() {
    PyObject* m;
    
    if (PyType_Ready(&EnumerateObject_Type) < 0) {
        return NULL;
    }

    m = PyModule_Create(&enumerate_module);

    if (m == NULL) {
        return NULL;
    }

    Py_INCREF(&EnumerateObject_Type);

    if (PyModule_AddObject(m, "EnumerateObject",(PyObject *)&EnumerateObject_Type) < 0) {
        Py_DECREF(&EnumerateObject_Type);
        Py_DECREF(m);
        return NULL;
    }
    
    return m;
}