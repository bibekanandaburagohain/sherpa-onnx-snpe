//==============================================================================
//
//  Copyright (c) 2017-2019 Qualcomm Technologies, Inc.
//  All Rights Reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================

#ifndef SETBUILDEROPTIONS_H
#define SETBUILDEROPTIONS_H

#include "../zdl/DlSystem/RuntimeList.hpp"
#include "../zdl/SNPE/SNPE.hpp"
#include "../zdl/DlSystem/DlEnums.hpp"
//#include "DlSystem/UDLFunc.hpp"
#include "../zdl/DlContainer/IDlContainer.hpp"
#include "../zdl/DlSystem/PlatformConfig.hpp"

std::unique_ptr<zdl::SNPE::SNPE> setBuilderOptions(std::unique_ptr<zdl::DlContainer::IDlContainer> & container,
                                                   zdl::DlSystem::Runtime_t runtime,
                                                   zdl::DlSystem::RuntimeList runtimeList,
                                                   bool useUserSuppliedBuffers,
                                                   bool useCaching);

#endif //SETBUILDEROPTIONS_H