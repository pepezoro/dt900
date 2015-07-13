#include "GUI.h"
#include "ui.h"

extern GUI_CONST_STORAGE GUI_FONT GUI_FontHelvetica32;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontVerdana32;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontHelveticaNeueLT48;

extern struct UIPage pMain;
extern struct UIPage pKeyboard;

struct UIInfo UIInfo;



void UIKeyboard()
{
	UIInfo.currentKey = GPIOA->IDR&0x7;
	
	if(UIInfo.currentKey!=UIInfo.lastKey)
	{
		UIInfo.keyCombo=0;
		UIInfo.KeyEvent = UIInfo.currentKey;
	}
	else
	{
		UIInfo.keyCombo++;
		if(UIInfo.keyCombo%50==0)
		{
				UIInfo.KeyEvent = UIInfo.currentKey;
		}
	}
	UIInfo.lastKey = UIInfo.currentKey;
}

void UITouch()
{
	//keyCombo=0;
}
	
void UIEventManager()
{
	if(UIInfo.keyCombo>30000 || (UIInfo.keyCombo==250 && UIInfo.KeyEvent==KEY_CANCEL))
	{
		LCD_PWR(0);
		DP_EN(0);
	}
	
	if(UIInfo.KeyEvent!=KEY_NONE)
		UIInfo.PagePtr->keyboard(UIInfo.PagePtr);
}

void keyboardEvent(struct UIPage *page)
{
	if(UIInfo.KeyEvent==KEY_CANCEL && UIInfo.keyCombo==0)
	{
		if(page->widgetSelected>=0)
		{
			page->widgetList[page->widgetSelected].active=0;
			page->widgetList[page->widgetSelected].widgetDraw(&page->widgetList[page->widgetSelected]);
			page->widgetSelected = -1;
		}
		if(page->pageReturn)
		{
			page->pageReturn(page);
		}
	}
	else if(UIInfo.KeyEvent==KEY_DOWN)
	{
		if(page->widgetSelected>=0 && page->widgetNum>1)
		{
			page->widgetList[page->widgetSelected].active=0;
			page->widgetList[page->widgetSelected].widgetDraw(&page->widgetList[page->widgetSelected]);
		}
		do
		page->widgetSelected = page->widgetSelected == page->widgetNum-1 ? 0 : page->widgetSelected+1;
		while(!page->widgetList[page->widgetSelected].enable);
		page->widgetList[page->widgetSelected].active=1;
		page->widgetList[page->widgetSelected].widgetDraw(&page->widgetList[page->widgetSelected]);
	}
	else if(UIInfo.KeyEvent==KEY_OK && UIInfo.keyCombo==0)
	{
		if(page->widgetSelected>=0 && page->widgetList[page->widgetSelected].widgetAct)
			page->widgetList[page->widgetSelected].widgetAct(&page->widgetList[page->widgetSelected]);
	}	
	else if(UIInfo.KeyEvent==KEY_OK && page==&pKeyboard)
	{
		if(page->widgetSelected>=0 && page->widgetList[page->widgetSelected].widgetAct)
			page->widgetList[page->widgetSelected].widgetAct(&page->widgetList[page->widgetSelected]);
	}
	UIInfo.KeyEvent = KEY_NONE;
}

void touchEvent(struct UIPage *page)
{
	
}

void UI_Init()
{
	memset(&UIInfo,0,sizeof(UIInfo));
	GUI_SetBkColor(WHITE);
	GUI_Clear();
	GUI_SetColor(TITLECOLOR);
	GUI_FillRect(0,0,479,119);
	PageJump(&pMain);
}


void PageJump(struct UIPage *page)
{
	UIInfo.PagePtr = page;
	GUI_SetColor(TITLECOLOR);
	GUI_FillRect(0,60,479,119);
	GUI_SetColor(WHITE);
	GUI_FillRect(0,120,479,799);
	
	if(page->pageInit)
		(page->pageInit)(page);
	UIDraw(page);
}

void UIDraw(struct UIPage *page)
{
	uint32_t i;
	
	GUI_SetColor(WHITE);
	GUI_SetBkColor(TITLECOLOR);
	GUI_SetFont(&GUI_FontHelveticaNeueLT48);
	GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
	GUI_DispStringAt(page->pageTitle,240,76);
	
	for(i=0;i<page->widgetNum;i++)
	{
		if(page->widgetList[i].widgetInit)
			page->widgetList[i].widgetInit(&page->widgetList[i]);
		
		if(page->widgetList[i].widgetDraw)
			page->widgetList[i].widgetDraw(&page->widgetList[i]);
	}
}

void drawButton(struct UIWidget* widget)
{
	GUI_SetColor(WHITE);
	GUI_FillRect(widget->rect.x0,widget->rect.y0,widget->rect.x1,widget->rect.y1);	
	GUI_SetPenSize(2);
	GUI_SetColor(widget->enable ? (widget->active ? 0x003fceff : 0x002f2f2f) : 0x00f0f0f0);
	GUI_AA_DrawRoundedRect(widget->rect.x0+1,widget->rect.y0+1,widget->rect.x1-1,widget->rect.y1-1,(widget->rect.y1-widget->rect.y0-2)/2);
	GUI_SetColor(widget->enable ? (widget->active ? 0x002fbeff : BLACK) : 0x00f0f0f0);
	GUI_SetBkColor(WHITE);
	GUI_SetFont(&GUI_FontHelvetica32);	
	GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
	GUI_DispStringAt(widget->widgetTitle,(widget->rect.x1+widget->rect.x0)/2,(widget->rect.y1+widget->rect.y0)/2);
}


void drawLabel(struct UIWidget* widget)
{
	if(!widget->enable)
			return;
	GUI_SetColor(WHITE);
	GUI_FillRect(widget->rect.x0,widget->rect.y0,widget->rect.x1,widget->rect.y1);
	GUI_SetColor(widget->active ? 0x002fbeff : BLACK);
	GUI_SetBkColor(WHITE);
	GUI_SetFont(&GUI_FontHelvetica32);	
	GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
	GUI_DispStringAt(widget->widgetTitle,(widget->rect.x1+widget->rect.x0)/2,(widget->rect.y1+widget->rect.y0)/2);
	GUI_SetColor(DARKGRAY);
	GUI_DrawHLine(widget->rect.y1,20,459);
}

void drawSLabel(struct UIWidget* widget)
{
	GUI_SetColor(WHITE);
	GUI_FillRect(widget->rect.x0,widget->rect.y0,widget->rect.x1,widget->rect.y1);
	
	GUI_SetFont(&GUI_FontHelvetica32);
	
	GUI_SetColor(widget->enable ? (widget->active ? 0x002fbeff : BLACK) : 0x00f0f0f0);
	GUI_SetBkColor(WHITE);
	GUI_SetTextAlign(GUI_TA_RIGHT | GUI_TA_VCENTER);	
	GUI_DispStringAt((char *)widget->widgetPtr,439,(widget->rect.y1+widget->rect.y0)/2);
	
	GUI_SetColor(widget->enable ? (widget->active ? 0x002fbeff : BLACK) : 0x00f0f0f0);
	GUI_SetBkColor(WHITE);
	GUI_SetTextAlign(GUI_TA_LEFT | GUI_TA_VCENTER);	
	GUI_DispStringAt(widget->widgetTitle,40,(widget->rect.y1+widget->rect.y0)/2);
	
	GUI_SetColor(DARKGRAY);
	GUI_DrawHLine(widget->rect.y1,widget->rect.x0+20,widget->rect.x1-20);
}
	