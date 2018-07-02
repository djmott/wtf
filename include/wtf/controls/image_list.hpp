/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once



namespace wtf {

  namespace controls {
    struct image_list : window {

      enum class styles {
        color = ILC_COLOR, //!< Use the default behavior if none of the other ILC_COLOR* flags is specified.Typically, the default is ILC_COLOR4, but for older display drivers, the default is ILC_COLORDDB.
        color4 = ILC_COLOR4,//!< Use a 4 - bit(16 - color) device - independent bitmap(DIB) section as the bitmap for the image list.
        color8 = ILC_COLOR8,//!< Use an 8 - bit DIB section.The colors used for the color table are the same colors as the halftone palette.
        color16 = ILC_COLOR16,//!< Use a 16 - bit(32 / 64k - color) DIB section.
        color24 = ILC_COLOR24,//!< Use a 24 - bit DIB section.
        color32 = ILC_COLOR32,//!< Use a 32 - bit DIB section.
        colorddb = ILC_COLORDDB,//!< Use a device - dependent bitmap.
        mask = ILC_MASK,//!< Use a mask.The image list contains two bitmaps, one of which is a monochrome bitmap used as a mask.If this value is not included, the image list contains only one bitmap.
        mirror = ILC_MIRROR,//!< Version 6.00.Microsoft® Windows® can be mirrored to display languages such as Hebrew or Arabic that read right - to - left.If the image list is created on a mirrored version of Windows, then the images in the lists are mirrored, that is, they are flipped so they display from right to left.Use this flag on a mirrored version of Windows to instruct the image list not to automatically mirror images.
        perite_mirror = ILC_PERITEMMIRROR,//!< Version 6.00.Specify this flag if ILC_MIRROR is used on an image list that contains a strip of images.In order for this flag to have any effect, ILC_MIRROR must also be specified.
      };

      styles style() const { return _style; }
      void style(styles newval) { _style = newval; }

    protected:
      int run() override {
        //_himagelist = ImageList_Create()
      }
      HIMAGELIST _himagelist;
      styles _style;
    };
  }

}