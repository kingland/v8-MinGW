// Copyright 2014 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "v8.h"

#include "field-index.h"
#include "objects.h"
#include "objects-inl.h"

namespace v8 {
namespace internal {


FieldIndex FieldIndex::ForLookupResult(const LookupResult* lookup_result) {
  Map* map = lookup_result->holder()->map();
  return ForPropertyIndex(map,
                          lookup_result->GetFieldIndexFromMap(map),
                          lookup_result->representation().IsDouble());
}


} }  // namespace v8::internal
