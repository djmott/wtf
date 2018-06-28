/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

  struct monitor : MONITORINFOEX{
    using vector = std::vector<monitor>;
    ~monitor() = default;
    monitor() = delete;
    monitor(const monitor&) = default;
    monitor& operator=(const monitor&) = default;
    monitor(HMONITOR hMonitor) : _handle(hMonitor) {
      memset(this, 0, sizeof(MONITORINFOEX));
      cbSize = sizeof(MONITORINFOEX);
      wtf::exception::throw_lasterr_if(::GetMonitorInfo(_handle, this), [](BOOL b) {return !b; });
    }
    monitor(const window& wnd) 
      : monitor(wtf::exception::throw_lasterr_if(::MonitorFromWindow(*wnd, MONITOR_DEFAULTTONEAREST), [](HMONITOR h) { return !h; }))
    {}
    
    constexpr bool is_primary() const noexcept { return dwFlags & MONITORINFOF_PRIMARY;}

    constexpr uint32_t width() const noexcept { return rcWork.right - rcWork.left; }
    constexpr uint32_t height() const noexcept { return rcWork.bottom - rcWork.top; }

    static vector get_all() {
      vector oRet;
      wtf::exception::throw_lasterr_if(::EnumDisplayMonitors(nullptr, nullptr, monitor_enum_proc, reinterpret_cast<LPARAM>(&oRet)), [](BOOL b) {return !b; });
      return oRet;
    }

  private:
    HMONITOR _handle = nullptr;
    static BOOL CALLBACK monitor_enum_proc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
      auto oRet = reinterpret_cast<vector*>(dwData);
      oRet->emplace_back(hMonitor);
      return TRUE;
    }

  };

}

