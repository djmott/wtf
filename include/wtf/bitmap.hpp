/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

  struct bitmap : std::shared_ptr<HBITMAP__> {
    enum class system_bitmaps {
      close = OBM_CLOSE,
      uparrow = OBM_UPARROW,
      dnarrow = OBM_DNARROW,
      rgarrow = OBM_RGARROW,
      lfarrow = OBM_LFARROW,
      reduce = OBM_REDUCE,
      zoom = OBM_ZOOM,
      restore = OBM_RESTORE,
      reduced = OBM_REDUCED,
      zoomd = OBM_ZOOMD,
      restored = OBM_RESTORED,
      uparrowd = OBM_UPARROWD,
      dnarrowd = OBM_DNARROWD,
      rgarrowd = OBM_RGARROWD,
      lfarrowd = OBM_LFARROWD,
      mnarrow = OBM_MNARROW,
      combo = OBM_COMBO,
      uparrowi = OBM_UPARROWI,
      dnarrowi = OBM_DNARROWI,
      rgarrowi = OBM_RGARROWI,
      lfarrowi = OBM_LFARROWI,
      old_close = OBM_OLD_CLOSE,
      size = OBM_SIZE,
      old_uparrow = OBM_OLD_UPARROW,
      old_dnarrow = OBM_OLD_DNARROW,
      old_rgarrow = OBM_OLD_RGARROW,
      old_lfarrow = OBM_OLD_LFARROW,
      btsize = OBM_BTSIZE,
      check = OBM_CHECK,
      checkboxes = OBM_CHECKBOXES,
      btncorners = OBM_BTNCORNERS,
      old_reduce = OBM_OLD_REDUCE,
      old_zoom = OBM_OLD_ZOOM,
      old_restore = OBM_OLD_RESTORE,

    };

    static bitmap from_system(system_bitmaps Style) {
      return bitmap(
        wtf::exception::throw_lasterr_if(::LoadBitmap(nullptr, MAKEINTRESOURCE(Style)), [](HBITMAP h) noexcept { return !h; }),
        [](HBITMAP) noexcept {});
    }

    static bitmap from_resource(LPCTSTR ResourceName) {
      return bitmap(
        wtf::exception::throw_lasterr_if(::LoadBitmap(instance_handle(), ResourceName), [](HBITMAP h) noexcept { return !h; }),
        [](HBITMAP)noexcept {});
    }
    static bitmap from_resource(int ResourceID) {
      return bitmap(
        wtf::exception::throw_lasterr_if(::LoadBitmap(instance_handle(), MAKEINTRESOURCE(ResourceID)), [](HBITMAP h) noexcept { return !h; }),
        [](HBITMAP)noexcept {});
    }

    bitmap() = delete;
    ~bitmap() = default;

    bitmap(const bitmap&) = delete;
    bitmap &operator=(const bitmap &) = delete;

    bitmap(bitmap &&src) : shared_ptr(std::move(src)) {}

    bitmap &operator=(bitmap &&src) noexcept {
      shared_ptr::swap(src);
      return *this;
    }

    operator HBITMAP() const noexcept { return get(); }

  protected:
    template<typename ... _ArgTs>
    bitmap(_ArgTs &&...oArgs) : shared_ptr(std::forward<_ArgTs>(oArgs)...) {}
  };
}

