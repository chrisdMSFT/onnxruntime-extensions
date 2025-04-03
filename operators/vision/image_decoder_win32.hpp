// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#pragma comment(lib, "Windowscodecs.lib")

#include <wincodec.h>
#include <wincodecsdk.h>
// #include <winrt/base.h>

#include "op_def_struct.h"
#include "ext_status.h"

namespace ort_extensions::internal {
struct DecodeImage {
  OrtxStatus OnInit() {
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
      return errorWithHr_("Failed when CoInitialize.", hr);
    }
    // Create the COM imaging factory
    hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pIWICFactory_));
    if (FAILED(hr)) {
      return errorWithHr_("Failed to create pIWICFactory.", hr);
    }

    return {};
  }

  OrtxStatus Compute(const ortc::Tensor<uint8_t>& input, ortc::Tensor<uint8_t>& output) const{
	  return errorWithHr_("Not Implemeneted", E_FAIL);
  }

  ~DecodeImage() {
    if (pIWICFactory_) {
      pIWICFactory_->Release();
      pIWICFactory_ = NULL;
    }
    CoUninitialize();
  }

  private:
    OrtxStatus errorWithHr_(const std::string message, HRESULT hr) const{
     return {
       kOrtxErrorInternal,
       "[ImageDecoder]: " + message + " HRESULT: " + std::to_string(hr)};
    }
    IWICImagingFactory* pIWICFactory_{NULL};
};
}  // namespace ort_extensions::internal
