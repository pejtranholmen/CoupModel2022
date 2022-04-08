#pragma once
#include "..\\std.h"
#include "..\Chart\Chartviewer.h"
#include "..\\Document\Doc.h"
class UG_Case
{
	public:
		UG_Case(Doc *pDoc) {};

		Image operator() (size_t choice) {

			// The data for the bar chart

			double data[] = { 85, 156, 179.5, 211, 123 };

			if (choice > 1) data[2] = 20.;

			// The labels for the bar chart
			const char* labels[] = { "Mon", "Tue", "Wed", "Thu", "Fri" };

			// Create a XYChart object of size 250 x 250 pixels
			unique_ptr<XYChart> c = make_unique<XYChart>(250, 250);

			// Set the plotarea at (30, 20) and of size 200 x 200 pixels
			c->setPlotArea(30, 20, 200, 200);

			// Add a bar chart layer using the given data
			c->addBarLayer(DoubleArray(data, sizeof(data) / sizeof(data[0])));

			// Set the labels on the x axis.
			c->xAxis()->setLabels(CUtil::StringArray(labels, sizeof(labels) / sizeof(labels[0])));
			auto m = c->makeChart(1);
			return ImageCache::getFromMemory(m.data, m.len);
		};

	};



