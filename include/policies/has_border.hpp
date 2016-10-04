#pragma once

namespace wtf {

  namespace policy {
    /** has_border
    * Creates borders
    */
    template<typename _SuperT>
    struct has_border : _SuperT {

      using border_edges = device_context::border_edges;
      using border_flags = device_context::border_flags;

      virtual ~has_border() = default;

      has_border() : _SuperT(), _border_edge(border_edges::raised),
                     _border_flag(
                       weak_enum_class<border_flags>::set_flags(border_flags::adjust, border_flags::whole_rect,
                                                                border_flags::soft, border_flags::flat,
                                                                border_flags::mono)) {}

      has_border(const has_border&) = delete;
      has_border(has_border&&) = delete;
      has_border &operator=(const has_border &) = delete;
      has_border &operator=(has_border&&) = delete;

      border_edges border_edge() const { return _border_edge; }

      void border_edge(border_edges newval) { _border_edge = newval; }

      border_flags border_flag() const { return _border_flag; }

      void border_flag(border_flags newval) { _border_flag = newval; }

    protected:
      virtual LRESULT handle_message(HWND , UINT umsg, WPARAM wparam, LPARAM, bool &) override {
        if (WM_PAINT == umsg && border_edges::none != _border_edge) {
          auto &oDC = *reinterpret_cast<const device_context *>(wparam);
          oDC.draw_edge(rect::get_client_rect(*this), _border_edge, _border_flag);
        }
        return 0;
      }

    private:
      border_edges _border_edge;
      border_flags _border_flag;
    };

  }
}
