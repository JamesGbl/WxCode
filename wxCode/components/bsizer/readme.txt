A border sizer lays out a container, arranging and resizing its components to 
fit in five regions: North, South, East, West, and Center. When adding a 
component to a container with a border sizer, use one of these five names, 
for example: 
    wxPanel *panel = new wxPanel(parent, -1);
    wxBorderSizer *bsizer = new wxBorderSizer();
    bsizer->add(new wxButton(panel, -1, "Button"), 
                0, wxALL, 10, wxBorderItemHandle("North"));
    panel->SetAutoLayout(TRUE);
    panel->SetSizer(bsizer);
 
The components are laid out according to their sizes and the constraints of 
the container's size. The North and South components may be stretched horizontally; 
the East and West components may be stretched vertically; the Center component 
may stretch both horizontally and vertically to fill any space left over. 

See bsizer.gif

----------------------------------------------------------------------------------
The wxGridBagSizer class is a flexible layout manager that aligns components 
vertically and horizontally, without requiring that the components be of the 
same size. Each wxGridBagSizer object maintains a dynamic rectangular grid of 
cells, with each component occupying one or more cells, called its display area. 
Each component managed by a grid bag layout is associated with an instance 
of wxGridBagItemHandle that specifies how the component is laid out within its 
display area. 

How a wxGridBagSizer object places a set of components depends on the wxGridBagItemHandle 
object associated with each component. 

To use a grid bag layout effectively, you must customize one or more of the 
wxGridBagItemHandle objects that are associated with its components. You customize 
a wxGridBagItemHandle object by setting its instance variables: 


x, y 
Specifies the cell at the upper left of the component's display area, where the
upper-left-most cell has address x = 0, y = 0. 

width, height 
Specifies the number of cells in a row (for width) or column (for height) in the 
component's display area.  

Files:
// wxGridBagSizer
gbsizer.h
gbsizer.cpp

// Sample
gbtest.cpp
gbtest.rc
gbsizer.gif
makefile.vc
