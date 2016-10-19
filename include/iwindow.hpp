#pragma once

namespace wtf{
  struct iwindow{
    
    iwindow() = delete;
    iwindow(const iwindow&) = delete;
    iwindow(iwindow&&) = delete;
    iwindow& operator=(const iwindow&) = delete;
    iwindow operator=(iwindow&&) = delete;

    explicit iwindow(iwindow * Parent) : _parent(Parent){
      if (Parent) Parent->_children.push_back(this);
    }

/*
    iwindow& operator=(iwindow&& src){
      if (&src == this) return *this;
      std::swap(_parent, src._parent);
      std::swap(_children, src._children);
      std::swap(_handle, src._handle);
      return *this;
    }

    iwindow(iwindow&& src) : _parent(nullptr){
      *this = std::move(src);
    }
*/

    const iwindow * const parent() const{ return _parent; }
    
    const std::vector<iwindow*>& children() const{ return _children; }
    
    virtual const type_info& type() const = 0;

    HWND operator*() const { return _handle; }
    operator HWND() const { return _handle; }

  protected:
    template <typename, template <typename, typename> class ... > friend struct _::base_window;
    virtual void make_window() = 0;

    iwindow * _parent;
    std::vector<iwindow*> _children;
    HWND _handle;
  };
}
