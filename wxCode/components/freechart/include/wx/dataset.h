/////////////////////////////////////////////////////////////////////////////
// Name:	dataset.h
// Purpose:
// Author:	Moskvichev Andrey V.
// Created:	2008/11/07
// RCS-ID:	$Id: wxAdvTable.h,v 1.3 2008/11/07 16:42:58 moskvichev Exp $
// Copyright:	(c) 2008 Moskvichev Andrey V.
// Licence:	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef DATASET_H_
#define DATASET_H_

#include <wx/wxfreechartdefs.h>

#include <time.h>
#include <wx/refobject.h>
#include <wx/observable.h>
#include <wx/chartrenderer.h>

class Dataset;

#define FOREACH_SERIE(index, dataset) \
	for (int index = 0; index < dataset->GetSerieCount(); index++)

#define FOREACH_DATAITEM(index, serie, dataset) \
	for (int index = 0; index < dataset->GetCount(serie); index++)

/**
 * Class that receives notifications about dataset changes.
 *
 */
class WXDLLEXPORT DatasetObserver
{
public:
	DatasetObserver();
	virtual ~DatasetObserver();

	virtual void DatasetChanged(Dataset *dataset) = 0;
};

/**
 * Base class for all datasets (XYDatasets, CategoryDatasets, OHLCDatasets, etc).
 *
 */
class WXDLLEXPORT Dataset : public wxObject,
	public RefObject, public Observable<DatasetObserver>, public DrawObserver
{
	DECLARE_CLASS(Dataset)
public:
	Dataset();
	virtual ~Dataset();

	/**
	 * Sets renderer for this dataset.
	 * @param renderer new renderer
	 */
	void SetRenderer(Renderer *renderer);

	Renderer *GetBaseRenderer();

	void BeginUpdate();

	void EndUpdate();

	/**
	 * Returns serie count in this dataset.
	 * @return serie count
	 */
	virtual int GetSerieCount() = 0;

	/**
	 * Returns value count in serie specified by index.
	 * @param serie index
	 * @return value count
	 */
	virtual int GetCount(int serie) = 0;

	/**
	 * Returns serie name.
	 * @param serie index
	 * @return serie name
	 */
	virtual wxString GetSerieName(int serie) = 0;

	virtual double GetMinValue(bool vertical) = 0;

	virtual double GetMaxValue(bool vertical) = 0;

	//
	// DrawObjectObserver
	//
	// Received from renderer
	virtual void NeedRedraw(DrawObject *obj);

protected:
	/**
	 * Checks whether renderer is acceptable by this dataset.
	 * TODO: this method must be removed, because acceptable renderer type depends on plot type.
	 */
	virtual bool AcceptRenderer(Renderer *r) = 0;

	void DatasetChanged();

	Renderer *m_renderer;
	bool m_updating;
	bool m_changed;

private:
	FIRE_WITH_THIS(DatasetChanged);
};

class WXDLLEXPORT DateTimeDataset : public Dataset
{
	DECLARE_CLASS(DateTimeDataset)
public:
	DateTimeDataset();
	virtual ~DateTimeDataset();

	virtual time_t GetDate(int index) = 0;

	virtual int GetCount() = 0;
};

#endif /*DATASET_H_*/