#include "ui.h"
#include "geotest.h"
#include <stdlib.h>

extern struct UIPage pSettings;
extern struct UIPage pSubSettings;
extern struct UIPage pKeyboard;
extern struct KBInfo kbInfo;
extern struct GeoParam geoparam[10];
extern struct Settings settings;

static char StringArray[3][20];
static struct UIWidget widgetList[3];
struct UIPage pEditF;
	
static void kbCallBack()
{
	char str[20];
	switch(pEditF.widgetSelected)
	{
		case 0:
			geoparam[pSettings.widgetSelected].F = (int)(atof(kbInfo.kbBuff)*10)/10.0;
			break;
		case 1:
			geoparam[pSettings.widgetSelected].Fp = (int)(atof(kbInfo.kbBuff)*10)/1000.0;
			break;
		case 2:
			geoparam[pSettings.widgetSelected].Fn = (int)(atof(kbInfo.kbBuff)*10)/1000.0;
			break;
		default:
			break;
	}
}

static void goSubSettings(struct UIWidget * widget)
{
	kbInfo.kbParent = &pEditF;
	strcpy(kbInfo.kbTitle,widget->widgetTitle);
	strcpy(kbInfo.kbBuff,widget->widgetPtr);
	kbInfo.kbCallBack = kbCallBack;
	PageJump(&pKeyboard);
}

static void widgetInit(struct UIWidget * widget)
{
	switch(widget->widgetIndex)
	{
		case 0:
			sprintf(widget->widgetPtr,"%.1f",geoparam[pSettings.widgetSelected].F);
			break;
		case 1:
			sprintf(widget->widgetPtr,"%.1f",geoparam[pSettings.widgetSelected].Fp*100);
			break;
		case 2:
			sprintf(widget->widgetPtr,"%.1f",geoparam[pSettings.widgetSelected].Fn*100);
			break;
		default:
			break;
	}
}

static void pageReturn(struct UIPage * page)
{
	PageJump(&pSubSettings);
}

static struct UIWidget widgetList[3] =
{
	{0,1,0,{0,120,479,179},"Frequency(Hz)",0,StringArray[0],widgetInit,drawSLabel,goSubSettings},
	{1,1,0,{0,180,479,239},"F Max(+)",0,StringArray[1],widgetInit,drawSLabel,goSubSettings},
	{2,1,0,{0,240,479,299},"F Min(-)",0,StringArray[2],widgetInit,drawSLabel,goSubSettings},
};

struct UIPage pEditF = 
{
	"Frequency",
	3,//char widgetNum;
	-1,
	widgetList,//struct UIWidget * widgetList;
	NULL,
	pageReturn,
	keyboardEvent,
	touchEvent,
};

