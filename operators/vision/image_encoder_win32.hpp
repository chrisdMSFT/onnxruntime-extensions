// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#pragma comment(lib, "Windowscodecs.lib")

#include <wincodec.h>
#include <wincodecsdk.h>

#include "op_def_struct.h"
#include "ext_status.h"

namespace ort_extensions::internal {
struct EncodeImage {
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

  bool JpgSupportsBgr() const{ return true; }

  OrtxStatus EncodeJpg(const uint8_t* source_data, bool source_is_bgr, int32_t width, int32_t height,
                uint8_t** outbuffer, size_t* outsize) const {
    std::vector<PROPBAG2> options;
    std::vector<VARIANT> values;

    {
      PROPBAG2 option = {0};
      option.pstrName = const_cast<wchar_t*>(L"ImageQuality");
      options.push_back(option);
      VARIANT varValue;
      VariantInit(&varValue);
      varValue.vt = VT_R4;
      varValue.fltVal = static_cast<FLOAT>(0.95);
      values.push_back(varValue);
    }

    {
      PROPBAG2 option = {0};
      option.pstrName = const_cast<wchar_t*>(L"JpegYCrCbSubsampling");
      options.push_back(option);
      VARIANT varValue;
      VariantInit(&varValue);
      varValue.vt = VT_UI1;
      varValue.bVal = WICJpegYCrCbSubsampling420;
      values.push_back(varValue);
    }

    return EncodeWith(GUID_ContainerFormatJpeg, options, values, source_data, source_is_bgr,
        width, height, outbuffer,outsize);
  }

  bool pngSupportsBgr() const{ return true; }

  OrtxStatus EncodePng(const uint8_t* source_data, bool source_is_bgr, int32_t width, int32_t height,
                uint8_t** outbuffer,size_t* outsize) const{
    std::vector<PROPBAG2> options;
    std::vector<VARIANT> values;

    {
      PROPBAG2 option = {0};
      option.pstrName = const_cast<wchar_t*>(L"InterlaceOption");
      options.push_back(option);
      VARIANT varValue;
      VariantInit(&varValue);
      varValue.vt = VT_BOOL;
      varValue.boolVal = FALSE;
      values.push_back(varValue);
    }

    {
      PROPBAG2 option = {0};
      option.pstrName = const_cast<wchar_t*>(L"FilterOption");
      options.push_back(option);
      VARIANT varValue;
      VariantInit(&varValue);
      varValue.vt = VT_UI1;
      varValue.bVal = WICPngFilterSub;
      values.push_back(varValue);
    }

    return EncodeWith(GUID_ContainerFormatPng, options, values, source_data, source_is_bgr, width, height,
                      outbuffer, outsize);
  }

  ~EncodeImage() {
    if (pIWICFactory_) {
      pIWICFactory_->Release();
      pIWICFactory_ = NULL;
    }
    CoUninitialize();
  }

  private:
   OrtxStatus EncodeWith(GUID conatinerFormatGUID, std::vector<PROPBAG2> options, std::vector<VARIANT> values,
                   const uint8_t* source_data, bool source_is_bgr, int32_t width, int32_t height, uint8_t** outbuffer,
                   size_t* outsize) const {

     return errorWithHr_("Not Implemeneted", E_FAIL);

   }

   OrtxStatus errorWithHr_(const std::string message, HRESULT hr) const {
     return {kOrtxErrorInternal, "[ImageEncoder]: " + message + " HRESULT: " + std::to_string(hr)};
   }

   IWICImagingFactory* pIWICFactory_{NULL};
};
}
