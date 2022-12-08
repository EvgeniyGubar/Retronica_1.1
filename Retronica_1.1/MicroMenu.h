/**
              MICRO-MENU V2

          (C) Dean Camera, 2012
        www.fourwalledcubicle.com
     dean [at] fourwalledcubicle.com

        Royalty-free for all uses.
	                                  */
#ifndef _MICRO_MENU_H_
#define _MICRO_MENU_H_

	#include <stddef.h>
	#include <stdint.h>

	#include "MenuConfig.h"
	

	typedef const struct Menu_Item {
		const struct Menu_Item *Next;		/**< Pointer to the next menu item of this menu item */
		const struct Menu_Item *Previous;	/**< Pointer to the previous menu item of this menu item */
		const struct Menu_Item *Child;		/**< Pointer to the child menu item of this menu item */
		void (*SelectCallback)(void);		/**< Pointer to the optional menu-specific select callback of this menu item */
	
	} Menu_Item_t;

	#define MENU_ITEM(Name, Next, Previous, Child, SelectFunc)\
		extern Menu_Item_t MENU_ITEM_STORAGE Next;\
		extern Menu_Item_t MENU_ITEM_STORAGE Previous;\
		extern Menu_Item_t MENU_ITEM_STORAGE Child;\
		Menu_Item_t MENU_ITEM_STORAGE Name = {&Next, &Previous, &Child, SelectFunc}

	/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the menu child. */
	#define MENU_CHILD          MENU_ITEM_READ_POINTER(&Menu_GetCurrentMenu()->Child)

	/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the next linked menu item. */
	#define MENU_NEXT           MENU_ITEM_READ_POINTER(&Menu_GetCurrentMenu()->Next)

	/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the previous linked menu item. */
	#define MENU_PREVIOUS       MENU_ITEM_READ_POINTER(&Menu_GetCurrentMenu()->Previous)

	/** Null menu entry, used in \ref MENU_ITEM() definitions where no menu link is to be made. */
	extern Menu_Item_t MENU_ITEM_STORAGE NULL_MENU;

	Menu_Item_t* Menu_GetCurrentMenu(void);

	void Menu_Navigate(Menu_Item_t* const NewMenu);

#endif



/** This is used when an invalid menu handle is required in
 *  a \ref MENU_ITEM() definition, i.e. to indicate that a
 *  menu has no linked parent, child, next or previous entry.
 */
Menu_Item_t PROGMEM NULL_MENU = {0};

/** \internal
 *  Pointer to the currently selected menu item.
 */
static Menu_Item_t* CurrentMenuItem = &NULL_MENU;

Menu_Item_t* Menu_GetCurrentMenu(void)
{
	return CurrentMenuItem;
}

void Menu_Navigate(Menu_Item_t* const NewMenu)
{
	
	if ((NewMenu == &NULL_MENU) || (NewMenu == NULL))
		return;

	CurrentMenuItem = NewMenu;
 
	void (*SelectCallback)(void) = MENU_ITEM_READ_POINTER(&CurrentMenuItem->SelectCallback);

	if (SelectCallback)
		SelectCallback();
}
