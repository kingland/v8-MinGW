// Copyright 2012 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "v8.h"

#include "assembler.h"
#include "base/once.h"
#include "base/platform/platform.h"
#include "bootstrapper.h"
#include "compiler/pipeline.h"
#include "debug.h"
#include "deoptimizer.h"
#include "elements.h"
#include "frames.h"
#include "heap/store-buffer.h"
#include "heap-profiler.h"
#include "hydrogen.h"
#include "isolate.h"
#include "lithium-allocator.h"
#include "objects.h"
#include "runtime-profiler.h"
#include "sampler.h"
#include "serialize.h"


namespace v8 {
namespace internal {

V8_DECLARE_ONCE(init_once);

v8::ArrayBuffer::Allocator* V8::array_buffer_allocator_ = NULL;
v8::Platform* V8::platform_ = NULL;


bool V8::Initialize(Deserializer* des) {
  InitializeOncePerProcess();
  Isolate* isolate = Isolate::UncheckedCurrent();
  if (isolate == NULL) return true;
  if (isolate->IsDead()) return false;
  if (isolate->IsInitialized()) return true;

  return isolate->Init(des);
}


void V8::TearDown() {
  Bootstrapper::TearDownExtensions();
  ElementsAccessor::TearDown();
  LOperand::TearDownCaches();
  compiler::Pipeline::TearDown();
  ExternalReference::TearDownMathExpData();
  RegisteredExtension::UnregisterAll();
  Isolate::GlobalTearDown();

  Sampler::TearDown();
}


void V8::SetReturnAddressLocationResolver(
      ReturnAddressLocationResolver resolver) {
  StackFrame::SetReturnAddressLocationResolver(resolver);
}


void V8::InitializeOncePerProcessImpl() {
  FlagList::EnforceFlagImplications();

  if (FLAG_predictable && FLAG_random_seed == 0) {
    // Avoid random seeds in predictable mode.
    FLAG_random_seed = 12347;
  }

  if (FLAG_stress_compaction) {
    FLAG_force_marking_deque_overflows = true;
    FLAG_gc_global = true;
    FLAG_max_semi_space_size = 1;
  }

  base::OS::Initialize(FLAG_random_seed, FLAG_hard_abort, FLAG_gc_fake_mmap);

  Sampler::SetUp();
  CpuFeatures::Probe(false);
  init_memcopy_functions();
  // The custom exp implementation needs 16KB of lookup data; initialize it
  // on demand.
  init_fast_sqrt_function();
#ifdef _WIN64
  init_modulo_function();
#endif
  ElementsAccessor::InitializeOncePerProcess();
  LOperand::SetUpCaches();
  compiler::Pipeline::SetUp();
  SetUpJSCallerSavedCodeData();
  ExternalReference::SetUp();
  Bootstrapper::InitializeOncePerProcess();
}


void V8::InitializeOncePerProcess() {
  base::CallOnce(&init_once, &InitializeOncePerProcessImpl);
}


void V8::InitializePlatform(v8::Platform* platform) {
  CHECK(!platform_);
  CHECK(platform);
  platform_ = platform;
}


void V8::ShutdownPlatform() {
  CHECK(platform_);
  platform_ = NULL;
}


v8::Platform* V8::GetCurrentPlatform() {
  DCHECK(platform_);
  return platform_;
}

} }  // namespace v8::internal