#pragma once
/**
  @file
  Defines the interface to retrieve keycodes
  Copyright (C) 2006, Navman New Zealand. All rights reserved.
*/

#ifndef _KEYMAP_H_
#define _KEYMAP_H_

#include "stdafx.h"


namespace navman {


class KeyMap
{
public:
  /**
    Defines the list of valid keys
  */
  enum Keys
  {
    Unknown,    // Unknown key, keycode = 0
    VolumeUp,   // Physical keys on device
    VolumeDown,
    Mute,
    Fuel,
    Parking,
    Menu,
    Go,
    Map,
    Camera,
    Cancel,     // Physical keys on remote
    Home,
    BrightUp,
    BrightDown,
    Space,      // Virtual keys
    Shift,
    Numeric,
    Symbol,
    Alpha,
    Enter,
    Escape,
    Back,
    Up,
    Down,
    Left,
    Right,
    Page,
    ZoomIn,
    ZoomOut,
    Capture,
    // ONLY FOR PC
    BtnL1,
    BtnL2,
    BtnL3,
    BtnL4,
    BtnR1,
    BtnR2,
    BtnR3,
    BtnR4,
    MAX_KEYS
  };

  /**
    Gets the keycode of a given key
    @param key Specifies the key of interest
    @return matching keycode
  */
  static WCHAR getKeycode(Keys key);

  /**
    Gets the key of a given keycode
    @param keycode Specifies the keycode of interest
    @return matching key
  */
  static Keys getKey(WCHAR keycode);

  /**
  Set KeyValue To array g_KeyMap by KeyName
  @param wcKeyName specifies the name of KEY
  @param keyValue specifies the value of KEY in string for example "0x01"
  @return
 */
  static void SetKeyValue(Keys key, int cKeyValue);

private:
  static WCHAR _keyMap[MAX_KEYS];
};


}

#endif // _KEYMAP_H_
