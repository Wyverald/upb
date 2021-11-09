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

#ifndef PYUPB_DESCRIPTOR_H__
#define PYUPB_DESCRIPTOR_H__

#include <stdbool.h>

#include "protobuf.h"
#include "upb/def.h"

PyObject *PyUpb_FieldDescriptor_GetOrCreateWrapper(const upb_fielddef *field,
                                                   PyObject *pool);
PyObject *PyUpb_FileDescriptor_GetOrCreateWrapper(const upb_filedef *file,
                                                  PyObject *pool);

const upb_filedef *PyUpb_FileDescriptor_GetDef(PyObject *file);

// Returns the underlying |def| for a given wrapper object. The caller must
// have already verified that the given Python object is of the expected type.
const upb_filedef *PyUpb_FileDescriptor_GetDef(PyObject *file);
const void *PyUpb_AnyDescriptor_GetDef(PyObject *_self);

bool PyUpb_InitDescriptor(PyObject *m);

#endif  // PYUPB_DESCRIPTOR_H__