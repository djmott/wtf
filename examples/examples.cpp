#include <strstream>
#include "wtf.hpp"

using namespace wtf;

#if 0
struct main_form : form{
  main_form() : form(), _tree(*this){
    for (int i = 0; i < 20; i++){
      tstring sTemp;
      auto s = std::to_string(i);
      std::copy(s.begin(), s.end(), std::back_inserter(sTemp));
      auto oNode = _tree.add_node(sTemp);
      oNode->expander_display_policy(tree::node::expander_display_policies::always);
      for (int x = 0; x < 20; x++){
        auto oChild = oNode->add_child(sTemp);
        for (int j = 0; j < 20; j++){
          oChild->add_child(sTemp);
        }
        oChild->expander_display_policy(tree::node::expander_display_policies::always);
        oChild->expanded(true);
      }
      oNode->expanded(true);
    }
    OnResized += [this](const point::client_coords& p){ _tree.move(0, 0, p.x, p.y); };
  }

  tree _tree;
};
#else


struct checkbox_page : panel{
  checkbox_page(tab_container& parent) : panel(parent), _left(*this), _right(*this){

    _left.move(5, 5, 150, 25);
    _left.text("Left aligned");
    _left.check_location(checkbox::check_locations::left);

    _right.move(5, 30, 150, 25);
    _right.text("Right aligned");
    _right.check_location(checkbox::check_locations::right);

  }


  checkbox _left, _right;
};

struct label_page : panel{
  label_page(tab_container& parent) : panel(parent),
    _left(*this), _right(*this), _center(*this),
    _raised(*this), _lowered(*this), _flat(*this){
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
    _raised.text("Raised border");
    _raised.border_style(label::border_styles::raised);

    _lowered.move(5, _raised.top() + _raised.height() + 5, 150, 25);
    _lowered.text("Lowered border");
    _lowered.border_style(label::border_styles::lowered);

    _flat.move(5, _lowered.top() + _lowered.height() + 5, 150, 25);
    _flat.text("Flat border");
    _flat.border_style(label::border_styles::flat);

  }

  label _left, _center, _right, _raised, _lowered, _flat;
};

struct listbox_page : panel{
  listbox_page(tab_container& parent) : panel(parent),
    _left(*this), _center(*this), _right(*this){
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
  button_page(tab_container& parent) : panel(parent), 
    _button1(*this), _button2(*this), _label1(*this), _label2(*this)
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

  push_button _button1;
  toggle_button _button2;
  label _label1;
  label _label2;
};

struct scroll_page : panel{
  scroll_page(tab_container& parent) : panel(parent), _hor(*this), _vert(*this) {
    _hor.move(5, 5, 100, 20);
    _hor.orientation(scroll_bar::orientations::horizontal);
    _vert.move(20, 30, 20, 100);
    _vert.orientation(scroll_bar::orientations::vertical);
  }

  scroll_bar _hor, _vert;
};

struct split_page : panel{
  split_page(tab_container& parent) : panel(parent), _splitter(*this){
    OnResized += [this](const point::client_coords& p){ _splitter.move(0, 0, p.x, p.y); };
  }

  struct splitter : split_container{
    splitter(panel& parent) : split_container(parent), _text1(first()), _text2(second()){
      first().OnResized += [this](const point::client_coords& p){ _text1.move(0, 0, p.x, p.y); };
      second().OnResized += [this](const point::client_coords& p){ _text2.move(0, 0, p.x, p.y); };
    }


    textbox _text1, _text2;
  }_splitter;

};

struct tree_page : panel{
  tree_page(tab_container& parent) : panel(parent), _tree(*this){
    OnResized += [this](const point::client_coords& p){ _tree.move(0, 0, p.x, p.y); };
    for (int i = 0; i < 100; i++){

      tstring sTemp;
      auto s = std::to_string(i);
      std::copy(s.begin(), s.end(), std::back_inserter(sTemp));
      _tree.add_node(sTemp);
    }

  }

  tree _tree;
};

struct main_form : form{
  main_form() :
    _tabs(*this)
  {
    titlebar("WTF example");
    _tabs.add_custom_page<checkbox_page>("checkbox");
    _tabs.add_custom_page<label_page>("label");
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
