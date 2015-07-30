#include "ui.h"
#include "geotest.h"
#include <stdlib.h>
#include "lcd.h"

extern struct UIPage pMain;
extern struct UIPage pKeyboard;
extern struct UIPage pTime;
extern struct KBInfo kbInfo;
extern struct GeoParam geoparam[10];
extern struct Settings settings;
extern struct UIInfo UIInfo;
static char StringArray[5][20];
struct UIPage pSys;
static struct UIWidget widgetList[5];

static void kbCallBack()
{
	char str[20];
	int val;
	switch(pSys.widgetSelected)
	{
		case 2:
			val = atoi(kbInfo.kbBuff);
			settings.backlight = val>100 ? 100 : val<1 ? 1 : val;
			LCD_BL(settings.backlight);
			break;
		case 3:
			val = atoi(kbInfo.kbBuff);
			settings.sleeptime = val<1 ? 1 : val;
		case 4:
			val = atoi(kbInfo.kbBuff);
			settings.shuttime = val<1 ? 1 : val;
		default:
			break;
	}
	UIInfo.flagSettings = 1;
}

static void goSubSettings(struct UIWidget * widget)
{
	kbInfo.kbParent = &pSys;
	strcpy(kbInfo.kbTitle,widget->widgetTitle);
	strcpy(kbInfo.kbBuff,widget->widgetPtr);
	kbInfo.kbCallBack = kbCallBack;
	PageJump(&pKeyboard);
}

static void goSubPage(struct UIWidget * widget)
{
	PageJump(&pTime);
}


static void widgetInit(struct UIWidget * widget)
{
	switch(widget->widgetIndex)
	{
		case 0:
			strcpy(widget->widgetPtr,"");
			break;
		case 1:
			sprintf(widget->widgetPtr,settings.buzzer ? "ON" : "OFF");
			break;
		case 2:
			sprintf(widget->widgetPtr,"%d",settings.backlight);
			break;
		case 3:
			sprintf(widget->widgetPtr,"%d",settings.sleeptime);
			break;
		case 4:
			sprintf(widget->widgetPtr,"%d",settings.shuttime);
			break;
		default:
			break;
	}
}

static void buzz(struct UIWidget * widget)
{
	settings.buzzer = settings.buzzer ? 0 : 1;
	widget->widgetInit(widget);
	widget->widgetDraw(widget);
}

static void pageReturn(struct UIPage * page)
{
	PageJump(&pMain);
}

static struct UIWidget widgetList[5] =
{
	{0,1,0,{0,120,479,179},"Date & Time",0,StringArray[0],widgetInit,drawSLabel,goSubPage},
	{1,1,0,{0,180,479,239},"Buzzer",0,StringArray[1],widgetInit,drawSLabel,buzz},
	{2,1,0,{0,240,479,299},"Backlight(%)",0,StringArray[2],widgetInit,drawSLabel,goSubSettings},
	{3,1,0,{0,300,479,359},"Sleep Time(Mins)",0,StringArray[3],widgetInit,drawSLabel,goSubSettings},
	{4,1,0,{0,360,479,419},"Shutdown Time(Mins)",0,StringArray[4],widgetInit,drawSLabel,goSubSettings},
};

struct UIPage pSys = 
{
	"System Preference",
	5,//char widgetNum;
	-1,
	widgetList,//struct UIWidget * widgetList;
	NULL,
	pageReturn,
	keyboardEvent,
	touchEvent,
};
