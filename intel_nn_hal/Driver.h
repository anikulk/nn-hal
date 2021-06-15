/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_ML_NN_OV_DRIVER_H
#define ANDROID_ML_NN_OV_DRIVER_H
#include <android/hardware/neuralnetworks/1.0/IDevice.h>
#include <android/hardware/neuralnetworks/1.0/IExecutionCallback.h>
#include <android/hardware/neuralnetworks/1.0/IPreparedModel.h>
#include <android/hardware/neuralnetworks/1.0/IPreparedModelCallback.h>
#include <android/hardware/neuralnetworks/1.0/types.h>
#include <android/hardware/neuralnetworks/1.1/IDevice.h>
#include <android/hardware/neuralnetworks/1.2/IDevice.h>
#include <android/hardware/neuralnetworks/1.3/IDevice.h>
#include <android/hardware/neuralnetworks/1.2/IExecutionCallback.h>
#include <android/hardware/neuralnetworks/1.3/IExecutionCallback.h>
#include <android/hardware/neuralnetworks/1.2/IPreparedModel.h>
#include <android/hardware/neuralnetworks/1.3/IPreparedModel.h>
#include <android/hardware/neuralnetworks/1.2/IPreparedModelCallback.h>
#include <android/hardware/neuralnetworks/1.3/IPreparedModelCallback.h>
#include <android/hardware/neuralnetworks/1.1/types.h>
#include <android/hardware/neuralnetworks/1.2/types.h>
#include <android/hardware/neuralnetworks/1.3/types.h>
#include <string>

namespace android {
namespace hardware {
namespace neuralnetworks {
namespace nnhal {

using Model = ::android::hardware::neuralnetworks::V1_3::Model;
using Operand = ::android::hardware::neuralnetworks::V1_3::Operand;
using V1_0_Model = ::android::hardware::neuralnetworks::V1_0::Model;
using V1_1_Model = ::android::hardware::neuralnetworks::V1_1::Model;
using V1_2_Model = ::android::hardware::neuralnetworks::V1_2::Model;
using Operation = ::android::hardware::neuralnetworks::V1_3::Operation;
using V1_0_Operation = ::android::hardware::neuralnetworks::V1_0::Operation;
using V1_1_Operation = ::android::hardware::neuralnetworks::V1_1::Operation;
using V1_2_Operation = ::android::hardware::neuralnetworks::V1_2::Operation;
using OperationType = ::android::hardware::neuralnetworks::V1_3::OperationType;
using OperandType = ::android::hardware::neuralnetworks::V1_3::OperandType;
using Capabilities = ::android::hardware::neuralnetworks::V1_3::Capabilities;
using V1_0_Capabilities = ::android::hardware::neuralnetworks::V1_0::Capabilities;
using V1_1_Capabilities = ::android::hardware::neuralnetworks::V1_1::Capabilities;
using V1_2_Capabilities = ::android::hardware::neuralnetworks::V1_2::Capabilities;
using V1_0_ErrorStatus = ::android::hardware::neuralnetworks::V1_0::ErrorStatus;
using ErrorStatus = ::android::hardware::neuralnetworks::V1_3::ErrorStatus;
using V1_3_OperandLifeTime = ::android::hardware::neuralnetworks::V1_3::OperandLifeTime;
using OperandLifeTime = ::android::hardware::neuralnetworks::V1_3::OperandLifeTime;
using V1_0_Request = ::android::hardware::neuralnetworks::V1_0::Request;
using Request = ::android::hardware::neuralnetworks::V1_3::Request;

using namespace ::android::hardware::neuralnetworks::V1_1;
using namespace ::android::hardware::neuralnetworks::V1_0;
using namespace ::android::hardware::neuralnetworks::V1_2;
using namespace ::android::hardware::neuralnetworks::V1_3;
using ::android::hardware::MQDescriptorSync;

using HidlToken = android::hardware::hidl_array<uint8_t, 32>;
using CacheToken =
	        android::hardware::hidl_array<uint8_t, static_cast<uint32_t>(Constant::BYTE_SIZE_OF_CACHE_TOKEN)>;

// Base class used to create vpu drivers for the NN HAL.  This class
// provides some implementation of the more common functions.
//
// Since these drivers simulate hardware, they must run the computations
// on the CPU.  An actual driver would not do that.
class Driver : public ::android::hardware::neuralnetworks::V1_3::IDevice {
public:
    Driver() {}
    Driver(const char* name) : mName(name) {}

    ~Driver() override {}
    Return<void> getCapabilities(getCapabilities_cb cb) override;
    Return<void> getCapabilities_1_1(getCapabilities_1_1_cb cb) override;
    Return<void> getCapabilities_1_2(getCapabilities_1_2_cb cb) override;
    Return<void> getCapabilities_1_3(getCapabilities_1_3_cb cb) override;
    Return<void> getSupportedOperations(const V1_0_Model& model,
                                        getSupportedOperations_cb cb) override;
    Return<void> getSupportedOperations_1_1(const V1_1_Model& model,
                                            getSupportedOperations_1_1_cb cb) override;
    Return<void> getSupportedOperations_1_2(const V1_2_Model& model,
                                            getSupportedOperations_1_2_cb cb) override;
    Return<void> getSupportedOperations_1_3(const Model& model,
                                            getSupportedOperations_1_3_cb cb) override;
    Return<V1_0_ErrorStatus> prepareModel(const V1_0_Model& model,
                                     const sp<V1_0::IPreparedModelCallback>& callback) override;
    Return<V1_0_ErrorStatus> prepareModel_1_1(const V1_1_Model& model, ExecutionPreference preference,
                                         const sp<V1_0::IPreparedModelCallback>& callback) override;
    Return<V1_0_ErrorStatus> prepareModel_1_2(const V1_2_Model& model, ExecutionPreference preference,
                                         const hidl_vec<hidl_handle>& modelCache,
                                         const hidl_vec<hidl_handle>& dataCache,
                                         const HidlToken& token,
                                         const sp<V1_2::IPreparedModelCallback>& callback) override;
    Return<ErrorStatus> prepareModel_1_3(const Model& model, ExecutionPreference preference,
                                         Priority priority,
					                     const OptionalTimePoint& deadline,
                                         const hidl_vec<hidl_handle>& modelCache,
				                         const hidl_vec<hidl_handle>& dataCache,
                                         const HidlToken& token,
                                         const sp<V1_3::IPreparedModelCallback>& callback) override;
    Return<ErrorStatus> prepareModelFromCache_1_3(
                                         const OptionalTimePoint&,
                                         const hidl_vec<hidl_handle>& modelCache,
                                         const hidl_vec<hidl_handle>& dataCache,
                                         const HidlToken& token,
                                         const sp<V1_3::IPreparedModelCallback>& callback) override;
    Return<V1_0_ErrorStatus> prepareModelFromCache(
        const hidl_vec<hidl_handle>& modelCache, const hidl_vec<hidl_handle>& dataCache,
        const HidlToken& token, const sp<V1_2::IPreparedModelCallback>& callback) override;
    Return<DeviceStatus> getStatus() override;
    Return<void> getVersionString(getVersionString_cb cb) override;
    Return<void> getType(getType_cb cb) override;
    Return<void> getSupportedExtensions(getSupportedExtensions_cb) override;
    Return<void> getNumberOfCacheFilesNeeded(getNumberOfCacheFilesNeeded_cb cb) override;

    Return<void> allocate(const BufferDesc& desc,
		          const hidl_vec<sp<V1_3::IPreparedModel>>& preparedModels,
		          const hidl_vec<BufferRole>& inputRoles,
			  const hidl_vec<BufferRole>& outputRoles,
			  allocate_cb cb) override;
protected:
    std::string mName;
};

}  // namespace nnhal
}  // namespace neuralnetworks
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_ML_NN_OV_DRIVER_H
