#pragma once
#include "..\..\StdAfx.h"

#include ".\PFButtons.h"

PFButtons::PFButtons()
{

	DefineButtonInfos();
	DefineListButtonInfos();
	DefineClickButtonInfos();
	DefineHintList();
	DefineComboList();
	DefineCheckBoxButtons();
}

PFButtons::~PFButtons()
{
}

void PFButtons::DefineButtonInfos()
{
	CLICK_BUTTON i;
	i.Type = INFOONLYBUTTON;
	i.Param = GetUniqueMenuNumber();
	i.Text = "Equal Number of Records for each site";
	i.TextOptional = "Non Equal size of records for each site";
	i.HintText = "With equal number of Records a single PG File with Repetitions will be created\n";
	i.HintText += "Otherwise a single PG file is created for each site";
	i.Col_start = 2;
	i.Col_end = 4;
	i.Row = 1;
	SetClickButtonMap(i);
	//m_SiteInfoButton = i;

}

void PFButtons::DefineCheckBoxButtons()
{
	CLICK_BUTTON c;
	c.Type = CHECKBOX;
	c.Param = GetUniqueMenuNumber();
	c.VerticalDirection = false;
	c.Col_start = 0;
	c.Col_end = 0;
	c.Row = 2;
	c.HintText = "Click on checkbox to select deselect Variables from output file";
	//m_SelectedOutputVarButton = c;
	SetClickButtonMap(c);
}

void PFButtons::DefineListButtonInfos()
{
	CLICK_BUTTON l;
	
}

void PFButtons::DefineClickButtonInfos()
{
	string ClickAdd;
	ClickAdd = "Click to ";
	CLICK_BUTTON b;
	b.Type = CLICKBUTTON;
	b.Param = GetUniqueMenuNumber();
	b.Col_start = 0;
	b.Col_end = 0;
	b.Row = -2;
	SetClickButtonMap(b);

	b.Text = "Add More PG Files";
	b.Col_start = 0;
	b.HintText = "Click to Add Addition Files to List";
	b.TextOptional = " Click to Specify Files";
	b.Col_end = 0;
	b.Row = -2;
	b.Param = GetUniqueMenuNumber();
	//m_AddMorePGFilesButton = b; 	SetClickButtonMap(b);




	b.HintText = "Click to Add Addition Files to the Mixed";
	b.Param = GetUniqueMenuNumber();



}

void PFButtons::DefineHintList()
{
	HINTLIST h;
	h.param = GetUniqueMenuNumber();
	

}

void PFButtons::DefineComboList()
{
	CLICK_BUTTON c;
	c.Type = COMBOBOX;
	c.Param = GetUniqueMenuNumber();
	c.Row = 2;
	c.Col_start = 3;
	c.Col_end = 3;
	c.HintText = "Click to Select the Repetition Number to be represented";
//	m_RepOptionsCombo = c;
	SetClickButtonMap(c);



}