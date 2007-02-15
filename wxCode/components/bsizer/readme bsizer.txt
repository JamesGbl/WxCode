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
