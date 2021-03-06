// Copyright 2013 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "v8.h"
#include "libplatform/worker-thread.h"

#include "include/v8-platform.h"
#include "libplatform/task-queue.h"

namespace v8 {
namespace platform {

WorkerThread::WorkerThread(TaskQueue* queue)
    : Thread(Options("V8 WorkerThread")), queue_(queue) {
  Start();
}


WorkerThread::~WorkerThread() {
  Join();
}


void WorkerThread::Run() {
  while (Task* task = queue_->GetNext()) {
    task->Run();
    delete task;
  }
}

} }  // namespace v8::platform
