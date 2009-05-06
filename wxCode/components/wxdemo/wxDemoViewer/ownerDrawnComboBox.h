/***************************************************************
 * Name:      ownerDrawnComboBox.h
 * Purpose:   Defines class encapsulating single Combo Box item and class for Combo Box with owner-drawn list items
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef OWNERDRAWNCOMBOBOX_H
#define OWNERDRAWNCOMBOBOX_H

#include <wx/wx.h>
#include <wx/odcombo.h>
#include <wx/imaglist.h>

/*! \brief Class encapsulating Combo Box item. */
class ownerDrawnComboBoxData {
    public:
        /*! \brief Text displayed in this Combo Box item. */
        wxString text;

        /*! \brief Text value asigned for this Combo Box item. */
        wxString value;

        /*! \brief Image index for this Combo Box item. */
        int imgIndex;

        /*! \brief Lexer for this Combo Box item. */
        int lexer;

        /*!
         * \brief Default contructor.
         * \param text Text displayed in this Combo Box item
         * \param value Text value asigned for this Combo Box item
         * \param imgIndex Image index for this Combo Box item
         * \param lexer Lexer for this Combo Box item
         */
        ownerDrawnComboBoxData(wxString text, wxString value, int imgIndex, int lexer = 0);

        /*! \brief Destructor */
        ~ownerDrawnComboBoxData();
};

WX_DECLARE_LIST(ownerDrawnComboBoxData, ownerDrawnComboBoxDataList);

/*! \brief Class represents a Combo Box with owner-drawn list items. */
class ownerDrawnComboBox : public wxOwnerDrawnComboBox
{
    public:
        /*!
         * \brief Default contructor.
         * \param parent Pointer to a parent window
         * \param id Window identifier
         * \param imgList Pointer to Image List
         * \param textColor Text color
         * \param backColor Background color
         */
        ownerDrawnComboBox(wxWindow *parent, wxWindowID id, wxImageList *imgList, wxColour textColor, wxColour backColor);

        /*! \brief Destructor */
        ~ownerDrawnComboBox();

        /*!
         * \brief Fills Combo Box with data.
         * \param data Pointer to a list with data
         */
        void setData(ownerDrawnComboBoxDataList *data);

    private:
        /*! \brief Combo Box data. */
        ownerDrawnComboBoxDataList *m_pData;

        /*! \brief Image list. */
        wxImageList *m_pImgList;

        /*! \brief Text color. */
        wxColour m_textColor;

        /*! \brief Background color. */
        wxColour m_backColor;

        /*! \brief Size of the image. */
        wxSize m_imgSize;

        /*! \brief Image count. */
        int m_imgCount;

        /*!
         * \brief Event handler call when control needs to be repainted
         * \param dc The device context to use for drawing
         * \param rect The bounding rectangle for the item being drawn
         * \param item The index of the item to be drawn
         * \param flags Combines the wxODCB_PAINTING_CONTROL and wxODCB_PAINTING_SELECTED values
         */
        virtual void OnDrawItem(wxDC &dc, const wxRect &rect, int item, int flags) const;

        /*!
         * \brief Event handler call when control's background needs to be repainted
         * \param dc The device context to use for drawing
         * \param rect The bounding rectangle for the item being drawn
         * \param item The index of the item to be drawn
         * \param flags Combines the wxODCB_PAINTING_CONTROL and wxODCB_PAINTING_SELECTED values
         */
        virtual void OnDrawBackground(wxDC &dc, const wxRect &rect, int item, int flags) const;

        /*!
         * \brief Returns the height of the specified item
         * \param item The index of the item
         */
        virtual wxCoord OnMeasureItem(size_t item) const;

        /*!
         * \brief Returns the height of the specified item
         * \param item The index of the item
         */
        virtual wxCoord OnMeasureItemWidth(size_t item) const;
};

#endif // OWNERDRAWNCOMBOBOX_H
