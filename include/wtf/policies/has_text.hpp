#pragma once

namespace wtf{

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

  namespace policy{
    /** has_text
    * provides members to draw text on UI elements
    */
    template<typename _SuperT, typename _ImplT>
    struct has_text : _SuperT::window_type::template add_policy<messages::wm_paint, policy::has_font> {


      virtual bool multiline() const{ return _multiline; }
      virtual void multiline(bool newval){ _multiline = newval; }

      bool word_wrap() const{ return _word_wrap; }
      void word_wrap(bool newval){ _word_wrap = newval; }

      virtual const tstring &text() const{ return _text; }
      virtual void text(const tstring &newval){
        _text = newval; 
        auto client = rect<coord_frame::client>::get(*this);
        if (_auto_draw_text) ::InvalidateRect(*this, &client,TRUE);
      }

      virtual text_vertical_alignments text_vertical_alignment() const{ return _text_vertical_alignment; }
      virtual void text_vertical_alignment(text_vertical_alignments newval){ _text_vertical_alignment = newval; }

      virtual text_horizontal_alignments text_horizontal_alignment() const{ return _text_horizontal_alignment; }
      virtual void text_horizontal_alignment(text_horizontal_alignments newval){ _text_horizontal_alignment = newval; }


      virtual size prefered_text_size() const{
        auto dc = device_context::get_client(*this);
        auto hFont = font().open();
        dc.select_object(hFont);
        return dc.get_text_extent(_text);
      }
    protected:
      using _super_t = typename _SuperT::window_type::template add_policy<messages::wm_paint, policy::has_font>;
      has_text(window<void> * pParent) : _super_t(pParent){}

      virtual bool auto_draw_text() const{ return _auto_draw_text; }
      virtual void auto_draw_text(bool newval){ _auto_draw_text = newval; }

      virtual void draw_text(const device_context& dc, const rect<coord_frame::client>& client){
        wtf::exception::throw_lasterr_if(::SetTextAlign(dc, TA_LEFT | TA_TOP | TA_NOUPDATECP),
                                         [](UINT i){ return GDI_ERROR == i; });

        UINT format = (multiline() ? 0 : DT_SINGLELINE) | (word_wrap() ? DT_WORDBREAK : 0);
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

        rect<coord_frame::client> oClient = client;

        wtf::exception::throw_lasterr_if(::DrawText(dc, _text.c_str(), -1, &oClient, format),
                                         [](BOOL b){ return !b; });

      }

      virtual LRESULT on_wm_paint(const device_context& dc, const paint_struct& ps, bool& bHandled) override{
        if (_auto_draw_text) draw_text(dc, ps.client());
        return _super_t::on_wm_paint(dc, ps, bHandled);
      }


    private:

      tstring _text = _T("");
      bool _multiline = false;
      bool _auto_draw_text = true;
      bool _word_wrap = false;

      text_vertical_alignments _text_vertical_alignment = text_vertical_alignments::center;
      text_horizontal_alignments _text_horizontal_alignment = text_horizontal_alignments::center;
    };
  }
}
