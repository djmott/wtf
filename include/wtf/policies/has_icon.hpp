/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{

    /** has_icon
    * Provides an icon associated with the window
    */
    template <typename _SuperT>
    struct has_icon : _SuperT{

      virtual icon &big_icon()noexcept { return _big_icon; }

      virtual const icon &big_icon() const noexcept { return _big_icon; }

      virtual void big_icon(icon &&src)noexcept { _big_icon = std::move(src); }

      virtual icon &small_icon()noexcept { return _small_icon; }

      virtual const icon &small_icon() const noexcept { return _small_icon; }

      virtual void small_icon(icon &&src)noexcept { _small_icon = std::move(src); }

    protected:
      explicit has_icon(window * pParent)  : _SuperT(pParent){}

      HICON on_wm_geticon(icon_type ico) noexcept override {
        if (icon_type::big_icon == ico) return big_icon();
        else return small_icon();
      }

      icon _big_icon = icon::from_system(icon::system_icons::winlogo);
      icon _small_icon = icon::from_system(icon::system_icons::winlogo);
    };
  }

  namespace _{
    template <> struct policy_traits<policy::has_icon>{
      using requires = policy_list<policy::wm_geticon>;
    };
  }
}