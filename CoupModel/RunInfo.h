#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "..\Document\Doc.h"
#include "..\\CoupModelUtil\Register.h"
#include "..\\PG\PGUtil.h"
#include "..\Simulator\DefineModel.h"

class RunInfo : public Component, public MouseListener
{
public:
	RunInfo(Doc* pdoc) : pDoc(pdoc) {

		m_labels.push_back("General Settings and Document Info");

		m_InfoText.push_back("Start Date");

		addAndMakeVisible(m_TitleLabel[0]);

		m_TitleLabel[0].setText(m_labels[0], NotificationType::dontSendNotification); 

		m_TitleLabel[0].setJustificationType(Justification::centred);




		addAndMakeVisible(m_InfoLabel[0]);
		m_InfoLabel[0].setText(m_InfoText[0], NotificationType::dontSendNotification);
		m_InfoLabel[0].setText(m_InfoText[0], NotificationType::dontSendNotification);

		addAndMakeVisible(m_EditLabel[0]);
		auto txt = pDoc->GetTimeCreated();
		auto DatumString = PGUtil::StringDatum(pDoc->RunOpt.longMinStart);
		m_EditLabel[0].setText(DatumString, NotificationType::dontSendNotification);
		m_EditLabel[0].setEditable(false, true);
		m_EditLabel[0].setColour(juce::Label::backgroundColourId, Colours::lightyellow);
		m_EditLabel[0].onTextChange = [this]() { m_EditLabel[0].setText(m_EditLabel[0].getText(), NotificationType::dontSendNotification);




		};
	};
	~RunInfo() {};
	void resized() override
		{
			auto area = getLocalBounds();
			m_TitleLabel[0].setBounds(10, 10, getWidth() - 20, 30);
			m_InfoLabel[0].setBounds(10, 30, getWidth() / 4 - 20, 30);
			m_EditLabel[0].setBounds(getWidth() / 4 + 10, 30, getWidth() / 4 - 20, 30);
		}
	void paint(Graphics& g) override {};
private:
	vector<String> m_labels;
	vector<String> m_InfoText;
	Doc* pDoc;
	juce::Label m_InfoLabel[5];
	juce::Label m_EditLabel[5];
	juce::Label m_TitleLabel[5];
	TextButton txtb;

};
