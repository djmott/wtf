#include <strstream>
#include <iostream>
#include <random>
#include "wtf.hpp"

using namespace wtf;

#if 0
struct main_form : form{
  main_form() : form(), _text(this){}

  virtual void CreateEvent() override{
    OnResized += [this](const point::client_coords& p){
      _text.move(0, 0, p.x-2, p.y-2);
    };
  }
  textbox _text;
};

#elif 1
struct main_form : form{
  main_form() : form(), _pb(this){}

  virtual void CreateEvent() override{
    _pb.move(10, 10, 100, 25);
    _pb.value(50);
  }
  progress_bar _pb;
};
#elif 0
struct main_form : form{
  main_form() : form(), _scroll(*this){
    _scroll.orientation(scroll_bar::orientations::horizontal);
    _scroll.move(10, 10, 100, 20);
    _scroll.min(0);
    _scroll.max(10);
    _scroll.value(5);
  }
  scroll_bar _scroll;
};
#else




struct label_page : panel{
  label_page(tab_container * parent) : panel(parent),
    _left(this), _right(this), _center(this),
    _raised(this), _lowered(this), _flat(this), 
    _etched(this), _bumped(this), _double_raised(this), _double_lowered(this)
  {
    _left.move(5, 5, 150, 25);
    _left.text("Left aligned");
    _left.text_horizontal_alignment(label::text_horizontal_alignments::left);

    _center.move(5, _left.top() + _left.height() + 5, 150, 25);
    _center.text("Center aligned");
    _center.text_horizontal_alignment(label::text_horizontal_alignments::center);

    _right.move(5, _center.top() + _center.height() + 5, 150, 25);
    _right.text("Right aligned");
    _right.text_horizontal_alignment(label::text_horizontal_alignments::right);

    _raised.move(5, _right.top() + _right.height() + 5, 150, 25);
    _raised.text("Raised");
    _raised.border_style(label::border_styles::raised);

    _lowered.move(5, _raised.top() + _raised.height() + 5, 150, 25);
    _lowered.text("Lowered");
    _lowered.border_style(label::border_styles::lowered);

    _flat.move(5, _lowered.top() + _lowered.height() + 5, 150, 25);
    _flat.text("Flat");
    _flat.border_style(label::border_styles::flat);

    _etched.move(5, _flat.top() + _flat.height() + 5, 150, 25);
    _etched.text("Etched");
    _etched.border_style(label::border_styles::etched);

    _bumped.move(5, _etched.top() + _etched.height() + 5, 150, 25);
    _bumped.text("Bumped");
    _bumped.border_style(label::border_styles::bumped);

    _double_raised.move(5, _bumped.top() + _bumped.height() + 5, 150, 25);
    _double_raised.text("Double raised");
    _double_raised.border_style(label::border_styles::double_raised);

    _double_lowered.move(5, _double_raised.top() + _double_raised.height() + 5, 150, 25);
    _double_lowered.text("Double lowered");
    _double_lowered.border_style(label::border_styles::double_lowered);

  }

  label _left, _center, _right;
  label _raised, _lowered, _flat;
  label _etched, _bumped, _double_raised, _double_lowered;
};


struct checkbox_page : panel{
  checkbox_page(tab_container * parent) : panel(parent), _left(this), _right(this){

    _left.move(5, 5, 150, 25);
    _left.text("Left aligned");
    _left.check_location(checkbox::check_locations::left);

    _right.move(5, 30, 150, 25);
    _right.text("Right aligned");
    _right.check_location(checkbox::check_locations::right);

  }


  checkbox _left, _right;
};


struct listbox_page : panel{
  listbox_page(tab_container * parent) : panel(parent),
    _left(this), _center(this), _right(this){
    for (int i = 0; i < 100; i++){
      tstringstream ss;
      ss << i;
      _left.add_item(ss.str());
      _center.add_item(ss.str());
      _right.add_item(ss.str());
    }
    _left.text_horizontal_alignment(listbox::text_horizontal_alignments::left);
    _center.text_horizontal_alignment(listbox::text_horizontal_alignments::center);
    _right.text_horizontal_alignment(listbox::text_horizontal_alignments::right);
    OnResized += [this](const point::client_coords& p){ 
      _left.move(0, 0, p.x/3, p.y);
      _center.move(p.x / 3, 0, p.x / 3, p.y);
      _right.move((p.x / 3) * 2, 0, p.x / 3, p.y);
    };
  }

  listbox _left, _center, _right;
};

struct button_page : panel{
  button_page(tab_container * parent) : panel(parent), 
    _button1(this), _button2(this), _label1(this), _label2(this)
  {
    _button1.move(5, 5, 100, 35);
    _button1.text("Push button");
    _label1.move(105, 5, 100, 35);
    _button1.OnClick += [this](){
      tstringstream ss;
      ss << GetTickCount();
      _label1.text(ss.str());
    };

    _button2.move(5, 50, 100, 35);
    _button2.text("Toggle button");
    _label2.move(105, 50, 100, 35);
    _button2.OnClick += [this](){
      tstringstream ss;
      ss << GetTickCount();
      _label2.text(ss.str());
    };
  }

  button _button1;
  toggle_button _button2;
  label _label1;
  label _label2;
};


struct scroll_page : panel{
  scroll_page(tab_container * parent) 
    : panel(parent), _hor_scroll(this), _vert_scroll(this), _hor_progress(this), _vert_progress(this)  
  {
    _hor_scroll.move(5, 5, 100, 20);
    _hor_scroll.orientation(scroll_bar::orientations::horizontal);

    _hor_progress.move(5, 30, 100, 20);
    _hor_progress.orientation(progress_bar::orientations::horizontal);

    _vert_scroll.move(120, 30, 20, 100);
    _vert_scroll.orientation(scroll_bar::orientations::vertical);

    _vert_progress.move(150, 30, 20, 100);
    _vert_progress.orientation(progress_bar::orientations::vertical);
  }

  scroll_bar _hor_scroll, _vert_scroll;
  progress_bar _hor_progress, _vert_progress;
};



struct split_page : panel{
  split_page(tab_container * parent) : panel(parent), _splitter(this){
    OnResized += [this](const point::client_coords& p){ _splitter.move(0, 0, p.x, p.y); };
    _splitter.set_split_position(50);
  }

  struct splitter : split_container{
    splitter(panel * parent) : split_container(parent), _inner_splitter(first()), _text1(second()){
      first()->OnResized += [this](const point::client_coords& p){ _inner_splitter.move(0, 0, p.x, p.y); };
      second()->OnResized += [this](const point::client_coords& p){ _text1.move(0, 0, p.x, p.y); };
      _inner_splitter.set_split_position(25);
    }

    struct inner_splitter : split_container{
      inner_splitter(panel * parent) : split_container(parent), _texta(first()), _textb(second()){
        orientation(inner_splitter::orientations::vertical);
        first()->OnResized += [this](const point::client_coords& p){ _texta.move(0, 0, p.x, p.y); };
        second()->OnResized += [this](const point::client_coords& p){ _textb.move(0, 0, p.x, p.y); };
        _texta.multiline(true);
      }
      textbox _texta, _textb;
    }_inner_splitter;

    textbox _text1;
  }_splitter;

};

struct tree_page : panel{
  tree_page(tab_container * parent) : panel(parent), _tree(this), rd(), gen(rd()), dist(1, 999999) {
    OnResized += [this](const point::client_coords& p){ _tree.move(0, 0, p.x, p.y); };
    for (int i = 0; i < 20; i++){
      auto oChild1 = _tree.add_node(RandomString());
      for (int x = 0; x < 20; x++){
        auto oChild2 = oChild1->add_node(RandomString());
        for (int j = 0; j < 20; j++){
          oChild2->add_node(RandomString());
        }
      }
    }

  }

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
  main_form() :
    _tabs(this){}

  virtual void CreateEvent(){
    titlebar("WTF example");
    _tabs.add_custom_page<label_page>("label");
    _tabs.add_custom_page<checkbox_page>("checkbox");
    _tabs.add_custom_page<listbox_page>("listbox");
    _tabs.add_custom_page<button_page>("buttons");
    _tabs.add_custom_page<scroll_page>("scroll_bar");
    _tabs.add_custom_page<split_page>("splitter");
    _tabs.add_custom_page<tree_page>("tree");

    OnResized += [this](const point::client_coords& p){ 
      _tabs.move(5, 5, p.x - 10, p.y - 10); 
    };
  }

  tab_container _tabs;
};
#endif

int main(){
  try{

/*
    form frm;
    scroll_bar s(frm);
    s.move(10, 10, 25, 100);
    s.orientation(scroll_bar::orientations::vertical);
    return frm.exec();
*/

    main_form oForm;
    return oForm.exec();
  }
  catch (const wtf::exception& ex){
    tstringstream ss;
    ss << ex.what() << std::endl << std::endl;
    ss << ex.file() << "(" << ex.line() << ")" << std::endl << std::endl;
    ss << ex.code();
    message_box::exec(nullptr, ss.str(), _T("An exception occurred."), message_box::buttons::ok, message_box::icons::stop);
    return -1;
  }
}
