#pragma once

namespace wtf {
    /** has_icon
    * Provides an icon associated with the window
    */
    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::has_icon, _Policies...> 
      : public window_impl<_ImplT, _Policies..., policy::wm_geticon>
    {
      using __super_t = window_impl<_ImplT, _Policies..., policy::wm_geticon>;
      template <typename, policy ... > friend class window_impl;
    public:

      virtual icon &big_icon() { return _big_icon; }

      virtual const icon &big_icon() const { return _big_icon; }

      virtual void big_icon(icon &&src) { _big_icon = std::move(src); }

      virtual icon &small_icon() { return _small_icon; }

      virtual const icon &small_icon() const { return _small_icon; }

      virtual void small_icon(icon &&src) { _small_icon = std::move(src); }

    protected:
      explicit window(iwindow * pParent) : __super_t(pParent){}

      virtual HICON on_wm_geticon(icon_type ico) override{
        if (icon_type::big_icon == ico) return big_icon();
        else return small_icon();
      }

      icon _big_icon = icon::from_system(icon::system_icons::winlogo);
      icon _small_icon = icon::from_system(icon::system_icons::winlogo);
    };
  }
