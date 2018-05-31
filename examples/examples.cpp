/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/

#define __WTF_DEBUG_MESSAGES__ 1

#include <strstream>
#include <iostream>
#include <random>
#include "wtf/wtf.hpp"
using namespace wtf;

#if 1

struct main_form : form {
  main_form(){}
  void on_wm_paint(const wtf::_::device_context& dc, const wtf::_::paint_struct& ps) override {
    tstring sTemp = "this\nis\na\ntest";
    RECT rc = ps.rcPaint;
    ::DrawText(dc, sTemp.c_str(), -1, &rc, DT_LEFT | DT_TOP);
  }
};

#elif 0

struct main_form : form {

  main_form() : _scroll_panel(this) {
    OnCreate += [this](window*) {
      _scroll_panel.move(5, 5, 100, 200);
    };
  }

  vscroll_panel _scroll_panel;

};

#elif 0
struct main_form : form{

  main_form() : _scroll_panel(this){
    OnCreate += [this](window*){
      _scroll_panel.move(5, 5, 100, 200);
    };
  }

  scroll_panel _scroll_panel;

};
#elif 0
struct main_form : form{

  main_form() : _listbox(this){
    OnCreate += [this](window*){
      _listbox.move(5, 5, 100, 200);
//       for (int i = 0; i < 1000; ++i){
//         _listbox.add_item(wtf::to_tstring(i));
//       }
      _listbox.add_items({ _T("1"), _T("2"), _T("3"), _T("4"), _T("5") });
    };
  }

  listbox _listbox;

};
#elif 0
struct main_form : form{

  main_form() : _scroll(this){
    OnCreate += [this](window*){
      _scroll.orientation(orientations::vertical);
      _scroll.move(5, 5, 20, 200); };
  }

  scroll_bar _scroll;

};
#elif 0

struct main_form : form{

  main_form() : _scroll(this){

    OnCreate += [this](window*){

      _scroll.orientation(orientations::horizontal);
      _scroll.move(10, 10, 200, 20);
      
    };

  }

  scroll_bar _scroll;

};

#elif 0
struct main_form : form{

  main_form() : _button(this), _menu(this){
    OnCreate += [this](window*){ _menu.move(10, 10, 100, 100); };
    _button.OnClick += [this](const mouse_msg_param& p){ _menu.popup( left(), top()); };
  }
  button _button;
  menu _menu;
};
#elif 0
struct main_form : form{

  main_form() : _text(this){
    OnCreate += [this](window*){ _text.move(10, 10, 100, 50); };
  }
  textbox _text;
};

#elif 0
struct main_form : form{

  main_form() : _split(this){
    OnSize += [this](window *, const point<coord_frame::client>& oSize){ _split.move(5, 5, oSize.x - 10, oSize.y - 10); };
  }

  split_container _split;
};


#else



struct label_page : tab_page{
  label_page(window * parent) : tab_page(parent),
    _left(this), _right(this), _center(this),
    _raised(this), _lowered(this), _flat(this), 
    _etched(this), _bumped(this), _double_raised(this), _double_lowered(this)
  {
    _raised.border_style(border_styles::raised);
    _lowered.border_style(border_styles::lowered);
    _flat.border_style(border_styles::flat);
    _etched.border_style(border_styles::etched);
    _bumped.border_style(border_styles::bumped);
    _double_raised.border_style(border_styles::double_raised);
    _double_lowered.border_style(border_styles::double_lowered);

  }
  void on_wm_create() override {
    _left.move(5, 5, 150, 25);
    _left.text("Left aligned");
    _left.text_horizontal_alignment(text_horizontal_alignments::left);

    _center.move(5, _left.top() + _left.height() + 5, 150, 25);
    _center.text("Center aligned");
    _center.text_horizontal_alignment(text_horizontal_alignments::center);

    _right.move(5, _center.top() + _center.height() + 5, 150, 25);
    _right.text("Right aligned");
    _right.text_horizontal_alignment(text_horizontal_alignments::right);

    _raised.move(5, _right.top() + _right.height() + 5, 150, 25);
    _raised.text("Raised");
    _raised.border_style(border_styles::raised);

    _lowered.move(5, _raised.top() + _raised.height() + 5, 150, 25);
    _lowered.text("Lowered");
    _lowered.border_style(border_styles::lowered);

    _flat.move(5, _lowered.top() + _lowered.height() + 5, 150, 25);
    _flat.text("Flat");
    _flat.border_style(border_styles::flat);

    _etched.move(5, _flat.top() + _flat.height() + 5, 150, 25);
    _etched.text("Etched");
    _etched.border_style(border_styles::etched);

    _bumped.move(5, _etched.top() + _etched.height() + 5, 150, 25);
    _bumped.text("Bumped");
    _bumped.border_style(border_styles::bumped);

    _double_raised.move(5, _bumped.top() + _bumped.height() + 5, 150, 25);
    _double_raised.text("Double raised");
    _double_raised.border_style(border_styles::double_raised);

    _double_lowered.move(5, _double_raised.top() + _double_raised.height() + 5, 150, 25);
    _double_lowered.text("Double lowered");
    _double_lowered.border_style(border_styles::double_lowered);
    return tab_page::on_wm_create();
  };

  label _left, _center, _right;
  label _raised, _lowered, _flat;
  label _etched, _bumped, _double_raised, _double_lowered;

};


struct checkbox_page : tab_page{
  checkbox_page(window * parent) : tab_page(parent), _left(this), _right(this){}

  void on_wm_create() override {
    _left.move(5, 5, 150, 25);
    _left.text("Left aligned");
    _left.text_horizontal_alignment(text_horizontal_alignments::left);

    _right.move(5, 30, 150, 25);
    _right.text("Right aligned");
    _right.text_horizontal_alignment(text_horizontal_alignments::right);
      return tab_page::on_wm_create();
  };

  checkbox _left, _right;
};


struct listbox_page : tab_page{
  listbox_page(window * parent) : tab_page(parent),
    _left(this), _center(this), _right(this)
  {

  }
  void on_wm_create() override {
    for (int i = 0; i < 100; i++){
      auto sTemp = to_tstring(i);
      _left.add_item(sTemp);
      _center.add_item(sTemp);
      _right.add_item(sTemp);
    }
//     _left.text_horizontal_alignment(text_horizontal_alignments::left);
//     _center.text_horizontal_alignment(text_horizontal_alignments::center);
//     _right.text_horizontal_alignment(text_horizontal_alignments::right);
    return tab_page::on_wm_create();

  };
  void on_wm_size(const point<coord_frame::client>& p) override {
    _left.move(0, 0, p.x / 3, p.y);
    _center.move(p.x / 3, 0, p.x / 3, p.y);
    _right.move((p.x / 3) * 2, 0, p.x / 3, p.y);
    return tab_page::on_wm_size(p);
  };

  listbox _left, _center, _right;
};

struct button_page : tab_page{
  button_page(window * parent) : tab_page(parent),
    _label1(this), _label2(this), _button1(this, _label1), _button2(this, _label2){}

  void on_wm_create() override {
    _button1.move(5, 5, 100, 35);
    _button1.text("Push button");
    _label1.move(105, 5, 100, 35);
    _button2.move(5, 50, 100, 35);
    _button2.text("Toggle button");
    _label2.move(105, 50, 100, 35);
    return tab_page::on_wm_create();
  };

  label _label1;
  label _label2;

  struct _button1 : button{
    _button1(window * pParent, label& oLabel) : button(pParent), _label1(oLabel){}

    void on_wm_click(const mouse_msg_param<coord_frame::client>& m) override{
      _label1.text(to_tstring(GetTickCount()));
      button::on_wm_click(m);
    };
    label& _label1;
  }_button1;

  struct _button2 : toggle_button{
    _button2(window * pParent, label& oLabel) : toggle_button(pParent), _label2(oLabel){}

    void on_wm_click(const mouse_msg_param<coord_frame::client>& m) override{
      _label2.text(to_tstring(GetTickCount()));
      toggle_button::on_wm_click(m);
    };

    label& _label2;
  }_button2;

};


struct scroll_page : tab_page{
  scroll_page(window * parent)
    : tab_page(parent), _hor_scroll(this), _vert_scroll(this), _hor_progress(this), _vert_progress(this)
  {}

  void on_wm_create() override {
    _hor_scroll.move(5, 5, 100, 20);
    _hor_scroll.orientation(orientations::horizontal);

    _hor_progress.move(5, 30, 100, 20);
    _hor_progress.orientation(orientations::horizontal);

    _vert_scroll.move(120, 5, 20, 100);
    _vert_scroll.orientation(orientations::vertical);

    _vert_progress.move(150, 5, 20, 100);
    _vert_progress.orientation(orientations::vertical);
    return tab_page::on_wm_create();
  };

  scroll_bar _hor_scroll, _vert_scroll;
  progress_bar _hor_progress, _vert_progress;
};



struct split_page : tab_page{

  split_page(window * parent) : tab_page(parent), _splitter(this){
    OnSize += [this](window *,const point<coord_frame::client>& p){ _splitter.move(0, 0, p.x, p.y); };
  }

  struct splitter : split_container{

    splitter(split_page * parent) : split_container(parent), _inner_splitter(&first()), _text1(&second()){
      OnCreate += [this](window *){ orientation(orientations::horizontal); };
      OnSize += [this](window *, const point<coord_frame::client>& p){
        if (!_InitialPosition && p.x && p.y){
          _InitialPosition = true;
          set_split_relative(25);
        }
      };
      first().OnSize += [this](window *,const point<coord_frame::client>& p){_inner_splitter.move(0, 0, p.x, p.y); };
      second().OnSize += [this](window *,const point<coord_frame::client>& p){_text1.move(0, 0, p.x, p.y); };
    }

    struct inner_splitter : split_container{

      inner_splitter(panel * parent) : split_container(parent), _texta(&first()), _textb(&second()){
        OnCreate += [this](window*){ orientation(orientations::vertical); };
        OnSize += [this](window*,const point<coord_frame::client>& p){
          if (!_InitialPosition && p.x && p.y){
            _InitialPosition = true;
            set_split_relative(25);
          }
        };
        first().OnSize += [this](window*,const point<coord_frame::client>& p){_texta.move(0, 0, p.x, p.y); };
        second().OnSize += [this](window*,const point<coord_frame::client>& p){_textb.move(0, 0, p.x, p.y); };
      }

      bool _InitialPosition = false;
      textbox _texta, _textb;
    }_inner_splitter;

    bool _InitialPosition = false;
    textbox _text1;
  }_splitter;

};

struct tree_page : tab_page{

  tree_page(window * parent) : tab_page(parent), _tree(this), rd(), gen(rd()), dist(1, 999999) {

  }

  void on_wm_size(const point<coord_frame::client>& p) override{
    _tree.move(0, 0, p.x, p.y); 
    return tab_page::on_wm_size(p);
  }

  void on_wm_create() override{
    for (int i = 0; i < 20; i++){
      auto oChild1 = _tree.add_item(RandomString());
      for (int x = 0; x < 20; x++){
        auto oChild2 = oChild1->add_item(RandomString());
        for (int j = 0; j < 20; j++){
          oChild2->add_item(RandomString());
        }
      }
    }
    return tab_page::on_wm_create();
  };

  tstring RandomString(){
    tstring sTemp;
    auto s = std::to_string(dist(gen));
    std::copy(s.begin(), s.end(), std::back_inserter(sTemp));
    return sTemp;
  }

  tree _tree;

  std::random_device rd;
  std::mt19937 gen;
  std::uniform_int_distribution<int> dist;

};

struct main_form : form{
  main_form() : _tabs(this){}

  void on_wm_create() override{
    titlebar("WTF example");
    _tabs.add_custom<label_page>("label");
    _tabs.add_custom<checkbox_page>("checkbox");
    _tabs.add_custom<listbox_page>("listbox");
    _tabs.add_custom<button_page>("buttons");
    _tabs.add_custom<scroll_page>("scroll_bar");
    _tabs.add_custom<split_page>("splitter");
    _tabs.add_custom<tree_page>("tree");
    return form::on_wm_create();
  };

  void on_wm_size(const point<coord_frame::client>& p) override{
    _tabs.move(5, 5, p.x - 10, p.y - 10);
    return form::on_wm_size(p);
  };

  tab_container _tabs;
};

#endif

int main(){
  try{
    return main_form().exec();
  }
  catch (const wtf::exception& ex){
    std::cerr << ex.what() << std::endl << std::endl;
    std::cerr << ex.file() << "(" << ex.line() << ")" << std::endl << std::endl;
    std::cerr << ex.code();
    return -1;
  }
}
