#pragma once
namespace wtf{

    struct label : window<label, policy::has_cursor, policy::has_paint, policy::has_size,
      policy::has_show, policy::has_text, policy::has_border, policy::has_click, policy::has_zorder>
    {
      label() = delete;
      label(const label&) = delete;
      label(label&&) = delete;
      label &operator=(const label &) = delete;
      label &operator=(label&&) = delete;
      virtual ~label() = default;
      explicit label(HWND hParent) : window(hParent, true)
      {
        this->border_style(border_styles::none);
      }
      callback<void()> OnClick;
    protected:
      virtual void ClickEvent(const point::client_coords&){ OnClick(); }


    };

}
