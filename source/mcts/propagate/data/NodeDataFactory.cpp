//
// Created by bjorn on 6-5-2026.
//

#include "NodeDataFactory.h"

std::shared_ptr<NodeData> NodeDataFactory::create() const {
    return policy->createNodeData();
}
