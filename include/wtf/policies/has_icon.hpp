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

      virtual icon &big_icon(){ return _big_icon; }

      virtual const icon &big_icon() const{ return _big_icon; }

      virtual void big_icon(icon &&src){ _big_icon = std::move(src); }

      virtual icon &small_icon(){ return _small_icon; }

      virtual const icon &small_icon() const{ return _small_icon; }

      virtual void small_icon(icon &&src){ _small_icon = std::move(src); }

    protected:
      explicit has_icon(iwindow * pParent) : _SuperT(pParent){}

      HICON on_wm_geticon(icon_type ico) override{
        if (icon_type::big_icon == ico) return big_icon();
        else return small_icon();
      }

      icon _big_icon = icon::from_system(icon::system_icons::winlogo);
      icon _small_icon = icon::from_system(icon::system_icons::winlogo);
    };
  }

  template <> struct policy_traits<policy::has_icon>{
    using requires = policy_list<policy::wm_geticon>;
  };

}