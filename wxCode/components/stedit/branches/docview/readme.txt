docview:
The idea was to carve out 'common' code from the frame class.
wxDocManager can handle opening and closing files, printing, filehistory, things
that wxStEdit also implements. But it turns out that this leads nowhere as there
are nothing that can be taken out. The editor can be used as a standalone widget
and this unfortunately rules out removing any code, it is not natural to make
the editor widget contain an wxDocManager instance, it belongs with the frame.
And putting wxDocManager in the frame gives no benefits really, just duplication
of code.

docview.patch: the docview code is enclosed with #if STEDIT_DOCVIEW
