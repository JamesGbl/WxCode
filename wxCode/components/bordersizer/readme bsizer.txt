A border sizer lays out a container, arranging and resizing its components to 
fit in five regions: North, South, East, West, and Center. (See bsizer.gif)

for example: 
    wxPanel *panel = new wxPanel(parent, wxID_ANY);
    wxBorderSizer *bsizer = new wxBorderSizer();

    bsizer->Add(new wxButton(panel, wxID_ANY, wxT("TOP BUTTON")), 
                0, wxALL, 10, new wxBorderItemHandle(wxNORTH));

    bsizer->Add(new wxButton(panel, wxID_ANY, wxT("BOTTOM BUTTON")), 
                0, wxALL, 10, new wxBorderItemHandle(wxSOUTH));

    panel->SetAutoLayout(TRUE);
    panel->SetSizer(bsizer);
 
The components are laid out according to their sizes and the constraints of 
the container's size. The North and South components may be stretched horizontally; 
the East and West components may be stretched vertically; the Center component 
may stretch both horizontally and vertically to fill any space left over. 

Acceptable values:
wxNORTH, wxEAST, wxWEST, wxSOUTH, wxCENTER, wxCENTRE

To install : simply add the header and source file to your project.