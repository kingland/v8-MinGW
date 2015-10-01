// Copyright 2014 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "v8.h"
#include "hydrogen-types.h"

#include "ostreams.h"
#include "types-inl.h"


namespace v8 {
namespace internal {

// static
template <class T>
HType HType::FromType(typename T::TypeHandle type) {
  if (T::Any()->Is(type)) return HType::Any();
  if (type->Is(T::None())) return HType::None();
  if (type->Is(T::SignedSmall())) return HType::Smi();
  if (type->Is(T::Number())) return HType::TaggedNumber();
  if (type->Is(T::Null())) return HType::Null();
  if (type->Is(T::String())) return HType::String();
  if (type->Is(T::Boolean())) return HType::Boolean();
  if (type->Is(T::Undefined())) return HType::Undefined();
  if (type->Is(T::Array())) return HType::JSArray();
  if (type->Is(T::Object())) return HType::JSObject();
  return HType::Tagged();
}


// static
template
HType HType::FromType<Type>(Type* type);


// static
template
HType HType::FromType<HeapType>(Handle<HeapType> type);


// static
HType HType::FromValue(Handle<Object> value) {
  if (value->IsSmi()) return HType::Smi();
  if (value->IsNull()) return HType::Null();
  if (value->IsHeapNumber()) return HType::HeapNumber();
  if (value->IsString()) return HType::String();
  if (value->IsBoolean()) return HType::Boolean();
  if (value->IsUndefined()) return HType::Undefined();
  if (value->IsJSArray()) return HType::JSArray();
  if (value->IsJSObject()) return HType::JSObject();
  DCHECK(value->IsHeapObject());
  return HType::HeapObject();
}


OStream& operator<<(OStream& os, const HType& t) {
  // Note: The c1visualizer syntax for locals allows only a sequence of the
  // following characters: A-Za-z0-9_-|:
  switch (t.kind_) {
#define DEFINE_CASE(Name, mask) \
  case HType::k##Name:          \
    return os << #Name;
    HTYPE_LIST(DEFINE_CASE)
#undef DEFINE_CASE
  }
  UNREACHABLE();
  return os;
}

} }  // namespace v8::internal
