/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

  struct monitor : MONITORINFOEX{
    monitor() = delete;
    ~monitor() = default;
    monitor(const monitor&) = default;
    monitor& operator=(const monitor&) = default;
    monitor(const window& wnd) :
      _handle(wtf::exception::throw_lasterr_if(::MonitorFromWindow(*wnd, MONITOR_DEFAULTTONEAREST), [](HMONITOR h) { return !h};)){}
  private:
    HMONITOR _handle;
  };

}

