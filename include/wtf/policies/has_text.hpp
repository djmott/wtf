/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
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
    template <typename _SuperT>
    struct has_text : _SuperT{

      virtual bool multiline() const{ return _multiline; }
      virtual void multiline(bool newval){ 
        _multiline = newval; 
        if (_auto_draw_text) _SuperT::invalidate();
      }

      bool word_wrap() const{ return _word_wrap; }
      void word_wrap(bool newval){ 
        _word_wrap = newval; 
        if (_auto_draw_text) _SuperT::invalidate();
      }

      virtual const tstring &text() const { return _text; }
      virtual tstring &text() { return _text; }
      virtual void text(const tstring &newval){
        _text = newval;
        if (_auto_draw_text) _SuperT::invalidate();
      }

      virtual text_vertical_alignments text_vertical_alignment() const{ return _text_vertical_alignment; }
      virtual void text_vertical_alignment(text_vertical_alignments newval){ 
        _text_vertical_alignment = newval; 
        if (_auto_draw_text) _SuperT::invalidate();
      }

      virtual text_horizontal_alignments text_horizontal_alignment() const{ return _text_horizontal_alignment; }
      virtual void text_horizontal_alignment(text_horizontal_alignments newval){ 
        _text_horizontal_alignment = newval; 
        if (_auto_draw_text) _SuperT::invalidate();
      }


      virtual size prefered_text_size() const{
        auto dc = _::device_context::get_client(*this);
        auto hFont = _SuperT::font().open();
        dc.select_object(hFont);
        return dc.get_text_extent(_text);
      }
    protected:

      explicit has_text(window * pParent) : _SuperT(pParent){}

      virtual bool auto_draw_text() const{ return _auto_draw_text; }
      virtual void auto_draw_text(bool newval){ _auto_draw_text = newval; }

      virtual void draw_text(const _::device_context& dc, const rect<coord_frame::client>& client, const tstring& str, int length){
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

        wtf::exception::throw_lasterr_if(::DrawText(dc, str.c_str(), length, &oClient, format),
                                         [](BOOL b){ return !b; });

      }

      virtual void draw_text(const _::device_context& dc, const rect<coord_frame::client>& client, const tstring& str) {
        draw_text(dc, client, str, static_cast<int>(str.size()));
      }

      virtual void draw_text(const _::device_context& dc, const rect<coord_frame::client>& client){
        draw_text(dc, client, text());
      }

      void on_wm_paint(const _::device_context& dc, const _::paint_struct& ps) override{
        if (_auto_draw_text) draw_text(dc, ps.client(), text());
        _SuperT::on_wm_paint(dc, ps);
      }


    private:

      tstring _text = _T("");
      text_vertical_alignments _text_vertical_alignment = text_vertical_alignments::center;
      text_horizontal_alignments _text_horizontal_alignment = text_horizontal_alignments::center;

      bool _multiline = false;
      bool _auto_draw_text = true;
      bool _word_wrap = false;

    };
  }

  namespace _{
    template <> struct policy_traits<policy::has_text>{
      using requires = policy_list<policy::has_font, policy::wm_paint>;
    };
  }
}