#pragma once

namespace wtf{
  namespace policy{
    /** has_text
    * provides members to draw text on UI elements
    */
    template<typename _SuperT>
    struct has_text : _SuperT{


      has_text() : _SuperT(),
        _text(_T("")),
        _vertical_alignment(alignment::Center),
        _horizontal_alignment(alignment::Center),
        _multiline(false)
      {}
      virtual ~has_text() = default;
      has_text(const has_text&) = delete;
      has_text &operator=(const has_text &) = delete;
      has_text(has_text&&) = delete;
      has_text &operator=(has_text&&) = delete;


      enum class alignment{
        Near,
        Center,
        Far,
      };


      alignment vertial_alignment() const{ return _vertical_alignment; }

      void vertial_alignment(alignment newval){ _vertical_alignment = newval; }

      alignment horizontal_alignment() const{ return _horizontal_alignment; }

      void horizontal_alignment(alignment newval){ _horizontal_alignment = newval; }

      bool multiline() const{ return _multiline; }

      void multiline(bool newval){ _multiline = newval; }

      const tstring &text() const{ return _text; }

      void text(const tstring &newval){ _text = newval; }

    protected:

      virtual LRESULT handle_message(HWND , UINT umsg, WPARAM wparam, LPARAM , bool &) override{
        using text_align_modes = device_context::text_align_modes;
        using draw_text_flags = device_context::draw_text_flags;
        if (WM_PAINT == umsg){
          auto& oFont = this->template get_policy<policy::has_font>();
          auto &oDC = *reinterpret_cast<const device_context *>(wparam);
          auto oHandle = oFont.font().open();
          oDC.select_object(oHandle);
          oDC.move_to(0, 0);
          weak_enum_class <draw_text_flags> Flags(draw_text_flags::word_break);
          Flags |= draw_text_flags::word_break;
          switch (_vertical_alignment){
            case alignment::Near:
              Flags |= draw_text_flags::top;
              break;
            case alignment::Center:
              Flags |= draw_text_flags::vcenter;
              break;
            case alignment::Far:
              Flags |= draw_text_flags::bottom;
              break;
          }
          switch (_horizontal_alignment){
            case alignment::Near:
              Flags |= draw_text_flags::left;
              break;
            case alignment::Center:
              Flags |= draw_text_flags::center;
              break;
            case alignment::Far:
              Flags |= draw_text_flags::right;
              break;
          }
          if (!_multiline) Flags |= draw_text_flags::single_line;
          oDC.text_align(weak_enum_class<text_align_modes>::set_flags(text_align_modes::left, text_align_modes::top,
                         text_align_modes::no_update_cp));
          if (_SuperT::background_mix_modes::opaque == oFont.background_mix_mode()){
            oDC.background_mix_mode(device_context::background_mix_modes::opaque);
          } else{
            oDC.background_mix_mode(device_context::background_mix_modes::transparent);
          }
          oDC.text_color(oFont.fore_color());
          oDC.background_color(oFont.back_color());
          auto oClient = rect::get_client_rect(*this);
          oDC.draw_text(_text, oClient, Flags);

        }
        return 0;
      }

      tstring _text;
      alignment _vertical_alignment;
      alignment _horizontal_alignment;
      bool _multiline;
    };
  }
}
