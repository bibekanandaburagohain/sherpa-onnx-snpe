//==============================================================================
//
//  Copyright (c) 2019 Qualcomm Technologies, Inc.
//  All Rights Reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================

#ifndef LOADCONTAINER_H
#define LOADCONTAINER_H

#include <string>

#include "../zdl/DlContainer/IDlContainer.hpp"

std::unique_ptr<zdl::DlContainer::IDlContainer> loadContainerFromFile(std::string containerPath);
std::unique_ptr<zdl::DlContainer::IDlContainer> loadContainerFromBuffer(const uint8_t * buffer, const size_t size);

#endif
