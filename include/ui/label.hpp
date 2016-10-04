#pragma once
namespace wtf{

    struct label : window<label, policy::has_cursor, policy::has_paint, policy::has_size,
      policy::has_show, policy::has_text, policy::has_border, policy::has_click>
    {
      label(HWND hParent) : window(hParent, true){}
      virtual ~label() = default;

      label(const label&) = delete;
    };

}
