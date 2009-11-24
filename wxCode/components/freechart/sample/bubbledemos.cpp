/////////////////////////////////////////////////////////////////////////////
// Name:	bubbledemos.cpp
// Purpose: This file contains source code, that demonstrates how to
//          create bubble charts.
// Author:	Moskvichev Andrey V.
// Created:	2009/04/03
// RCS-ID:	$Id: wxAdvTable.h,v 1.3 2008/04/03 16:42:58 moskvichev Exp $
// Copyright:	(c) 2009 Moskvichev Andrey V.
// Licence:	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#include "democollection.h"

#include <wx/xyz/bubbleplot.h>
#include <wx/xyz/xyzrenderer.h>
#include <wx/xyz/xyzdataset.h>
#include <wx/axis/numberaxis.h>

#include <wx/dynarray.h>
#include <wx/arrimpl.cpp>

/**
 * XYZ serie data.
 */
class XYZSerie
{
public:
	XYZSerie(double *data, int count)
	{
		m_data = new double[3 * count];
		memcpy(m_data, data, 3 * count * sizeof(double));
		m_count = count;
	}

	virtual ~XYZSerie()
	{
		wxDELETEA(m_data);
	}

	double GetX(int index)
	{
		wxCHECK_MSG(index < m_count, 0, wxT("XYSerie::GetX"));
		return m_data[index * 3];
	}

	double GetY(int index)
	{
		wxCHECK_MSG(index < m_count, 0, wxT("XYSerie::GetY"));
		return m_data[index * 3 + 1];
	}

	double GetZ(int index)
	{
		wxCHECK_MSG(index < m_count, 0, wxT("XYSerie::GetZ"));
		return m_data[index * 3 + 2];
	}

	int GetCount()
	{
		return m_count;
	}

	const wxString &GetName()
	{
		return m_name;
	}

	void SetName(const wxString &name)
	{
		m_name = name;;
	}

private:
	double *m_data;
	int m_count;
	wxString m_name;
};

WX_DECLARE_OBJARRAY(XYZSerie *, XYZSerieArray);
WX_DEFINE_OBJARRAY(XYZSerieArray);

/**
 * XYZ demo dataset.
 */
class XYZDemoDataset : public XYZDataset
{
public:
	XYZDemoDataset()
	{
	}

	virtual ~XYZDemoDataset()
	{
		for (size_t n = 0; n < m_series.Count(); n++) {
			wxDELETE(m_series[n]);
		}
	}

	void AddSerie(double *data, int count)
	{
		m_series.Add(new XYZSerie(data, count));
		DatasetChanged();
	}

	virtual double GetX(int index, int serie)
	{
		return m_series[serie]->GetX(index);
	}

	virtual double GetY(int index, int serie)
	{
		return m_series[serie]->GetY(index);
	}

	virtual double GetZ(int index, int serie)
	{
		return m_series[serie]->GetZ(index);
	}

	virtual int GetSerieCount()
	{
		return m_series.Count();
	}

	virtual int GetCount(int serie)
	{
		return m_series[serie]->GetCount();
	}

	virtual wxString GetSerieName(int serie)
	{
		return m_series[serie]->GetName();
	}

	void SetSerieName(int serie, const wxString &name)
	{
		m_series[serie]->SetName(name);
		DatasetChanged();
	}

private:
	XYZSerieArray m_series;
};

/**
 * Bubble demo with one xyz serie.
 */
class BubbleDemo1 : public ChartDemo
{
public:
	BubbleDemo1()
	: ChartDemo(wxT("Bubble Demo 1 - simple"))
	{
	}

	virtual Chart *Create()
	{
		// serie 1 values
		double data1[][3] = {
				{ 10, 20, 10, },
				{ 13, 16, 40, },
				{ 15, 26, 30, },
				{ 7, 30, 11, },
				{ 25, 14, 15, },
				{ 15, 34, 35, },
				{ 25, 4, 20, },
		};
		// serie 2 values
		double data2[][3] = {
				{ 13, 18, 40, },
				{ 53, 36, 20, },
				{ 25, 23, 10, },
				{ 76, 40, 41, },
				{ 30, 15, 25, },
				{ 45, 24, 45, },
				{ 15, 4, 10, },
		};

		// first step: create plot
		BubblePlot *plot = new BubblePlot();

		// create xyz dataset and add serie to it
		XYZDemoDataset *dataset = new XYZDemoDataset();
		dataset->AddSerie((double *) data1, WXSIZEOF(data1));
		dataset->AddSerie((double *) data2, WXSIZEOF(data2));

		// minimal/maximal radius for bubbles
		const int minRad = 5;
		const int maxRad = 15;

		// set xyz renderer with minimal and maximal radius values
		dataset->SetRenderer(new XYZRenderer(minRad, maxRad));

		plot->AddDataset(dataset);

		// create left number axis
		NumberAxis *leftAxis = new NumberAxis(AXIS_LEFT);
		leftAxis->SetMargins(maxRad, maxRad);

		// create bottom number axis
		NumberAxis *bottomAxis = new NumberAxis(AXIS_BOTTOM);
		bottomAxis->SetMargins(maxRad, maxRad);

		// add axes to plot
		plot->AddAxis(leftAxis);
		plot->AddAxis(bottomAxis);

		// link axes and dataset
		plot->LinkDataVerticalAxis(0, 0);
		plot->LinkDataHorizontalAxis(0, 0);

		// and finally create chart
		return new Chart(plot, GetName());
	}
};

ChartDemo *bubbleDemos[] = {
		new BubbleDemo1(),
};
int bubbleDemosCount = WXSIZEOF(bubbleDemos);
