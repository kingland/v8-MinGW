// Copyright 2013 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "compiler/graph.h"

#include "compiler/common-operator.h"
#include "compiler/generic-node-inl.h"
#include "compiler/graph-inl.h"
#include "compiler/node.h"
#include "compiler/node-aux-data-inl.h"
#include "compiler/node-properties.h"
#include "compiler/node-properties-inl.h"
#include "compiler/operator-properties.h"
#include "compiler/operator-properties-inl.h"

namespace v8 {
namespace internal {
namespace compiler {

Graph::Graph(Zone* zone)
    : GenericGraph<Node>(zone),
      decorators_(DecoratorVector::allocator_type(zone)) {}


Node* Graph::NewNode(Operator* op, int input_count, Node** inputs) {
  DCHECK(op->InputCount() <= input_count);
  Node* result = Node::New(this, input_count, inputs);
  result->Initialize(op);
  for (DecoratorVector::iterator i = decorators_.begin();
       i != decorators_.end(); ++i) {
    (*i)->Decorate(result);
  }
  return result;
}


void Graph::ChangeOperator(Node* node, Operator* op) { node->set_op(op); }


void Graph::DeleteNode(Node* node) {
#if DEBUG
  // Nodes can't be deleted if they have uses.
  Node::Uses::iterator use_iterator(node->uses().begin());
  DCHECK(use_iterator == node->uses().end());
#endif

#if DEBUG
  memset(node, 0xDE, sizeof(Node));
#endif
}
}
}
}  // namespace v8::internal::compiler
