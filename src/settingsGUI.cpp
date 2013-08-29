#include <fxcg/display.h>
#include <fxcg/file.h>
#include <fxcg/keyboard.h>
#include <fxcg/system.h>
#include <fxcg/misc.h>
#include <fxcg/app.h>
#include <fxcg/serial.h>
#include <fxcg/rtc.h>
#include <fxcg/heap.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "settingsGUI.hpp"
#include "menuGUI.hpp"
#include "selectorGUI.hpp"
#include "aboutGUI.hpp"
#include "settingsProvider.hpp"
#include "keyboardProvider.hpp"
#include "graphicsProvider.hpp"
#include "timeProvider.hpp" 
#include "timeGUI.hpp"
#include "lockGUI.hpp"

void settingsMenu() {
  MenuItem menuitems[13];
  strcpy(menuitems[0].text, "Set time");
  
  strcpy(menuitems[1].text, "Set date");
  
  strcpy(menuitems[2].text, "Time format");
  
  strcpy(menuitems[3].text, "Long date format");
  
  strcpy(menuitems[4].text, "Date format");
  
  strcpy(menuitems[5].text, "Dark theme");
  menuitems[5].type = MENUITEM_CHECKBOX;
  
  strcpy(menuitems[6].text, "Display statusbar");
  menuitems[6].type = MENUITEM_CHECKBOX;
  
  strcpy(menuitems[7].text, "Show advanced tools");
  menuitems[7].type = MENUITEM_CHECKBOX;
  
  strcpy(menuitems[8].text, "Show F. keys labels");
  menuitems[8].type = MENUITEM_CHECKBOX;
  
  strcpy(menuitems[9].text, "Startup brightness");
  
  strcpy(menuitems[10].text, "Calc. lock settings");
  
  strcpy(menuitems[11].text, "Show events count");
  menuitems[11].type = MENUITEM_CHECKBOX;
  
  strcpy(menuitems[12].text, "About this add-in");
  
  Menu menu;
  menu.items=menuitems;
  menu.numitems=13;
  menu.scrollbar=1;
  menu.scrollout=1;
  menu.showtitle=0;
  menu.selection=1;
  menu.scroll=0;
  menu.allowMkey=0;
  strcpy(menu.nodatamsg, "");
  strcpy(menu.title, "");
  strcpy(menu.statusText, "");
  while(1) {
    menuitems[5].value = GetSetting(SETTING_THEME);
    menuitems[6].value = GetSetting(SETTING_DISPLAY_STATUSBAR);
    menuitems[7].value = GetSetting(SETTING_SHOW_ADVANCED);
    menuitems[8].value = GetSetting(SETTING_DISPLAY_FKEYS);
    menuitems[11].value = GetSetting(SETTING_SHOW_CALENDAR_EVENTS_COUNT);
    
    int res = doMenu(&menu);
    if(res == MENU_RETURN_EXIT) return;
    else if(res == MENU_RETURN_SELECTION) {
      // deal with checkboxes first
      if(menu.selection == 6) {
        if(menuitems[5].value == MENUITEM_VALUE_CHECKED) menuitems[5].value=MENUITEM_VALUE_NONE;
        else menuitems[5].value=MENUITEM_VALUE_CHECKED;
        SetSetting(SETTING_THEME, menuitems[5].value, 1); 
      }
      if(menu.selection == 7) {
        if(menuitems[6].value == MENUITEM_VALUE_CHECKED) menuitems[6].value=MENUITEM_VALUE_NONE;
        else menuitems[6].value=MENUITEM_VALUE_CHECKED;
        SetSetting(SETTING_DISPLAY_STATUSBAR, menuitems[6].value, 1); 
      }
      if(menu.selection == 8) {
        if(menuitems[7].value == MENUITEM_VALUE_CHECKED) menuitems[7].value=MENUITEM_VALUE_NONE;
        else menuitems[7].value=MENUITEM_VALUE_CHECKED;
        SetSetting(SETTING_SHOW_ADVANCED, menuitems[7].value, 1); 
      }
      if(menu.selection == 9) {
        if(menuitems[8].value == MENUITEM_VALUE_CHECKED) menuitems[8].value=MENUITEM_VALUE_NONE;
        else menuitems[8].value=MENUITEM_VALUE_CHECKED;
        SetSetting(SETTING_DISPLAY_FKEYS, menuitems[8].value, 1); 
      }
      if(menu.selection == 12) {
        if(menuitems[11].value == MENUITEM_VALUE_CHECKED) menuitems[11].value=MENUITEM_VALUE_NONE;
        else menuitems[11].value=MENUITEM_VALUE_CHECKED;
        SetSetting(SETTING_SHOW_CALENDAR_EVENTS_COUNT, menuitems[11].value, 1);
      }
      // deal with other menu items
      if(menu.selection == 1) { // set time
        setTimeGUI();
      }
      if(menu.selection == 2) { // set date
        setDateGUI();
      }
      
      if(menu.selection == 3) { // set time format
        Selector format;
        strcpy(format.title, "Set time format");
        strcpy(format.subtitle, "");
        format.value = GetSetting(SETTING_TIMEFORMAT);
        format.min = 0;
        format.max = 1;
        format.allowMkey = 0;
        format.cycle = 1;
        format.type = SELECTORTYPE_TIMEFORMAT;
        int res = doSelector(&format);
        if (res == SELECTOR_RETURN_EXIT) continue;
        SetSetting(SETTING_TIMEFORMAT, format.value, 1);
      }
      
      if(menu.selection == 4) { // set long date format
        Selector format;
        strcpy(format.title, "Set long date format");
        strcpy(format.subtitle, "");
        format.value = GetSetting(SETTING_LONGDATEFORMAT);
        format.min = 0;
        format.max = 9;
        format.allowMkey = 0;
        format.cycle = 1;
        format.type = SELECTORTYPE_LONGDATEFORMAT;
        int res = doSelector(&format);
        if (res == SELECTOR_RETURN_EXIT) continue;
        SetSetting(SETTING_LONGDATEFORMAT, format.value, 1);
      }
      
      if(menu.selection == 5) { // set date format
        Selector format;
        strcpy(format.title, "Set date format");
        strcpy(format.subtitle, "");
        format.value = GetSetting(SETTING_DATEFORMAT);
        format.min = 0;
        format.max = 2;
        format.allowMkey = 0;
        format.cycle = 1;
        format.type = SELECTORTYPE_DATEFORMAT;
        int res = doSelector(&format);
        if (res == SELECTOR_RETURN_EXIT) continue;
        SetSetting(SETTING_DATEFORMAT, format.value, 1);
      }
      
      if(menu.selection == 10) { // set startup brightness
        Selector sel;
        strcpy(sel.title, "Set start brightness");
        strcpy(sel.subtitle, "");
        sel.value = GetSetting(SETTING_STARTUP_BRIGHTNESS);
        sel.min = 1;
        sel.max = 250;
        sel.allowMkey = 0;
        sel.cycle = 1;
        sel.type = SELECTORTYPE_STARTUP_BRIGHTNESS;
        int res = doSelector(&sel);
        if (res == SELECTOR_RETURN_EXIT) continue;
        SetSetting(SETTING_STARTUP_BRIGHTNESS, sel.value, 1);
      }
      if(menu.selection == 11) {
        lockSettingsMenu();
      }
      if(menu.selection == 13) {
        showAbout();
      }
    }
  }
}

void lockSettingsMenu() {
  MenuItem menuitems[5];
  strcpy(menuitems[0].text, "Set lock code");;
  
  strcpy(menuitems[1].text, "Show last code char");
  menuitems[1].type = MENUITEM_CHECKBOX;
  
  strcpy(menuitems[2].text, "Off after locking");
  menuitems[2].type = MENUITEM_CHECKBOX;
  
  strcpy(menuitems[3].text, "Lock on [EXE]");
  menuitems[3].type = MENUITEM_CHECKBOX;
  
  strcpy(menuitems[4].text, "Run-Mat on unlock");
  
  Menu menu;
  menu.items=menuitems;
  menu.numitems=5;
  menu.type=MENUTYPE_NORMAL;
  menu.width=21;
  menu.height=8;
  menu.scrollbar=1;
  menu.scrollout=1;
  menu.showtitle=0;
  menu.selection=1;
  menu.scroll=0;
  menu.allowMkey=0;
  strcpy(menu.nodatamsg, "");
  strcpy(menu.title, "");
  strcpy(menu.statusText, "");
  while(1) {
    menuitems[1].value = GetSetting(SETTING_PASSWORD_PRIVACY);
    menuitems[2].value = GetSetting(SETTING_LOCK_AUTOOFF);
    menuitems[3].value = GetSetting(SETTING_LOCK_ON_EXE);
    int res = doMenu(&menu);
    if(res == MENU_RETURN_EXIT) return;
    else if(res == MENU_RETURN_SELECTION) {
      // deal with checkboxes first
      if(menu.selection == 2) {
        if(menuitems[1].value == MENUITEM_VALUE_CHECKED) menuitems[1].value=MENUITEM_VALUE_NONE;
        else menuitems[1].value=MENUITEM_VALUE_CHECKED;
        SetSetting(SETTING_PASSWORD_PRIVACY, menuitems[1].value, 1); 
      }
      if(menu.selection == 3) {
        if(menuitems[2].value == MENUITEM_VALUE_CHECKED) menuitems[2].value=MENUITEM_VALUE_NONE;
        else menuitems[2].value=MENUITEM_VALUE_CHECKED;
        SetSetting(SETTING_LOCK_AUTOOFF, menuitems[2].value, 1); 
      }
      if(menu.selection == 4) {
        if(menuitems[3].value == MENUITEM_VALUE_CHECKED) menuitems[3].value=MENUITEM_VALUE_NONE;
        else menuitems[3].value=MENUITEM_VALUE_CHECKED;
        SetSetting(SETTING_LOCK_ON_EXE, menuitems[3].value, 1); 
      }
      // deal with other menu items
      if(menu.selection == 1) { // set calc lock code
        setPassword();
      }
      if(menu.selection == 5) { // open a menu with Yes, No and Ask
        MsgBoxPush(4);
        MenuItem smallmenuitems[3];
        strcpy(smallmenuitems[0].text, "Off");
        smallmenuitems[0].type = MENUITEM_NORMAL;
        smallmenuitems[0].color = TEXT_COLOR_BLACK;
        
        strcpy(smallmenuitems[1].text, "On");
        smallmenuitems[1].type = MENUITEM_NORMAL;
        smallmenuitems[1].color = TEXT_COLOR_BLACK;
        
        strcpy(smallmenuitems[2].text, "Ask");
        smallmenuitems[2].type = MENUITEM_NORMAL;
        smallmenuitems[2].color = TEXT_COLOR_BLACK;
        
        Menu smallmenu;
        smallmenu.items=smallmenuitems;
        smallmenu.numitems=3;
        smallmenu.type=MENUTYPE_NORMAL;
        smallmenu.width=17;
        smallmenu.height=4;
        smallmenu.startX=3;
        smallmenu.startY=2;
        smallmenu.scrollbar=0;
        smallmenu.scrollout=1;
        smallmenu.showtitle=1;
        smallmenu.selection=GetSetting(SETTING_UNLOCK_RUNMAT)+1;
        smallmenu.scroll=0;
        smallmenu.allowMkey=0;
        strcpy(smallmenu.nodatamsg, "");
        strcpy(smallmenu.title, "Run-Mat on unlock");
        strcpy(smallmenu.statusText, "");
        int sres = doMenu(&smallmenu);
        if(sres == MENU_RETURN_SELECTION) {
          SetSetting(SETTING_UNLOCK_RUNMAT, smallmenu.selection-1, 1);
        }
        MsgBoxPop();
      }
    }
  }
}