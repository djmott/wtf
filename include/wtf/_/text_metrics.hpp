/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace _{
    struct text_metrics : TEXTMETRIC{
      ~text_metrics() = default;
      text_metrics() = default;
      text_metrics(const TEXTMETRIC& src) noexcept : TEXTMETRIC(src){}
      text_metrics(const text_metrics& src) noexcept : TEXTMETRIC(src){}
      text_metrics(text_metrics&& src) noexcept : TEXTMETRIC(src) {}
      text_metrics& operator=(const text_metrics&) = default;
      text_metrics& operator=(text_metrics&&) = default;
      static text_metrics get(const device_context& dc)  {
        text_metrics oRet;
        wtf::exception::throw_lasterr_if(::GetTextMetrics(dc, &oRet), [](BOOL b)noexcept { return !b; });
        return oRet;
      }
    };
  }
}