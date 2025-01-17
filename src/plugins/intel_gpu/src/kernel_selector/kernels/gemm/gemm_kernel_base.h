// Copyright (C) 2018-2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include "kernel_base_opencl.h"
#include "kernel_selector_params.h"

namespace kernel_selector {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// gemm_params
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct gemm_params : public base_params {
    gemm_params()
        : base_params(KernelType::GEMM), alpha(1.0f), beta(0.0f), transpose_input0(false), transpose_input1(false) {}

    float alpha;
    float beta;
    uint32_t transpose_input0;
    uint32_t transpose_input1;
    std::vector<int64_t> input0_order;
    std::vector<int64_t> input1_order;
    std::vector<int64_t> output_order;
    QuantizationType quantization = QuantizationType::NONE;

    ParamsKey GetParamsKey() const override {
        ParamsKey k = base_params::GetParamsKey();
        k.EnableQuantization(quantization);
        return k;
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// gemm_optional_params
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct gemm_optional_params : optional_params {
    gemm_optional_params() : optional_params(KernelType::GEMM) {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GemmKernelBase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GemmKernelBase : public KernelBaseOpenCL {
public:
    using KernelBaseOpenCL::KernelBaseOpenCL;
    using FusedOpDesc = fused_operation_desc;
    using DispatchData = CommonDispatchData;
    virtual ~GemmKernelBase() {}

protected:
    virtual JitConstants GetJitConstants(const gemm_params& params) const;
    virtual DispatchData SetDefault(const gemm_params& params) const;
    KernelsData GetCommonKernelsData(const Params& params, const optional_params&) const;

    std::string GetDimsOrder(const std::vector<int64_t>& order_idx) const;
    size_t GetOuputSize(const std::vector<int64_t>& output_order, const kernel_selector::DataTensor &output, char target_dim) const;
    std::vector<int64_t> ConvTo8dims(const std::vector<int64_t>& order_idx) const;
    std::vector<std::string> GetTransposedDims(const std::vector<int64_t>& order_idx, bool is_tiled_opt = false) const;

    // Fused ops
    virtual JitConstants GetFusedPrimitivesJitConstants(const gemm_params& params, const DispatchData& dispatchData) const;
    Datatype GetActivationType(const gemm_params& params) const;
    // --Fused ops
    bool Validate(const Params& p, const optional_params&) const override;
    void GetUpdateDispatchDataFunc(KernelData& kd) const override;
};
}  // namespace kernel_selector
