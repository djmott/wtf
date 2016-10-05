#pragma once

namespace wtf{
  namespace policy{
    /** has_text
    * provides members to draw text on UI elements
    */
    template<typename _SuperT>
    struct has_text : _SuperT{

      has_text()
        : _SuperT(),
        _text(_T("")),
        _multiline(false),
        _text_vertical_alignment(text_vertical_alignments::center),
        _text_horizontal_alignment(text_horizontal_alignments::center){}

      virtual ~has_text() = default;
      has_text(const has_text&) = delete;
      has_text &operator=(const has_text &) = delete;
      has_text(has_text&&) = delete;
      has_text &operator=(has_text&&) = delete;

      enum class text_vertical_alignments{
        top,
        center,
        bottom,
      };

      enum class text_horizontal_alignments{
        left,
        center,
        right,
      };


      virtual bool multiline() const{ return _multiline; }
      virtual void multiline(bool newval){ _multiline = newval; }

      virtual const tstring &text() const{ return _text; }
      virtual void text(const tstring &newval){ _text = newval; }

      virtual text_vertical_alignments text_vertical_alignment() const{ return _text_vertical_alignment; }
      virtual void text_vertical_alignment(text_vertical_alignments newval){ _text_vertical_alignment = newval; }

      virtual text_horizontal_alignments text_horizontal_alignment() const{ return _text_horizontal_alignment; }
      virtual void text_horizontal_alignment(text_horizontal_alignments newval){ _text_horizontal_alignment = newval; }


    protected:

      virtual void DrawText(const device_context& dc, rect& client){
        ApplyFontEvent(dc);
        wtf::exception::throw_lasterr_if(::SetTextAlign(dc, TA_LEFT | TA_TOP | TA_NOUPDATECP),
                                         [](UINT i){ return GDI_ERROR == i; });

        UINT format = DT_WORDBREAK | (multiline() ? 0 : DT_SINGLELINE);
        switch (text_vertical_alignment()){
          case text_vertical_alignments::top:
            format |= DT_TOP; break;
          case text_vertical_alignments::center:
            format |= DT_VCENTER; break;
          default:
            format |= DT_BOTTOM; break;
        }
        switch (text_horizontal_alignment()){
          case text_horizontal_alignments::left:
            format |= DT_LEFT; break;
          case text_horizontal_alignments::center:
            format |= DT_CENTER; break;
          default:
            format |= DT_RIGHT; break;
        }
        wtf::exception::throw_lasterr_if(::DrawText(dc, _text.c_str(), -1, &client, format),
                                         [](BOOL b){ return !b; });

      }

    private:
      tstring _text;
      bool _multiline;
      text_vertical_alignments _text_vertical_alignment;
      text_horizontal_alignments _text_horizontal_alignment;
    };
  }
}
