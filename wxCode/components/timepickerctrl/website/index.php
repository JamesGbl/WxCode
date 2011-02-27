<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
   <body>
      <br/>
      <H1 align="center">TimePickerCtrl version 0.2</H1>
      <H2>Screenshots</H2>
         <table width="100%">
            <tr><td width="50%">
                  <img src="image_win.png" alt="image_win.png"/>
               	<p>TimePickerCtrl on Windows</p>
            </td><td width="50%">
                  <img src="image_linux.png" alt="image_linux.png"/>
                  <p>TimePickerCtrl on Linux</p>
            </td></tr>
         </table>
  <div class="center">
  </div>
   <br/>
   <H2>Usage sample</H2>
      <p><pre>
         #include "timepickerctrl.h"
         ...
         class MyWindow : public wxFrame{
         public:
            MyWindow(...);
            ...
            void OnTimePicker(wxCommandEvent& event); // Handler for TimePickerCtrl changed events
            ...
            TimePickerCtrl m_timepicker;
            ...
         };
         ...
         BEGIN_EVENT_TABLE(MyWindow,wxFrame)
            ...
            EVT_TIMEPICKER(TimePickerCtrlFrame::OnTimePicker)
            ...
         END_EVENT_TABLE()
         ...
         MyWindow::MyWindow(...){
            ...
            m_timepicker = new TimePickerCtrl(this,ID_CUSTOM1,wxEmptyString,wxDefaultPosition,wxSize(118,45),0);
            ...
         }
         ...
         void Mywindow::OnTimePicker(wxCommandEvent& event){
            // Add code here to handle events triggered by the TimePickerCtrl
            ...
         }
         ...</pre>
  <br/><H2>Documentation</H2>
  <p>The documentation for this component is available online
  <a href="../docs/html/">here</a>.</p>


	<br/><H2>Known bugs</H2>
  <p>None. To submit a bug report please look at <a href="../../support.php">wxCode support page</a>.</p>

    
	<br/><H2>Future enhancements</H2>
  <p>Currently the time format is fixed. Depending on public interest and my available time, I may implement customization.  To submit a feature request please look at <a href="../../support.php">wxCode support page</a>.</p>
</body>
</html>
