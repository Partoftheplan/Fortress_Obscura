//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef TF_CLASSMENU_H
#define TF_CLASSMENU_H
#ifdef _WIN32
#pragma once
#endif

#include <classmenu.h>
#include <vgui_controls/EditablePanel.h>
#include "vgui_controls/KeyRepeat.h"
#include <filesystem.h>
#include <tf_shareddefs.h>
#include "cbase.h"
#include "tf_controls.h"
#include "tf_gamerules.h"
#include "basemodelpanel.h"
#include "imagemouseoverbutton.h"
#include "IconPanel.h"
#include <vgui_controls/CheckButton.h>
#include "GameEventListener.h"
#include "c_tf_playerresource.h"

using namespace vgui;

#define CLASS_COUNT_IMAGES	11

//-----------------------------------------------------------------------------
// This is the entire info panel for the specific class
//-----------------------------------------------------------------------------
class CTFClassInfoPanel : public vgui::EditablePanel
{
private:
	DECLARE_CLASS_SIMPLE( CTFClassInfoPanel, vgui::EditablePanel );

public:
	CTFClassInfoPanel( vgui::Panel *parent, const char *panelName ) : vgui::EditablePanel( parent, panelName )
	{
	}

	virtual void SetVisible( bool state )
	{
		CModelPanel *pModelPanel = dynamic_cast<CModelPanel *>(FindChildByName( "classModel" ) );
		if ( pModelPanel )
		{
			pModelPanel->SetPanelDirty();

			if ( !state )
			{
				// stop the panel from running any VCD data
				pModelPanel->DeleteVCDData();
			}
		}

		CTFRichText *pRichText = dynamic_cast<CTFRichText *>(FindChildByName( "classInfo" ) );
		if ( pRichText )
		{
			pRichText->InvalidateLayout( true, false );
		}

		BaseClass::SetVisible( state );
	}
};

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
class CTFClassMenu : public CClassMenu
{
private:
	DECLARE_CLASS_SIMPLE( CTFClassMenu, CClassMenu );

public:
	CTFClassMenu( IViewPort *pViewPort );

	virtual void Update( void );
	virtual Panel *CreateControlByName( const char *controlName );
	virtual void OnTick( void );
	virtual void PaintBackground( void );
	virtual void SetVisible( bool state );
	virtual void PerformLayout();

	MESSAGE_FUNC_CHARPTR( OnShowPage, "ShowPage", page );
	CON_COMMAND_MEMBER_F( CTFClassMenu, "join_class", Join_Class, "Send a joinclass command", 0 );

	virtual void OnClose();
	virtual void ShowPanel( bool bShow );
	virtual void UpdateClassCounts( void ){}

protected:
	virtual void ApplySchemeSettings( IScheme *pScheme );
	virtual void OnKeyCodePressed( KeyCode code );
	virtual CImageMouseOverButton<CTFClassInfoPanel> *GetCurrentClassButton();
	virtual void OnKeyCodeReleased( vgui::KeyCode code );
	virtual void OnThink();
	virtual void UpdateNumClassLabels( int iTeam );

protected:

	CImageMouseOverButton<CTFClassInfoPanel> *m_pClassButtons[TF_CLASS_MENU_BUTTONS];
	CTFClassInfoPanel *m_pClassInfoPanel;

private:

#ifdef _X360
	CTFFooter		*m_pFooter;
#endif

	ButtonCode_t	m_iClassMenuKey;
	int				m_iCurrentClassIndex;
	vgui::CKeyRepeatHandler	m_KeyRepeat;

#ifndef _X360
	CTFImagePanel *m_ClassCountImages[CLASS_COUNT_IMAGES];
	CTFLabel *m_pCountLabel;
#endif
};

//-----------------------------------------------------------------------------
// Purpose: Draws the blue class menu
//-----------------------------------------------------------------------------

class CTFClassMenu_Blue : public CTFClassMenu
{
private:
	DECLARE_CLASS_SIMPLE( CTFClassMenu_Blue, CTFClassMenu );

public:
	CTFClassMenu_Blue( IViewPort *pViewPort ) : BaseClass( pViewPort )
	{
		m_pClassButtons[TF_CLASS_SCOUT] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "scout_blue", m_pClassInfoPanel );
		m_pClassButtons[TF_CLASS_SOLDIER] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "soldier_blue", m_pClassInfoPanel );
		m_pClassButtons[TF_CLASS_PYRO] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "pyro_blue", m_pClassInfoPanel );
		m_pClassButtons[TF_CLASS_DEMOMAN] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "demoman_blue", m_pClassInfoPanel );
		m_pClassButtons[TF_CLASS_MEDIC] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "medic_blue", m_pClassInfoPanel );
		m_pClassButtons[TF_CLASS_HEAVYWEAPONS] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "heavyweapons_blue", m_pClassInfoPanel );
		m_pClassButtons[TF_CLASS_SNIPER] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "sniper_blue", m_pClassInfoPanel );
		m_pClassButtons[TF_CLASS_ENGINEER] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "engineer_blue", m_pClassInfoPanel );
		m_pClassButtons[TF_CLASS_SPY] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "spy_blue", m_pClassInfoPanel );
		m_pClassButtons[TF_CLASS_RANDOM] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "randompc_blue", m_pClassInfoPanel );
	}

	virtual void ApplySchemeSettings( IScheme *pScheme )
	{
		BaseClass::ApplySchemeSettings( pScheme );

		LoadControlSettings( "Resource/UI/Classmenu_blue.res" );

		for( int i = 0; i < GetChildCount(); i++ ) 
		{
			CImageMouseOverButton<CTFClassInfoPanel> *button = dynamic_cast<CImageMouseOverButton<CTFClassInfoPanel> *>( GetChild( i ) );

			if ( button )
			{
				button->SetPreserveArmedButtons( true );
				button->SetUpdateDefaultButtons( true );
			}
		}
	}

	virtual void ShowPanel( bool bShow )
	{
		if ( bShow )
		{
			// make sure the Red class menu isn't open
			if ( gViewPortInterface )
			{
				gViewPortInterface->ShowPanel( PANEL_CLASS_RED, false );
			}
			// make sure the Ylw class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_YLW, false);
			}
			// make sure the Grn class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_GRN, false);
			}
			// make sure the Prp class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_PRP, false);
			}
			// make sure the Brw class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_BRW, false);
			}
		}

		BaseClass::ShowPanel( bShow );
	}

	virtual const char *GetName( void )
	{ 
		return PANEL_CLASS_BLUE; 
	}

	virtual int GetTeamNumber( void )
	{
		return 
			TF_TEAM_BLUE;
	}

	virtual void UpdateClassCounts( void ){ UpdateNumClassLabels( TF_TEAM_BLUE ); }
};

//-----------------------------------------------------------------------------
// Purpose: Draws the red class menu
//-----------------------------------------------------------------------------

class CTFClassMenu_Red : public CTFClassMenu
{
private:
	DECLARE_CLASS_SIMPLE( CTFClassMenu_Red, CTFClassMenu );

public:
	CTFClassMenu_Red( IViewPort *pViewPort ) : BaseClass( pViewPort )
	{
		m_pClassButtons[TF_CLASS_SCOUT] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "scout_red", m_pClassInfoPanel );
		m_pClassButtons[TF_CLASS_SOLDIER] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "soldier_red", m_pClassInfoPanel );
		m_pClassButtons[TF_CLASS_PYRO] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "pyro_red", m_pClassInfoPanel );
		m_pClassButtons[TF_CLASS_DEMOMAN] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "demoman_red", m_pClassInfoPanel );
		m_pClassButtons[TF_CLASS_MEDIC] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "medic_red", m_pClassInfoPanel );
		m_pClassButtons[TF_CLASS_HEAVYWEAPONS] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "heavyweapons_red", m_pClassInfoPanel );
		m_pClassButtons[TF_CLASS_SNIPER] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "sniper_red", m_pClassInfoPanel );
		m_pClassButtons[TF_CLASS_ENGINEER] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "engineer_red", m_pClassInfoPanel );
		m_pClassButtons[TF_CLASS_SPY] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "spy_red", m_pClassInfoPanel );
		m_pClassButtons[TF_CLASS_RANDOM] = new CImageMouseOverButton<CTFClassInfoPanel>( this, "randompc_red", m_pClassInfoPanel );
	}

	virtual void ApplySchemeSettings( IScheme *pScheme )
	{
		BaseClass::ApplySchemeSettings( pScheme );

		LoadControlSettings( "Resource/UI/Classmenu_red.res" );

		for( int i = 0; i < GetChildCount(); i++ ) 
		{
			CImageMouseOverButton<CTFClassInfoPanel> *button = dynamic_cast<CImageMouseOverButton<CTFClassInfoPanel> *>( GetChild( i ) );

			if ( button )
			{
				button->SetPreserveArmedButtons( true );
				button->SetUpdateDefaultButtons( true );
			}
		}
	}

	virtual void ShowPanel( bool bShow )
	{
		if ( bShow )
		{
			// make sure the Blue class menu isn't open
			if ( gViewPortInterface )
			{
				gViewPortInterface->ShowPanel( PANEL_CLASS_BLUE, false );
			}
			// make sure the Ylw class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_YLW, false);
			}
			// make sure the Grn class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_GRN, false);
			}
			// make sure the Prp class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_PRP, false);
			}
			// make sure the Brw class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_BRW, false);
			}
		}

		BaseClass::ShowPanel( bShow );
	}

	virtual const char *GetName( void )
	{ 
		return PANEL_CLASS_RED;
	}

	virtual int GetTeamNumber( void )
	{
		return TF_TEAM_RED;
	}

	virtual void UpdateClassCounts( void ){ UpdateNumClassLabels( TF_TEAM_RED ); }
};

//-----------------------------------------------------------------------------
// Purpose: Draws the ylw class menu
//-----------------------------------------------------------------------------

class CTFClassMenu_Ylw : public CTFClassMenu
{
private:
	DECLARE_CLASS_SIMPLE(CTFClassMenu_Ylw, CTFClassMenu);

public:
	CTFClassMenu_Ylw(IViewPort *pViewPort) : BaseClass(pViewPort)
	{
		m_pClassButtons[TF_CLASS_SCOUT] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "scout_ylw", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_SOLDIER] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "soldier_ylw", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_PYRO] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "pyro_ylw", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_DEMOMAN] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "demoman_ylw", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_MEDIC] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "medic_ylw", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_HEAVYWEAPONS] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "heavyweapons_ylw", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_SNIPER] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "sniper_ylw", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_ENGINEER] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "engineer_ylw", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_SPY] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "spy_ylw", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_RANDOM] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "randompc_ylw", m_pClassInfoPanel);
	}

	virtual void ApplySchemeSettings(IScheme *pScheme)
	{
		BaseClass::ApplySchemeSettings(pScheme);

		LoadControlSettings("Resource/UI/Classmenu_ylw.res");

		for (int i = 0; i < GetChildCount(); i++)
		{
			CImageMouseOverButton<CTFClassInfoPanel> *button = dynamic_cast<CImageMouseOverButton<CTFClassInfoPanel> *>(GetChild(i));

			if (button)
			{
				button->SetPreserveArmedButtons(true);
				button->SetUpdateDefaultButtons(true);
			}
		}
	}

	virtual void ShowPanel(bool bShow)
	{
		if (bShow)
		{
			// make sure the Blue class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_BLUE, false);
			}
			// make sure the Red class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_RED, false);
			}
			// make sure the Grn class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_GRN, false);
			}
			// make sure the Prp class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_PRP, false);
			}
			// make sure the Brw class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_BRW, false);
			}
		}

		BaseClass::ShowPanel(bShow);
	}

	virtual const char *GetName(void)
	{
		return PANEL_CLASS_YLW;
	}

	virtual int GetTeamNumber(void)
	{
		return TF_TEAM_YLW;
	}

	virtual void UpdateClassCounts(void){ UpdateNumClassLabels(TF_TEAM_YLW); }
};

//-----------------------------------------------------------------------------
// Purpose: Draws the grn class menu
//-----------------------------------------------------------------------------

class CTFClassMenu_Grn : public CTFClassMenu
{
private:
	DECLARE_CLASS_SIMPLE(CTFClassMenu_Grn, CTFClassMenu);

public:
	CTFClassMenu_Grn(IViewPort *pViewPort) : BaseClass(pViewPort)
	{
		m_pClassButtons[TF_CLASS_SCOUT] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "scout_grn", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_SOLDIER] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "soldier_grn", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_PYRO] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "pyro_grn", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_DEMOMAN] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "demoman_grn", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_MEDIC] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "medic_grn", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_HEAVYWEAPONS] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "heavyweapons_grn", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_SNIPER] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "sniper_grn", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_ENGINEER] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "engineer_grn", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_SPY] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "spy_grn", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_RANDOM] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "randompc_grn", m_pClassInfoPanel);
	}

	virtual void ApplySchemeSettings(IScheme *pScheme)
	{
		BaseClass::ApplySchemeSettings(pScheme);

		LoadControlSettings("Resource/UI/Classmenu_grn.res");

		for (int i = 0; i < GetChildCount(); i++)
		{
			CImageMouseOverButton<CTFClassInfoPanel> *button = dynamic_cast<CImageMouseOverButton<CTFClassInfoPanel> *>(GetChild(i));

			if (button)
			{
				button->SetPreserveArmedButtons(true);
				button->SetUpdateDefaultButtons(true);
			}
		}
	}

	virtual void ShowPanel(bool bShow)
	{
		if (bShow)
		{
			// make sure the Blue class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_BLUE, false);
			}
			// make sure the Red class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_RED, false);
			}
			// make sure the Ylw class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_YLW, false);
			}
			// make sure the Prp class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_PRP, false);
			}
			// make sure the Brw class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_BRW, false);
			}
		}

		BaseClass::ShowPanel(bShow);
	}

	virtual const char *GetName(void)
	{
		return PANEL_CLASS_GRN;
	}

	virtual int GetTeamNumber(void)
	{
		return TF_TEAM_GRN;
	}

	virtual void UpdateClassCounts(void){ UpdateNumClassLabels(TF_TEAM_GRN); }
};

//-----------------------------------------------------------------------------
// Purpose: Draws the prp class menu
//-----------------------------------------------------------------------------

class CTFClassMenu_Prp : public CTFClassMenu
{
private:
	DECLARE_CLASS_SIMPLE(CTFClassMenu_Prp, CTFClassMenu);

public:
	CTFClassMenu_Prp(IViewPort *pViewPort) : BaseClass(pViewPort)
	{
		m_pClassButtons[TF_CLASS_SCOUT] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "scout_prp", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_SOLDIER] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "soldier_prp", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_PYRO] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "pyro_prp", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_DEMOMAN] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "demoman_prp", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_MEDIC] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "medic_prp", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_HEAVYWEAPONS] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "heavyweapons_prp", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_SNIPER] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "sniper_prp", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_ENGINEER] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "engineer_prp", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_SPY] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "spy_prp", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_RANDOM] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "randompc_prp", m_pClassInfoPanel);
	}

	virtual void ApplySchemeSettings(IScheme *pScheme)
	{
		BaseClass::ApplySchemeSettings(pScheme);

		LoadControlSettings("Resource/UI/Classmenu_prp.res");

		for (int i = 0; i < GetChildCount(); i++)
		{
			CImageMouseOverButton<CTFClassInfoPanel> *button = dynamic_cast<CImageMouseOverButton<CTFClassInfoPanel> *>(GetChild(i));

			if (button)
			{
				button->SetPreserveArmedButtons(true);
				button->SetUpdateDefaultButtons(true);
			}
		}
	}

	virtual void ShowPanel(bool bShow)
	{
		if (bShow)
		{
			// make sure the Blue class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_BLUE, false);
			}
			// make sure the Red class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_RED, false);
			}
			// make sure the Ylw class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_YLW, false);
			}
			// make sure the Grn class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_GRN, false);
			}
			// make sure the Brw class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_BRW, false);
			}
		}

		BaseClass::ShowPanel(bShow);
	}

	virtual const char *GetName(void)
	{
		return PANEL_CLASS_PRP;
	}

	virtual int GetTeamNumber(void)
	{
		return TF_TEAM_PRP;
	}

	virtual void UpdateClassCounts(void){ UpdateNumClassLabels(TF_TEAM_PRP); }
};

//-----------------------------------------------------------------------------
// Purpose: Draws the brw class menu
//-----------------------------------------------------------------------------

class CTFClassMenu_Brw : public CTFClassMenu
{
private:
	DECLARE_CLASS_SIMPLE(CTFClassMenu_Brw, CTFClassMenu);

public:
	CTFClassMenu_Brw(IViewPort *pViewPort) : BaseClass(pViewPort)
	{
		m_pClassButtons[TF_CLASS_SCOUT] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "scout_brw", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_SOLDIER] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "soldier_brw", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_PYRO] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "pyro_brw", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_DEMOMAN] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "demoman_brw", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_MEDIC] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "medic_brw", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_HEAVYWEAPONS] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "heavyweapons_brw", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_SNIPER] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "sniper_brw", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_ENGINEER] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "engineer_brw", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_SPY] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "spy_brw", m_pClassInfoPanel);
		m_pClassButtons[TF_CLASS_RANDOM] = new CImageMouseOverButton<CTFClassInfoPanel>(this, "randompc_brw", m_pClassInfoPanel);
	}

	virtual void ApplySchemeSettings(IScheme *pScheme)
	{
		BaseClass::ApplySchemeSettings(pScheme);

		LoadControlSettings("Resource/UI/Classmenu_brw.res");

		for (int i = 0; i < GetChildCount(); i++)
		{
			CImageMouseOverButton<CTFClassInfoPanel> *button = dynamic_cast<CImageMouseOverButton<CTFClassInfoPanel> *>(GetChild(i));

			if (button)
			{
				button->SetPreserveArmedButtons(true);
				button->SetUpdateDefaultButtons(true);
			}
		}
	}

	virtual void ShowPanel(bool bShow)
	{
		if (bShow)
		{
			// make sure the Blue class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_BLUE, false);
			}
			// make sure the Red class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_RED, false);
			}
			// make sure the Ylw class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_YLW, false);
			}
			// make sure the Grn class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_GRN, false);
			}
			// make sure the Prp class menu isn't open
			if (gViewPortInterface)
			{
				gViewPortInterface->ShowPanel(PANEL_CLASS_PRP, false);
			}
		}

		BaseClass::ShowPanel(bShow);
	}

	virtual const char *GetName(void)
	{
		return PANEL_CLASS_BRW;
	}

	virtual int GetTeamNumber(void)
	{
		return TF_TEAM_BRW;
	}

	virtual void UpdateClassCounts(void){ UpdateNumClassLabels(TF_TEAM_BRW); }
};

#endif // TF_CLASSMENU_H

