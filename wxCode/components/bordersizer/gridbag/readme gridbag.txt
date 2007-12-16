Note : this component was included along bordersizer by its original author. it has not been updated by the new manager
___________
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
