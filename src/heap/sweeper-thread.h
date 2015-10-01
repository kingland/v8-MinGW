// Copyright 2012 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_HEAP_SWEEPER_THREAD_H_
#define V8_HEAP_SWEEPER_THREAD_H_

#include "base/atomicops.h"
#include "base/platform/platform.h"
#include "flags.h"
#include "utils.h"

#include "heap/spaces.h"

#include "heap/heap.h"

namespace v8 {
namespace internal {

class SweeperThread : public base::Thread {
 public:
  explicit SweeperThread(Isolate* isolate);
  ~SweeperThread() {}

  void Run();
  void Stop();
  void StartSweeping();
  void WaitForSweeperThread();
  bool SweepingCompleted();

  static int NumberOfThreads(int max_available);

 private:
  Isolate* isolate_;
  Heap* heap_;
  MarkCompactCollector* collector_;
  base::Semaphore start_sweeping_semaphore_;
  base::Semaphore end_sweeping_semaphore_;
  base::Semaphore stop_semaphore_;
  volatile base::AtomicWord stop_thread_;
};
}
}  // namespace v8::internal

#endif  // V8_HEAP_SWEEPER_THREAD_H_
