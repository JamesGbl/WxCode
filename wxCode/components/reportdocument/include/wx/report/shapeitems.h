#ifndef SHAPEELEMENTS_H
#define SHAPEELEMENTS_H

#include "wx/report/pageitems.h" // Base class: PageItem
#include "wx/pen.h"

//////////////////////////////// wxReportShapeItem

/**
 * \brief Base class for graphics shape items.
 */
class WXDLLIMPEXP_RP wxReportShapeItem : public wxReportPageItem
{
protected:
	double m_dLineThickness;
	wxColour m_lineColor;
	int m_iLineStyle;
	wxColour m_fillColor;
	
public:
	/**
	 * \brief Default constructor.
	 */
	wxReportShapeItem() {;}
	/**
	 * \brief Virtual destructor. 
	 */
	virtual ~wxReportShapeItem() {;}
	/**
	 * \brief Sets the width of the shape's drawing line.
	 * \param thickness width of the line
	 */
	void SetLineThickness(double thickness);
	/**
	 * \brief Sets the color of the shape's drawing line.
	 * \param lineColor color of the line
	 */
	void SetLineColor(const wxColour &lineColor);
	/**
	 * \brief Sets the style of the shape's drawing line.
	 * \param lineStyle line style
	 */
	void SetLineStyle(int lineStyle = wxSOLID);
	/**
	 * \brief Sets the filling color for the shape. Does nothing for the line shapes.
	 * \param fillColor color to fill
	 */
	void SetFillColor(const wxColour &fillColor);
	/**
	 * \brief Returns the width of the shape's drawing line
	 * \return width of the line
	 */
	const double& GetLineThickness();
	/**
	 * \brief Returns the color of the shape's drawing line.
	 * \return color of the line
	 */
	const wxColour& GetLineColor();
	/**
	 * \brief Returns the style of the shape's drawing line.
	 * \return according value from the wxPenStyle enumeration
	 */
	const int& GetLineStyle();
	/**
	 * \brief Returns the filling color for the shape. Does nothing for the line shapes.
	 * \return filling color, wxNullColour of no filling colour previously set
	 */
	const wxColour& GetFillColor();
};

//////////////////////////////// wxReportLineShape

/**
 * \brief Represents the straight line with specified color.
 */
class WXDLLIMPEXP_RP wxReportLineShape : public wxReportShapeItem
{
protected:
	wxRealPoint m_point1;
	wxRealPoint m_point2;
	
public:
	/**
	 * \brief Default constructor.
	 */
	wxReportLineShape();
	/**
	 * \brief Constructor.
	 * \param x1 horizontal position of the starting point
	 * \param y1 vertical position of the starting point
	 * \param x2 horizontal position of the ending point
	 * \param y2 vertical position of the ending point
	 * \param lineThickness width of the shape's line
	 * \param lineColor color of the shape's line
	 * \param lineStyle style of the shape's line (wxPenStyle enumeration values are used)
	 */
	wxReportLineShape(double x1, double y1, double x2, double y2, double lineThickness = 0.5,
						const wxColour &lineColor = *wxBLACK, int lineStyle = wxSOLID);
	/**
	 * \brief Constructor.
	 * \param point1 starting point of the line
	 * \param point2 ending point of the line
	 * \param lineThickness width of the shape's line
	 * \param lineColor color of the shape's line
	 * \param lineStyle style of the shape's line (wxPenStyle enumeration values)
	 */
	wxReportLineShape(const wxRealPoint& point1, const wxRealPoint& point2, double lineThickness = 0.5,
				const wxColour &lineColor = *wxBLACK, int lineStyle = wxSOLID);
				
	/*!
	 * \brief Default destructor.
	 */
	~wxReportLineShape() {;}
	/**
	 * \brief Sets the coordinates of the line.
	 * \param x1 horizontal position of the starting point
	 * \param y1 vertical position of the starting point
	 * \param x2 horizontal position of the ending point
	 * \param y2 vertical position of the ending point
	 */
	void SetPoints(double x1, double y1, double x2, double y2);
	/**
	 * \brief Sets the coordinates of the line. Coordinates defined by the wxRealPoint objects.
	 * \param point1 starting point of the line
	 * \param point2 ending point of the line
	 */
	void SetPoints(const wxRealPoint& point1, const wxRealPoint& point2);
	/**
	 * \brief Returns the first coordinate of the line.
	 * \return wxRealPoint object with the coordinate
	 */
	const wxRealPoint& GetPoint1();
	/**
	 * \brief Returns the second coordinate of the line.
	 * \return wxRealPoint object with the coordinate
	 */
	const wxRealPoint& GetPoint2();
	/**
	 * \brief Creates the XML node for the shape.
	 * \return XML node with the shape
	 */
	wxXmlNode* CreateXmlNode();
	/**
	 * \brief Retrieves the shape from the XML node.
	 * \param node XML node to retrieve
	 * \return TRUE on success, otherwise FALSE
	 */
	bool RetrieveFromXmlNode(const wxXmlNode* node);
	/**
	 * \brief Draws the shape to the given device context.
	 * \param dc device context to drawing
	 * \param toScreen tells if the sahe will be drawn to the screen or to the printer
	 * \param pageStyle style of the page where the shape will be drawned
	 */
	void DrawToDC(wxDC* dc, bool toScreen, const wxReportPageStyle& pageStyle);
};

//////////////////////////////// wxReportRectangleShape

/**
 * \brief Represents the rectangle shape item.
 */
class WXDLLIMPEXP_RP wxReportRectangleShape : public wxReportShapeItem
{
protected:
	wxRealPoint m_topLeftPoint;
	double m_dWidth;
	double m_dHeight;
	
public:
	/**
	 * \brief Default constructor.
	 */
	wxReportRectangleShape();
	/**
	 * \brief Constructor.
	 * \param x horizontal position of the left top corner
	 * \param y vertical position of the left top corner
	 * \param width width of the rectangle
	 * \param height height of the rectangle
	 * \param lineThickness width of the shape's line
	 * \param lineColor color of the shape's line
	 * \param lineStyle style of the shape's line - wxPenStyle enumeration values
	 * \param fillColor filling color for the shape
	 */
	wxReportRectangleShape(double x, double y, double width, double height, double lineThickness = 0.5,
				   const wxColour &lineColor = *wxBLACK, int lineStyle = wxSOLID,
				   const wxColour &fillColor = wxNullColour);
	/**
	 * \brief Constructor.
	 * \param topLeftCorner top left corner position
	 * \param width width of the rectangle
	 * \param height height of the rectangle
	 * \param lineThickness width of the shape's line
	 * \param lineColor color of the shape's line
	 * \param lineStyle style of the shape's line - wxPenStyle enumeration values
	 * \param fillColor filling color for the shape
	 */
	wxReportRectangleShape(const wxRealPoint& topLeftCorner, double width, double height, double lineThickness = 0.5,
				   const wxColour &lineColor = *wxBLACK, int lineStyle = wxSOLID,
				   const wxColour &fillColor = wxNullColour);
	/**
	 * \brief Default destructor
	 */
	~wxReportRectangleShape() {;}
	/**
	 * \brief Sets the coordinates of the top left corner of the rectangle.
	 * \param x horizontal position of the left top corner
	 * \param y vertical position of the left top corner
	 */
	void SetTopLeftCorner(double x, double y);
	/**
	 * \brief Sets the coordinates of the top left corner of the rectangle.
	 * \param point top left corner position
	 */
	void SetTopLeftCorner(const wxRealPoint& point);
	/**
	 * \brief Sets the width of the rectangle.
	 * \param width width of the rectangle
	 */
	void SetWidth(double width);
	/**
	 * \brief Sets the height of the rectangle.
	 * \param height height of the rectangle
	 */
	void SetHeight(double height);
	/**
	 * \brief Returns the coordinate of the top left corner of the rectangle.
	 * \return wxRealPoint object with the coordinate
	 */
	const wxRealPoint& GetTopLeftCorner();
	/**
	 * \brief Returns the width of the rectangle.
	 * \return width of the rectangle
	 */
	const double& GetWidth();
	/**
	 * \brief Returns the height of the rectangle.
	 * \return height of the rectangle
	 */
	const double& GetHeight();
	/**
	 * \brief Creates the XML node for the shape.
	 * \return XML node with the shape
	 */
	wxXmlNode* CreateXmlNode();
	/**
	 * \brief Retrieves the shape from the XML node.
	 * \param node XML node to retrieve
	 * \return TRUE on success, otherwise FALSE
	 */
	bool RetrieveFromXmlNode(const wxXmlNode* node);
	/**
	 * \brief Draws the shape to the given device context.
	 * \param dc devicew context to drawing
	 * \param toScreen tells if the sahe will be drawn to the screen or to the printer
	 * \param pageStyle sstyle of the page where the shape will be drawned
	 */
	void DrawToDC(wxDC* dc, bool toScreen, const wxReportPageStyle& pageStyle);
};

/**
 * \brief Represents the circle shape item.
 */
class WXDLLIMPEXP_RP wxReportCircleShape : public wxReportShapeItem
{
protected:
	wxRealPoint m_centreCoord;
	double m_dRadius;
	
public:
	/**
	 * \brief Constructor.
	 */
	wxReportCircleShape();
	/**
	 * \brief Constructor.
	 * \param centreX horizontal position of circle's center
	 * \param centreY vertical position of circle's center
	 * \param radius radius of the circle
	 * \param lineThickness width of the shape's line
	 * \param lineColor color of the shape's line
	 * \param lineStyle style of the shape's line - wxPenStyle enumeration values
	 * \param fillColor filling color for the shape
	 */
	wxReportCircleShape(double centreX, double centreY, double radius, double lineThickness = 0.5,
				   const wxColour &lineColor = *wxBLACK, int lineStyle = wxSOLID,
				   const wxColour &fillColor = wxNullColour);
	/**
	 * \brief Constructor.
	 * \param centreCoord position of circle's center
	 * \param radius radius of the circle
	 * \param lineThickness width of the shape's line
	 * \param lineColor color of the shape's line
	 * \param lineStyle style of the shape's line - wxPenStyle enumeration values
	 * \param fillColor filling color for the shape
	 */
	wxReportCircleShape(const wxRealPoint& centreCoord, double raius, double lineThickness = 0.5,
				   const wxColour &lineColor = *wxBLACK, int lineStyle = wxSOLID,
				   const wxColour &fillColor = wxNullColour);
	/**
	 * \brief Default destructor
	 */
	~wxReportCircleShape() {;} 
	/**
	 * \brief Sets the position of the circle's center
	 * \param coordX horizontal position
	 * \param coordY vertical position
	 */
	void SetCentreCoord(double coordX, double coordY);
	/**
	 * \brief Sets the radius of the circle.
	 * \param r radius
	 */
	void SetRadius(double r);
	/**
	 * \brief Returns the coordinates of the circle's center
	 * \return wxRealPoint object with the coordinates
	 */
	const wxRealPoint& GetCentreCoord();
	/**
	 * \brief Return the circle's radius.
	 * \return circle radius
	 */
	const double& GetRadius();
	/**
	 * \brief Creates the XML node for the shape.
	 * \return XML node with the shape
	 */
	wxXmlNode* CreateXmlNode();
	/**
	 * \brief Retrieves the shape from the XML node.
	 * \param node XML node to retrieve
	 * \return TRUE on success, otherwise FALSE
	 */
	bool RetrieveFromXmlNode(const wxXmlNode* node);
	/**
	 * \brief Draws the shape to the given device context.
	 * \param dc devicew context to drawing
	 * \param toScreen tells if the sahe will be drawn to the screen or to the printer
	 * \param pageStyle style of the page where the shape will be drawned
	 */
	void DrawToDC(wxDC* dc, bool toScreen, const wxReportPageStyle& pageStyle);
};

#endif // SHAPEELEMENTS_H