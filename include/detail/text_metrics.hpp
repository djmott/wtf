#pragma once

namespace wtf{
  struct text_metrics : TEXTMETRIC{
    text_metrics() = default;
    text_metrics(const TEXTMETRIC& src) : TEXTMETRIC(src){}
    text_metrics(const text_metrics& src) : TEXTMETRIC(src){}
    static text_metrics get(const device_context& dc){
      text_metrics oRet;
      wtf::exception::throw_lasterr_if(::GetTextMetrics(dc, &oRet), [](BOOL b){ return !b; });
      return oRet;
    }
  };
}