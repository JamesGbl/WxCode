#ifndef DEFS_H
#define DEFS_H

#ifdef USING_SOURCE_RP
	#define WXDLLIMPEXP_RP
    #define WXDLLIMPEXP_DATA_RP(type) type
#elif defined( LIB_USINGDLL )
    #define WXDLLIMPEXP_RP
    #define WXDLLIMPEXP_DATA_RP(type)
#elif defined( WXMAKINGDLL_WXRP )
    #define WXDLLIMPEXP_RP WXEXPORT
    #define WXDLLIMPEXP_DATA_RP(type) WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_RP WXIMPORT
    #define WXDLLIMPEXP_DATA_RP(type) WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_RP
    #define WXDLLIMPEXP_DATA_RP(type) type
#endif

#define FRIENDS friend class wxReportPage;

enum ItemsTypes
{
	wxRP_TEXT,
	wxRP_PARAGRAPH,
	wxRP_TEXTVALUE,
	wxRP_IMAGE,
	wxRP_TABLE,
	wxRP_TABLECELL,
	wxRP_LINESHAPE,
	wxRP_RECTANGLESHAPE,
	wxRP_CIRCLESHAPE
};

enum SourcesTypes
{
	wxRP_WXARRAYSHORT,
	wxRP_WXARRAYINT,
	wxRP_WXARRAYLONG,
	wxRP_WXARRAYDOUBLE,
	wxRP_WXARRAYSTRING,
	wxRP_ARRAYSHORT,
	wxRP_ARRAYINT,
	wxRP_ARRAYLONG,
	wxRP_ARRAYFLOAT,
	wxRP_ARRAYDOUBLE,
};

// Styles constants
enum SidesValues
{
	wxRP_NOBORDER 	= 0,//0b0000,
	wxRP_LEFTBORDER 	= 1,//0b0001,
	wxRP_RIGHTBORDER 	= 2,//0b0010,
	wxRP_TOPBORDER		= 4,//0b0100,
	wxRP_BOTTOMBORDER	= 8,//0b1000,
	wxRP_ALLBORDER		= 15,//0b1111,
	wxRP_ALLINBORDER	= 16//0b10000 // for tables
};

enum TextAlign
{
	wxRP_LEFTALIGN = 0,
	wxRP_CENTERALIGN = 1,
	wxRP_RIGHTALIGN = 2,
	wxRP_JUSTIFY = 3
};

enum wxReportPageParts
{
	wxRP_BODY,
	wxRP_HEADER,
	wxRP_FOOTER
};

enum VariableTypes
{
	wxRP_SHORT,
	wxRP_INT,
	wxRP_LONG,
	wxRP_FLOAT,
	wxRP_DOUBLE,
	wxRP_WXSTRING
};

enum Units
{
	wxRP_MM,
	wxRP_INCH
};

enum PositionsArray
{
	wxRP_AT_LAST = -1
};

enum wxReportPositions
{
	wxRP_LEFT = 10000,
	wxRP_CENTER = 10001,
	wxRP_RIGHT = 10002,
	wxRP_TOP = 10003,
	wxRP_BOTTOM = 10004
};

/**
 * //\brief Encapsulates inche to milimeter ratio for the size to pixels transform opeartions.
 */
/*class wxReportUnit
{
private:
	static double m_unitTypeRatio;
	
public:
	static void SetUnitsRatio(int unitType)
	{
		if(unitType == wxRP_INCH)
			m_unitTypeRatio = 1.0;
			
		if(unitType == wxRP_MM)
			m_unitTypeRatio = 25.4;
	}
	inline static double GetUnitsRatio()
	{
		return m_unitTypeRatio;
	}
};*/

#endif // DEFS_H
