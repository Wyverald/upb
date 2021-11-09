/*
 * Copyright (c) 2009-2021, Google LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Google LLC nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL Google LLC BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PYUPB_PROTOBUF_H__
#define PYUPB_PROTOBUF_H__

#include <stdbool.h>

#define Py_LIMITED_API 0x03060000
#include <Python.h>

// This function was not officially added to the limited API until Python 3.10.
// But in practice it has been stable since Python 3.1.  See:
//   https://bugs.python.org/issue41784
PyAPI_FUNC(const char *)
    PyUnicode_AsUTF8AndSize(PyObject *unicode, Py_ssize_t *size);

#include "upb/table_internal.h"

#define PYUPB_MODULE_NAME "google.protobuf.pyext._message"

// -----------------------------------------------------------------------------
// ModuleState
// -----------------------------------------------------------------------------

// We store all "global" state in this struct instead of using (C) global
// variables. This makes this extension compatible with sub-interpreters.

typedef struct {
  // From descriptor.c
  PyTypeObject *field_descriptor_type;
  PyTypeObject *file_descriptor_type;

  // From descriptor_containers.c
  PyTypeObject *by_name_map_type;
  PyTypeObject *by_number_map_type;
  PyTypeObject *descriptor_iterator_type;
  PyTypeObject *generic_sequence_type;

  // From descriptor_pool.c
  PyTypeObject *descriptor_pool_type;

  // From protobuf.c
  upb_arena *obj_cache_arena;
  upb_inttable obj_cache;
} PyUpb_ModuleState;

// Returns the global state object from the current interpreter. The current
// interpreter is looked up from thread-local state.
PyUpb_ModuleState *PyUpb_ModuleState_Get(void);
PyUpb_ModuleState *PyUpb_ModuleState_GetFromModule(PyObject *module);

// -----------------------------------------------------------------------------
// ObjectCache
// -----------------------------------------------------------------------------

// The ObjectCache is a weak map that maps C pointers to the corresponding
// Python wrapper object. We want a consistent Python wrapper object for each
// C object, both to save memory and to provide object stability (ie. x is x).
//
// Each wrapped object should add itself to the map when it is constructed and
// remove itself from the map when it is destroyed. The map is weak so it does
// not take references to the cached objects.

// Adds the given object to the cache, indexed by the given key.
void PyUpb_ObjCache_Add(const void *key, PyObject *py_obj);

// Removes the given key from the cache. It must exist in the cache currently.
void PyUpb_ObjCache_Delete(const void *key);

// Returns a new reference to an object if it exists, otherwise returns NULL.
PyObject *PyUpb_ObjCache_Get(const void *key);

// -----------------------------------------------------------------------------
// Utilities
// -----------------------------------------------------------------------------

PyTypeObject *AddObject(PyObject *m, const char *name, PyType_Spec *spec);

// Creates a Python type from `spec` and adds it to the given module `m`.
PyTypeObject *PyUpb_AddClass(PyObject *m, PyType_Spec *spec);

// Our standard dealloc func. It follows the guidance defined in:
//   https://docs.python.org/3/c-api/typeobj.html#c.PyTypeObject.tp_dealloc
// However it tests Py_TPFLAGS_HEAPTYPE dynamically so that a single dealloc
// function can work for any type.
static inline void PyUpb_Dealloc(void *self) {
  PyTypeObject *tp = Py_TYPE(self);
  freefunc tp_free = PyType_GetSlot(tp, Py_tp_free);
  tp_free(self);
  if (PyType_GetFlags(tp) & Py_TPFLAGS_HEAPTYPE) {
    Py_DECREF(tp);
  }
}

// Equivalent to the Py_NewRef() function introduced in Python 3.10.  If/when we
// drop support for Python <3.10, we can remove this function and replace all
// callers with Py_NewRef().
static inline PyObject *PyUpb_NewRef(PyObject *obj) {
  Py_INCREF(obj);
  return obj;
}

const char *PyUpb_GetStrData(PyObject *obj);

#endif  // PYUPB_PROTOBUF_H__