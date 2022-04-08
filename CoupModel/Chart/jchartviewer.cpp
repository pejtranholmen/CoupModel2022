#include "jchartviewer.h"
#include <time.h>
#include "../JuceLibraryCode/JuceHeader.h"
/*#include <MouseEvent>
#include <QToolTip>
#include <QCursor>
#include <Bitmap>
#include <File>*/

/////////////////////////////////////////////////////////////////////////////
//
// QChartViewer
//
/////////////////////////////////////////////////////////////////////////////


//
// Build in mouse cursors for zooming and scrolling support
//
/*
static hcurs &getZoomInCursor();
static QCursor &getZoomOutCursor();
static QCursor &getNoZoomCursor();
static QCursor &getNoMove2DCursor();
static QCursor &getNoMoveHorizCursor();
static QCursor &getNoMoveVertCursor();
  */
//
// Constants used in m_delayChartUpdate
//
enum { NO_DELAY, NEED_DELAY, NEED_UPDATE };

JChartViewer::JChartViewer(String txt) :
    Label(txt)
{
    // current chart and hot spot tester
    m_currentChart = 0;
    m_hotSpotTester = 0;

    // initialize chart configuration
    m_selectBoxLineColor = Colour(0, 0, 0);
    m_selectBoxLineWidth = 2;
    m_mouseUsage = Chart::MouseUsageDefault;
    m_zoomDirection = Chart::DirectionHorizontal;
    m_zoomInRatio = 2;
    m_zoomOutRatio = 0.5;
    m_mouseWheelZoomRatio = 1;
    m_scrollDirection = Chart::DirectionHorizontal;
    m_minDragAmount = 5;
    m_updateInterval = 20;

    // current state of the mouse
    m_isOnPlotArea = false;
    m_isPlotAreaMouseDown = false;
    m_isDragScrolling = false;
    m_currentHotSpot = -1;
    m_isClickable = false;
    m_isMouseTracking = false;
    m_isInMouseMove = false;

    // chart update rate support
    m_needUpdateChart = false;
    m_needUpdateImageMap = false;
    m_holdTimerActive = false;
    m_isInViewPortChanged = false;
    m_delayUpdateChart = NO_DELAY;
    m_delayedChart = 0;
    m_lastMouseMove = 0;
    m_delayedMouseEvent = 0;
    m_delayImageMapUpdate = false;

    // track cursor support
    m_autoHideMsg = "";
    m_currentMouseX = -1;
    m_currentMouseY = -1;
    m_isInMouseMovePlotArea = false;

    // selection rectangle
    m_LeftLine = nullptr;
    m_RightLine = 0;
    m_TopLine = 0;
    m_BottomLine = 0;

   // setMouseTracking(true);
}

JChartViewer::~JChartViewer()
{
    delete m_hotSpotTester;
    delete m_delayedMouseEvent;
}

namespace {
   /* static class QtResourceLoader {
    public:
        QtResourceLoader() { Chart::setResourceLoader(loader); }
        static bool loader(const char *id, char *(allocator)(int), char **data, int *len) {
            File f(String(":/") + id);
            return f.open(QIODevice::ReadOnly) && (0 < (*len = f.size())) &&
                (0 != (*data = allocator(*len))) && f.read(*data, *len);
        }
    } QtResourceLoaderRegistration;*/
}

/////////////////////////////////////////////////////////////////////////////
// QChartViewer message handlers

//
// MouseMove event handler
//
/*
void QChartViewer::mouseMoveEvent(MouseEvent *event)
{
    // Enable mouse tracking to detect mouse leave events
    m_isMouseTracking = true;

    emit mouseMove(event);

    // On Windows, mouse events can by-pass the event queue. If there are too many mouse events,
    // the event queue may not get processed, preventing other controls from updating. If two mouse
    // events are less than 10ms apart, there is a risk of too many mouse events. So we repost the
    // mouse event as a timer event that is queued up normally, allowing the queue to get processed.
    unsigned int timeBetweenMouseMove = (((unsigned int)clock()) - m_lastMouseMove)
                                        * 1000 / CLOCKS_PER_SEC ;
    if ((m_delayedMouseEvent && (timeBetweenMouseMove < 250)) || (timeBetweenMouseMove < 10))
    {
        if (!m_delayedMouseEvent)
            m_delayedMouseEventTimerId = startTimer(1);
        else
            delete m_delayedMouseEvent;

        m_delayedMouseEvent = new QMouseEvent(event->type(), event->pos(), event->globalPos(),
            event->button(), event->buttons(), event->modifiers());
    }
    else
        commitMouseMove(event);

    onSetCursor();
}*/

//
// The method that actually performs MouseMove event processing
//
void JChartViewer::commitMouseMove(MouseEvent *event)
{
    // Remember the mouse coordinates for later use
    m_currentMouseX = event->getScreenX();
    m_currentMouseY = event->getScreenY();

    // The chart can be updated more than once during mouse move. For example, it can update due to
    // drag to scroll, and also due to drawing track cursor. So we delay updating the display until
    // all all events has occured.
    m_delayUpdateChart = NEED_DELAY;
    m_isInMouseMove = true;

    // Check if mouse is dragging on the plot area
    m_isOnPlotArea = m_isPlotAreaMouseDown || inPlotArea(event->getScreenX(), event->getScreenY());
    if (m_isPlotAreaMouseDown)
        onPlotAreaMouseDrag(event);

    // Emit mouseMoveChart
  //  emit mouseMoveChart(event);

    if (inExtendedPlotArea(event->getScreenX(), event->getScreenY()))
    {
        // Mouse is in extended plot area, emit mouseMovePlotArea
        m_isInMouseMovePlotArea = true;
    //    emit mouseMovePlotArea(event);
    }
    else if (m_isInMouseMovePlotArea)
    {
        // Mouse was in extended plot area, but is not in it now, so emit mouseLeavePlotArea
        m_isInMouseMovePlotArea = false;
      //  emit mouseLeavePlotArea(event);
        applyAutoHide("mouseleaveplotarea");
    }

    //
    // Show hot spot tool tips if necessary
    // (Due to QT bug, tooltip cannot be put in delayedMouseEvent, otherwise sometimes tooltip
    // position will be incorrect.)
    //

    if (m_delayImageMapUpdate)
    {
        m_delayImageMapUpdate = false;
        if (!m_isPlotAreaMouseDown)
            updateViewPort(false, true);
    }

    //if (event->buttons() != Qt::NoButton)
    //{
        // Hide tool tips if mouse button is pressed.
      //  QToolTip::hideText();
    //}
    //else
    
        // Use the ChartDirector ImageMapHandler to determine if the mouse is over a hot spot
        int hotSpotNo = 0;
        if (0 != m_hotSpotTester)
            hotSpotNo = m_hotSpotTester->getHotSpot(event->getScreenX(), event->getScreenY());

        // If the mouse is in the same hot spot since the last mouse move event, there is no need
        // to update the tool tip.
        if (hotSpotNo != m_currentHotSpot)
        {
            // Hot spot has changed - update tool tip text
            m_currentHotSpot = hotSpotNo;

            if (hotSpotNo == 0)
            {
                // Mouse is not actually on hanlder hot spot - use default tool tip text and reset
                // the clickable flag.
               // QToolTip::showText(event->globalPos(), m_defaultToolTip);
                m_isClickable = false;
            }
            else
            {
                // Mouse is on a hot spot. In this implementation, we consider the hot spot as
                // clickable if its href ("path") parameter is not empty.
                const char *path = m_hotSpotTester->getValue("path");
                m_isClickable = ((0 != path) && (0 != *path));
                //QToolTip::showText(event->globalPos(), QString::fromUtf8(m_hotSpotTester->getValue("title")));
            }
      
    }

    // Cancel the delayed mouse event if any
    if (m_delayedMouseEvent)
    {
       // killTimer(m_delayedMouseEventTimerId);
        delete m_delayedMouseEvent;
        m_delayedMouseEvent = 0;
    }

    // Can update chart now
    commitUpdateChart();
    m_isInMouseMove = false;

    m_lastMouseMove = (unsigned int)clock();
}

//
// Delayed MouseMove event handler
//
void JChartViewer::onDelayedMouseMove()
{
    if (m_delayedMouseEvent)
        commitMouseMove(m_delayedMouseEvent);
}

void JChartViewer::leaveEvent(MouseEvent *event)
{
    // Process delayed mouse move, if any
    onDelayedMouseMove();

    // Mouse tracking is no longer active
    m_isMouseTracking = false;

    if (m_isInMouseMovePlotArea)
    {
        // Mouse was in extended plot area, but is not in it now, so emit mouseLeavePlotArea
        m_isInMouseMovePlotArea = false;
      //  emit mouseLeavePlotArea(event);
        applyAutoHide("mouseleaveplotarea");
    }

    // emit mouseLeaveChart
    //emit mouseLeaveChart(event);
    applyAutoHide("mouseleavechart");
}

//
// MouseWheel handler
//
void JChartViewer::wheelEvent(MouseEvent *event)
{
  /*  bool hasReceivers = receivers(SIGNAL(mouseWheel(QWheelEvent *))) > 0;
    if (hasReceivers)
    {
        // Process delayed mouse move, if any
        onDelayedMouseMove();

        // emit mouseWheel event
        emit mouseWheel(event);
    }

    // Process the mouse wheel only if the mouse is over the plot area
    bool hasMouseWheelZoom = isMouseOnPlotArea() &&
        onMouseWheelZoom(getPlotAreaMouseX(), getPlotAreaMouseY(), event->delta());

    if (!(hasReceivers || hasMouseWheelZoom))
        event->ignore();    */
}

bool JChartViewer::onMouseWheelZoom(int x, int y, int zDelta)
{
    // Zoom ratio = 1 means no zooming
    if (m_mouseWheelZoomRatio == 1)
        return false;

    // X and Y zoom ratios
    double rx = 1;
    double ry = 1;
    if (getZoomDirection() != Chart::DirectionVertical)
        rx = (zDelta > 0) ? m_mouseWheelZoomRatio : 1 / m_mouseWheelZoomRatio;
    if (getZoomDirection() != Chart::DirectionHorizontal)
        ry = (zDelta > 0) ? m_mouseWheelZoomRatio : 1 / m_mouseWheelZoomRatio;

    // Do the zooming
    if (zoomAround(x, y, rx, ry))
        updateViewPort(true, true);

    return true;
}

//
// Change the mouse cursor.
//
void JChartViewer::onSetCursor()
{
    if (this->m_isDragScrolling)
    {
        switch (m_scrollDirection)
        {
        case Chart::DirectionHorizontal:
      //      setCursor(getNoMoveHorizCursor());
            break;
        case Chart::DirectionVertical:
       //     setCursor(getNoMoveVertCursor());
            break;
        default :
        //    setCursor(getNoMove2DCursor());
            break;
        }

        return;
    }

    if (m_isOnPlotArea)
    {
        switch (m_mouseUsage)
        {
        case Chart::MouseUsageZoomIn:
            if (canZoomIn(m_zoomDirection))
              //  setCursor(getZoomInCursor());
            //else
                //setCursor(getNoZoomCursor());
            return;
        case Chart::MouseUsageZoomOut:
            if (canZoomOut(m_zoomDirection))
              //  setCursor(getZoomOutCursor());
            //else
              //  setCursor(getNoZoomCursor());
            return;
        }
    }

    //if (m_isClickable)
       // setCursor(Qt::PointingHandCursor);
    //else
      //  unsetCursor();
}

//
// Mouse button down event.
//

void JChartViewer::mouseMoveEvent(MouseEvent& event)
{
    onDelayedMouseMove();

    //if ((event->button() == Qt::LeftButton) && inPlotArea(event->x(), event->y()) &&
    //    (m_mouseUsage != Chart::MouseUsageDefault))
    //{
        // Mouse usage is for drag to zoom/scroll. Capture the mouse to prepare for dragging and
        // save the mouse down position to draw the selection rectangle.
    m_isPlotAreaMouseDown = true;
    m_plotAreaMouseDownXPos = event.getScreenX();
    m_plotAreaMouseDownYPos = event.getScreenY();
    startDrag();
    //}
}

void JChartViewer::mousePressEvent(MouseEvent &event)
{
    onDelayedMouseMove();

    //if ((event->button() == Qt::LeftButton) && inPlotArea(event->x(), event->y()) &&
    //    (m_mouseUsage != Chart::MouseUsageDefault))
    //{
        // Mouse usage is for drag to zoom/scroll. Capture the mouse to prepare for dragging and
        // save the mouse down position to draw the selection rectangle.
        m_isPlotAreaMouseDown = true;
        m_plotAreaMouseDownXPos = event.getScreenX();
        m_plotAreaMouseDownYPos = event.getScreenY();
        startDrag();
    //}
}

//
// Mouse button up event.
//
void JChartViewer::mouseReleaseEvent(MouseEvent &event)
{
 /*   onDelayedMouseMove();

    if ((event.mods.isRightButtonDown()) && m_isPlotAreaMouseDown)
    {
        // Release the mouse capture.
        m_isPlotAreaMouseDown = false;
        setRectVisible(false);
        bool hasUpdate = false;

        switch (m_mouseUsage)
        {
        case Chart::MouseUsageZoomIn :
            if (canZoomIn(m_zoomDirection))
            {
                if (isDrag(m_zoomDirection, event))
                    // Zoom to the drag selection rectangle.
                    hasUpdate = zoomTo(m_zoomDirection,
                        m_plotAreaMouseDownXPos, m_plotAreaMouseDownYPos, event->x(), event->y());
                else
                    // User just click on a point. Zoom-in around the mouse cursor position.
                    hasUpdate = zoomAt(m_zoomDirection, event->x(), event->y(), m_zoomInRatio);
            }
            break;
        case Chart::MouseUsageZoomOut:
            // Zoom out around the mouse cursor position.
            if (canZoomOut(m_zoomDirection))
                hasUpdate = zoomAt(m_zoomDirection, event->x(), event->y(), m_zoomOutRatio);
            break;
        default :
            if (m_isDragScrolling)
                // Drag to scroll. We can update the image map now as scrolling has finished.
                updateViewPort(false, true);
            else
                // Is not zooming or scrolling, so is just a normal click event.
                emit clicked(event);
            break;
        }

        m_isDragScrolling = false;
        if (hasUpdate)
            // View port has changed - update it.
            updateViewPort(true, true);
    }
    else
        emit clicked(event);

    onSetCursor();*/
}

//
// Chart hold timer.
//
/*
void QChartViewer::timerEvent(QTimerEvent *event)
{
    if (m_delayedMouseEvent && (event->timerId() == m_delayedMouseEventTimerId))
    {
        // Is a delayed mouse move event
        onDelayedMouseMove();
    }
    else if (m_holdTimerActive && (event->timerId() == m_holdTimerId))
    {
        killTimer(m_holdTimerId);
        m_holdTimerActive = false;

        // If has pending chart update request, handles them now.
        if (m_needUpdateChart || m_needUpdateImageMap)
            updateViewPort(m_needUpdateChart, m_needUpdateImageMap);
    }
}
*/
/////////////////////////////////////////////////////////////////////////////
// QChartViewer properties

//
// Set the chart to the control
//
void JChartViewer::setChart(BaseChart *c)
{
    m_currentChart = c;
    setImageMap(0);

	
    if (c!=nullptr) c->makeChart();

    updateDisplay();
}

//
// Get back the same BaseChart pointer provided by the previous setChart call.
//
BaseChart *JChartViewer::getChart()
{
    return m_currentChart;
}

//
// Set image map used by the chart
//
void JChartViewer::setImageMap(const char *imageMap)
{
    //delete the existing ImageMapHandler
    if (0 != m_hotSpotTester)
        delete m_hotSpotTester;
    m_currentHotSpot = -1;
    m_isClickable = false;

    //create a new ImageMapHandler to represent the image map
    if ((0 == imageMap) || (0 == *imageMap))
        m_hotSpotTester = 0;
    else
        m_hotSpotTester = new ImageMapHandler(imageMap);
}

//
// Get the image map handler for the chart
//
ImageMapHandler *JChartViewer::getImageMapHandler()
{
    return m_hotSpotTester;
}

//
// Set the default tool tip to use
//
void JChartViewer::setDefaultToolTip(String text)
{
    m_defaultToolTip = text;
}

//
// Set the border width of the selection box
//
void JChartViewer::setSelectionBorderWidth(int width)
{
    m_selectBoxLineWidth = width;
}

//
// Get the border with of the selection box.
//
int JChartViewer::getSelectionBorderWidth()
{
    return m_selectBoxLineWidth;
}

//
// Set the border color of the selection box
//
void JChartViewer::setSelectionBorderColor(Colour c)
{
    m_selectBoxLineColor = c;
    if (m_TopLine != 0)
    {
    /*    QPalette p(c, c);
        m_TopLine->setPalette(p);
        m_LeftLine->setPalette(p);
        m_BottomLine->setPalette(p);
        m_RightLine->setPalette(p);*/
    }
}

//
// Get the border color of the selection box.
//
Colour JChartViewer::getSelectionBorderColor()
{
    return m_selectBoxLineColor;
}

//
// Set the mouse usage mode
//
void JChartViewer::setMouseUsage(int mouseUsage)
{
    m_mouseUsage = mouseUsage;
}

//
// Get the mouse usage mode
//
int JChartViewer::getMouseUsage()
{
    return m_mouseUsage;
}

//
// Set the zoom direction
//
void JChartViewer::setZoomDirection(int direction)
{
    m_zoomDirection = direction;
}

//
// Get the zoom direction
//
int JChartViewer::getZoomDirection()
{
    return m_zoomDirection;
}

//
// Set the scroll direction
//
void JChartViewer::setScrollDirection(int direction)
{
    m_scrollDirection = direction;
}

//
// Get the scroll direction
//
int JChartViewer::getScrollDirection()
{
    return m_scrollDirection;
}

//
// Set the zoom-in ratio for mouse click zoom-in
//
void JChartViewer::setZoomInRatio(double ratio)
{
    m_zoomInRatio = ratio;
}

//
// Get the zoom-in ratio for mouse click zoom-in
//
double JChartViewer::getZoomInRatio()
{
    return m_zoomInRatio;
}

//
// Set the zoom-out ratio
//
void JChartViewer::setZoomOutRatio(double ratio)
{
    m_zoomOutRatio = ratio;
}

//
// Get the zoom-out ratio
//
double JChartViewer::getZoomOutRatio()
{
    return m_zoomOutRatio;
}

//
// Set the mouse wheel zoom ratio 
//
void JChartViewer::setMouseWheelZoomRatio(double ratio)
{
	m_mouseWheelZoomRatio = ratio;
}

//
// Get the mouse wheel zoom ratio 
//
double JChartViewer::getMouseWheelZoomRatio()
{
	return m_mouseWheelZoomRatio;	
}

//
// Set the minimum mouse drag before the dragging is considered as real. This is to avoid small
// mouse vibrations triggering a mouse drag.
//
void JChartViewer::setMinimumDrag(int offset)
{
    m_minDragAmount = offset;
}

//
// Get the minimum mouse drag before the dragging is considered as real.
//
int JChartViewer::getMinimumDrag()
{
    return m_minDragAmount;
}

//
// Set the minimum interval between ViewPortChanged events. This is to avoid the chart being
// updated too frequently. (Default is 20ms between chart updates.) Multiple update events
// arrived during the interval will be merged into one chart update and executed at the end
// of the interval.
//
void JChartViewer::setUpdateInterval(int interval)
{
    m_updateInterval = interval;
}

//
// Get the minimum interval between ViewPortChanged events.
//
int JChartViewer::getUpdateInterval()
{
    return m_updateInterval;
}

//
// Check if there is a pending chart update request.
//
bool JChartViewer::needUpdateChart()
{
    return m_needUpdateChart;
}

//
// Check if there is a pending image map update request.
//
bool JChartViewer::needUpdateImageMap()
{
    return m_needUpdateImageMap;
}

//
// Get the current mouse x coordinate when used in a mouse move event handler
//
int JChartViewer::getChartMouseX()
{
    int ret = m_currentMouseX;
    if (ret < 0)
        ret = getPlotAreaLeft() + getPlotAreaWidth();
    return ret;
}

//
// Get the current mouse y coordinate when used in a mouse move event handler
//
int JChartViewer::getChartMouseY()
{
    int ret = m_currentMouseY;
    if (ret < 0)
        ret = getPlotAreaTop() + getPlotAreaHeight();
    return ret;
}

//
// Get the current mouse x coordinate bounded to the plot area when used in a mouse event handler
//
int JChartViewer::getPlotAreaMouseX()
{
    int ret = getChartMouseX();
    if (ret < getPlotAreaLeft())
        ret = getPlotAreaLeft();
    if (ret > getPlotAreaLeft() + getPlotAreaWidth())
        ret = getPlotAreaLeft() + getPlotAreaWidth();
    return ret;
}

//
// Get the current mouse y coordinate bounded to the plot area when used in a mouse event handler
//
int JChartViewer::getPlotAreaMouseY()
{
    int ret = getChartMouseY();
    if (ret < getPlotAreaTop())
        ret = getPlotAreaTop();
    if (ret > getPlotAreaTop() + getPlotAreaHeight())
        ret = getPlotAreaTop() + getPlotAreaHeight();
    return ret;
}

//
// Check if mouse is on the extended plot area
//
bool JChartViewer::isMouseOnPlotArea()
{
    if (m_isMouseTracking)
        return inExtendedPlotArea(getChartMouseX(), getChartMouseY());
    else
        return false;
}

//
// Check if mouse is dragging to scroll or to select the zoom rectangle
//
bool JChartViewer::isMouseDragging()
{
    return m_isPlotAreaMouseDown;
}

//
// Check if is currently processing a mouse move event
//
bool JChartViewer::isInMouseMoveEvent()
{
    return m_isInMouseMove;
}

//
// Check if is currently processing a view port changed event
//
bool JChartViewer::isInViewPortChangedEvent()
{
    return m_isInViewPortChanged;
}

/////////////////////////////////////////////////////////////////////////////
// QChartViewer methods

//
// Update the display
//
void JChartViewer::updateDisplay()
{
    if (m_delayUpdateChart == NO_DELAY)
        commitUpdateChart();
    else
    {
        m_delayUpdateChart = NEED_UPDATE;
        delete m_delayedChart;
        m_delayedChart = (0 != m_currentChart) ? new BaseChart(m_currentChart) : 0;
    }
}

//
// Commit chart to display
//
void JChartViewer::commitUpdateChart()
{
    if (m_delayUpdateChart == NEED_DELAY)
    {
        // No actual update occur
        m_delayUpdateChart = NO_DELAY;
        return;
    }

    // Get the image and metrics for the chart
    BaseChart *c = (m_delayUpdateChart == NEED_UPDATE) ? m_delayedChart : m_currentChart;
	/*Image i;
	auto koll = i.BitmapData;
    const char *chartMetrics = 0;

    if (0 != c)
    {
        // Output chart as Device Indpendent Bitmap with file headers
        MemBlock m = c->makeChart(Chart::QTIMG);
        image.loadFromData((uchar *)m.data, (uint)m.len);
        setFixedSize(c->getDrawArea()->getWidth(), c->getDrawArea()->getHeight());

        // Get chart metrics
        chartMetrics = c->getChartMetrics();
    }

    // Set the QPixmap for display
    setPixmap(image);

    // Set the chart metrics and clear the image map
    setChartMetrics(chartMetrics);

    // Any delayed chart has been committed
    m_delayUpdateChart = NO_DELAY;
    delete m_delayedChart;
    m_delayedChart = 0;*/
}

//
// Set the message used to remove the dynamic layer
//
void JChartViewer::removeDynamicLayer(const char *msg)
{
    m_autoHideMsg = String(msg ? msg : "").toLowerCase();
    if (m_autoHideMsg == "now")
        applyAutoHide(msg);
}

//
// Attempt to hide the dynamic layer using the specified message
//
void JChartViewer::applyAutoHide(const char *msg)
{
    if (m_autoHideMsg == msg)
    {
        if (0 != m_currentChart)
            m_currentChart->removeDynamicLayer();
        m_autoHideMsg = "";
        updateDisplay();
    }
}

//
// Create the edges for the selection rectangle
//
void JChartViewer::initRect()
{
   // m_LeftLine = new Label(this);
    //m_LeftLine->setAutoFillBackground(true);
    //m_RightLine = new Label(this);
    //m_RightLine->setAutoFillBackground(true);
    //m_TopLine = new Label(this);
    //m_TopLine->setAutoFillBackground(true);
    //m_BottomLine = new Label(this);
    //m_BottomLine->setAutoFillBackground(true);

    setSelectionBorderColor(getSelectionBorderColor());
}

//
// Set selection rectangle position and size
//
void JChartViewer::drawRect(int x, int y, int width, int height)
{
    // Create the edges of the rectangle if not already created
    if (m_TopLine == 0)
        initRect();

    // width < 0 is interpreted as the rectangle extends to the left or x.
    // height <0 is interpreted as the rectangle extends to above y.
    if (width < 0)
        x -= (width = -width);
    if (height < 0)
        y -= (height = -height);

    // Put the edges along the sides of the rectangle
/*    m_TopLine->move(x, y);
    m_TopLine->setFixedSize(width, m_selectBoxLineWidth);
    m_LeftLine->move(x, y);
    m_LeftLine->setFixedSize(m_selectBoxLineWidth, height);
    m_BottomLine->move(x, y + height - m_selectBoxLineWidth + 1);
    m_BottomLine->setFixedSize(width, m_selectBoxLineWidth);
    m_RightLine->move(x + width - m_selectBoxLineWidth + 1, y);
    m_RightLine->setFixedSize(m_selectBoxLineWidth, height);*/
}

//
// Show/hide selection rectangle
//
void JChartViewer::setRectVisible(bool b)
{
    // Create the edges of the rectangle if not already created
    if (b && (m_TopLine == 0))
        initRect();

    // Show/hide the edges
    if (m_TopLine != 0)
    {
        m_TopLine->setVisible(b);
        m_LeftLine->setVisible(b);
        m_BottomLine->setVisible(b);
        m_RightLine->setVisible(b);
    }
}

//
// Determines if the mouse is dragging.
//
bool JChartViewer::isDrag(int direction, MouseEvent *event)
{
    // We only consider the mouse is dragging it is has dragged more than m_minDragAmount. This is
    // to avoid small mouse vibrations triggering a mouse drag.
    int spanX = abs(event->getScreenX() - m_plotAreaMouseDownXPos);
    int spanY = abs(event->getScreenY() - m_plotAreaMouseDownYPos);
    return ((direction != Chart::DirectionVertical) && (spanX >= m_minDragAmount)) ||
        ((direction != Chart::DirectionHorizontal) && (spanY >= m_minDragAmount));
}

//
// Process mouse dragging over the plot area
//
void JChartViewer::onPlotAreaMouseDrag(MouseEvent *event)
{
  /*  switch (m_mouseUsage)
    {
        case Chart::MouseUsageZoomIn :
        {
            //
            // Mouse is used for zoom in. Draw the selection rectangle if necessary.
            //

            bool isDragZoom = canZoomIn(m_zoomDirection) && isDrag(m_zoomDirection, event);
            if (isDragZoom)
            {
                int spanX = m_plotAreaMouseDownXPos - event->x();
                int spanY = m_plotAreaMouseDownYPos - event->y();

                switch (m_zoomDirection)
                {
                case Chart::DirectionHorizontal:
                    drawRect(event->x(), getPlotAreaTop(), spanX, getPlotAreaHeight());
                    break;
                case Chart::DirectionVertical:
                    drawRect(getPlotAreaLeft(), event->y(), getPlotAreaWidth(), spanY);
                    break;
                default:
                    drawRect(event->x(), event->y(), spanX, spanY);
                    break;
                }
            }
            setRectVisible(isDragZoom);
            break;
        }
        case Chart::MouseUsageScroll :
        {
            //
            // Mouse is used for drag scrolling. Scroll and update the view port.
            //

            if (m_isDragScrolling || isDrag(m_scrollDirection, event))
            {
                m_isDragScrolling = true;
                if (dragTo(m_scrollDirection,
                    event->x() - m_plotAreaMouseDownXPos, event->y() - m_plotAreaMouseDownYPos))
                    updateViewPort(true, false);
            }
        }
    }*/
}

//
// Trigger the ViewPortChanged event
//
void JChartViewer::updateViewPort(bool needUpdateChart, bool needUpdateImageMap)
{
    // Already updating, no need to update again
    if (m_isInViewPortChanged)
        return;

    // Merge the current update requests with any pending requests.
    m_needUpdateChart = m_needUpdateChart || needUpdateChart;
    m_needUpdateImageMap = needUpdateImageMap;

    // Hold timer has not expired, so do not update chart immediately. Keep the requests pending.
    if (m_holdTimerActive)
        return;

    // The chart can be updated more than once during mouse move. For example, it can update due to
    // drag to scroll, and also due to drawing track cursor. So we delay updating the display until
    // all all updates has occured.
    int hasDelayUpdate = (m_delayUpdateChart != NO_DELAY);
    if (!hasDelayUpdate)
        m_delayUpdateChart = NEED_DELAY;

    // Can trigger the ViewPortChanged event.
    validateViewPort();
    //m_isInViewPortChanged = true;
   // emit viewPortChanged();
    //m_isInViewPortChanged = false;

    // Can update chart now
  /*  if (!hasDelayUpdate)
        commitUpdateChart();

    // Clear any pending updates.
    m_needUpdateChart = false;
    m_needUpdateImageMap = false;

    // Set hold timer to prevent multiple chart updates within a short period.
    if (m_updateInterval > 0)
    {
        m_holdTimerActive = true;
        m_holdTimerId = startTimer(m_updateInterval);
    }*/
}

/////////////////////////////////////////////////////////////////////////////
// Build in mouse cursors for zooming and scrolling support
//

static const unsigned int zoomInCursorA[] =
{
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xff3ff8ff,
0xff0fe0ff,
0xff07c0ff,
0xff07c0ff,
0xff0380ff,
0xff0380ff,
0xff0380ff,
0xff0380ff,
0xff0380ff,
0xff07c0ff,
0xff07c0ff,
0xff01e0ff,
0xff30f8ff,
0x7ff0ffff,
0x3ff8ffff,
0x1ffcffff,
0x0ffeffff,
0x0fffffff,
0x9fffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff
};

static const unsigned int zoomInCursorB[] =
{
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00c00700,
0x00f01f00,
0x00f01e00,
0x00f83e00,
0x00f83e00,
0x00183000,
0x00f83e00,
0x00f83e00,
0x00f01e00,
0x00f01f00,
0x00c00700,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000
};

static const unsigned int zoomOutCursorA[] =
{
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xff3ff8ff,
0xff0fe0ff,
0xff07c0ff,
0xff07c0ff,
0xff0380ff,
0xff0380ff,
0xff0380ff,
0xff0380ff,
0xff0380ff,
0xff07c0ff,
0xff07c0ff,
0xff01e0ff,
0xff30f8ff,
0x7ff0ffff,
0x3ff8ffff,
0x1ffcffff,
0x0ffeffff,
0x0fffffff,
0x9fffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff
};

static const unsigned int zoomOutCursorB[] =
{
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00c00700,
0x00f01f00,
0x00f01f00,
0x00f83f00,
0x00f83f00,
0x00183000,
0x00f83f00,
0x00f83f00,
0x00f01f00,
0x00f01f00,
0x00c00700,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000
};

static const unsigned int noZoomCursorA[] =
{
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xff3ff8ff,
0xff0fe0ff,
0xff07c0ff,
0xff07c0ff,
0xff0380ff,
0xff0380ff,
0xff0380ff,
0xff0380ff,
0xff0380ff,
0xff07c0ff,
0xff07c0ff,
0xff01e0ff,
0xff30f8ff,
0x7ff0ffff,
0x3ff8ffff,
0x1ffcffff,
0x0ffeffff,
0x0fffffff,
0x9fffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff
};

static const unsigned int noZoomCursorB[] =
{
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00c00700,
0x00f01f00,
0x00f01f00,
0x00f83f00,
0x00f83f00,
0x00f83f00,
0x00f83f00,
0x00f83f00,
0x00f01f00,
0x00f01f00,
0x00c00700,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000
};

static const unsigned int noMove2DCursorA[] =
{
0xffffffff,
0xffffffff,
0xffffffff,
0xff7ffeff,
0xff3ffcff,
0xff1ff8ff,
0xff0ff0ff,
0xff07e0ff,
0xff03c0ff,
0xff03c0ff,
0xfffc3fff,
0x7ffc3ffe,
0x3ffc3ffc,
0x1f7c3ef8,
0x0f3c3cf0,
0x071c38e0,
0x071c38e0,
0x0f3c3cf0,
0x1f7c3ef8,
0x3ffc3ffc,
0x7ffc3ffe,
0xfffc3fff,
0xff03c0ff,
0xff03c0ff,
0xff07e0ff,
0xff0ff0ff,
0xff1ff8ff,
0xff3ffcff,
0xff7ffeff,
0xffffffff,
0xffffffff,
0xffffffff
};

static const unsigned int noMove2DCursorB[] =
{
0x00000000,
0x00000000,
0x00000000,
0x00800100,
0x00400200,
0x00200400,
0x00100800,
0x00081000,
0x00042000,
0x00fc3f00,
0x0003c000,
0x80024001,
0x40024002,
0x20824104,
0x10424208,
0x08224410,
0x08224410,
0x10424208,
0x20824104,
0x40024002,
0x80024001,
0x0003c000,
0x00fc3f00,
0x00042000,
0x00081000,
0x00100800,
0x00200400,
0x00400200,
0x00800100,
0x00000000,
0x00000000,
0x00000000
};

static const unsigned int noMoveHorizCursorA[] =
{
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xfffc3fff,
0x7ffc3ffe,
0x3ffc3ffc,
0x1f7c3ef8,
0x0f3c3cf0,
0x071c38e0,
0x071c38e0,
0x0f3c3cf0,
0x1f7c3ef8,
0x3ffc3ffc,
0x7ffc3ffe,
0xfffc3fff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff
};

static const unsigned int noMoveHorizCursorB[] =
{
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x0003c000,
0x80024001,
0x40024002,
0x20824104,
0x10424208,
0x08224410,
0x08224410,
0x10424208,
0x20824104,
0x40024002,
0x80024001,
0x0003c000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000
};

static const unsigned int noMoveVertCursorA[] =
{
0xffffffff,
0xffffffff,
0xffffffff,
0xff7ffeff,
0xff3ffcff,
0xff1ff8ff,
0xff0ff0ff,
0xff07e0ff,
0xff03c0ff,
0xff03c0ff,
0xffffffff,
0xffffffff,
0xffffffff,
0xff7ffeff,
0xff3ffcff,
0xff1ff8ff,
0xff1ff8ff,
0xff3ffcff,
0xff7ffeff,
0xffffffff,
0xffffffff,
0xffffffff,
0xff03c0ff,
0xff03c0ff,
0xff07e0ff,
0xff0ff0ff,
0xff1ff8ff,
0xff3ffcff,
0xff7ffeff,
0xffffffff,
0xffffffff,
0xffffffff
};

static const unsigned int noMoveVertCursorB[] =
{
0x00000000,
0x00000000,
0x00000000,
0x00800100,
0x00400200,
0x00200400,
0x00100800,
0x00081000,
0x00042000,
0x00fc3f00,
0x00000000,
0x00000000,
0x00000000,
0x00800100,
0x00400200,
0x00200400,
0x00200400,
0x00400200,
0x00800100,
0x00000000,
0x00000000,
0x00000000,
0x00fc3f00,
0x00042000,
0x00081000,
0x00100800,
0x00200400,
0x00400200,
0x00800100,
0x00000000,
0x00000000,
0x00000000
};

static MouseCursor *hZoomInCursor = 0;
static MouseCursor *hZoomOutCursor = 0;
static MouseCursor *hNoZoomCursor = 0;
static MouseCursor *hNoMove2DCursor = 0;
static MouseCursor *hNoMoveHorizCursor = 0;
static MouseCursor *hNoMoveVertCursor = 0;

class FreeCursors
{
public:
    ~FreeCursors()
    {
        delete hZoomInCursor;
        delete hZoomOutCursor;
        delete hNoZoomCursor;
        delete hNoMove2DCursor;
        delete hNoMoveHorizCursor;
        delete hNoMoveVertCursor;
    }
} dummyFreeCursorObj;
/*
static MouseCursor *createCursor(const unsigned int *andPlane, const unsigned int *orPlane, int hotX, int hotY)
{
    // The cursor bitmaps are in and/or plane formats used in the Win32 CreateCursor
    // function. We need to change it to the bitmap/mask format used by QT.
    unsigned int buffer[32];
    for (int i = 0; i < 32; ++i)
        buffer[i] = ~(orPlane[i] + andPlane[i]);
    Image::BitmapData bitmap{ };// = QBitmap::fromData(QSize(32, 32), (uchar*)buffer, QImage::Format_Mono);
    for (int i = 0; i < 32; ++i)
        buffer[i] = orPlane[i] | ~andPlane[i];
    QBitmap mask = QBitmap::fromData(QSize(32, 32), (uchar *)buffer, QImage::Format_Mono);

    return new MouseCursor( hotX, hotY);
}     */
/*
static MouseCursor &getZoomInCursor()
{
    if (0 == hZoomInCursor)
        hZoomInCursor = createCursor(zoomInCursorA, zoomInCursorB, 15, 15);
    return *hZoomInCursor;
}

static MouseCursor &getZoomOutCursor()
{
    if (0 == hZoomOutCursor)
        hZoomOutCursor = createCursor(zoomOutCursorA, zoomOutCursorB, 15, 15);
    return *hZoomOutCursor;
}

static MouseCursor &getNoZoomCursor()
{
    if (0 == hNoZoomCursor)
        hNoZoomCursor = createCursor(noZoomCursorA, noZoomCursorB, 15, 15);
    return *hNoZoomCursor;
}

static MouseCursor &getNoMove2DCursor()
{
    if (0 == hNoMove2DCursor)
        hNoMove2DCursor = createCursor(noMove2DCursorA, noMove2DCursorB, 15, 15);
    return *hNoMove2DCursor;
}

static MouseCursor &getNoMoveHorizCursor()
{
    if (0 == hNoMoveHorizCursor)
        hNoMoveHorizCursor = createCursor(noMoveHorizCursorA, noMoveHorizCursorB, 15, 15);
    return *hNoMoveHorizCursor;
}

static MouseCursor &getNoMoveVertCursor()
{
    if (0 == hNoMoveVertCursor)
        hNoMoveVertCursor = createCursor(noMoveVertCursorA, noMoveVertCursorB, 15, 15);
    return *hNoMoveVertCursor;
}
  */


/////////////////////////////////////////////////////////////////////////////
//
// JViewPortControl
//
/////////////////////////////////////////////////////////////////////////////

JViewPortControl::JViewPortControl(Component *parent) 
    //Label(parent)
{
    m_ChartViewer = 0;
    m_Chart = 0;

    m_mouseDownX = 0;
    m_mouseDownY = 0;

    //setMouseTracking(true);
}

//
// Free	resources
//
JViewPortControl::~JViewPortControl()
{
}

//
// Set the CChartViewer to be associated with this control
//
void JViewPortControl::setViewer(JChartViewer *viewer)
{
    if (0 != m_ChartViewer)
        // disconnect from previous QChartViewer
//        disconnect(m_ChartViewer, 0, this, 0);
    m_ChartViewer = viewer;
  //  setViewPortManager(viewer);
    if (0 != m_ChartViewer)
        // Connect to new QChartViewer
   //     connect(m_ChartViewer, SIGNAL(viewPortChanged()), SLOT(onViewPortChanged()));

    updateDisplay();
}

//
// Get back	the	same QChartViewer pointer provided	by the previous	setViewer call.
//
JChartViewer *JViewPortControl::getViewer()
{
    return m_ChartViewer;
}

//
// Set the chart to	be displayed in the	control
//
void JViewPortControl::setChart(BaseChart *c)
{
    m_Chart = c;
    //ViewPortControlBase::setChart(c);
    updateDisplay();
}

//
// Get back	the	same BaseChart pointer provided	by the previous	setChart call.
//
BaseChart *JViewPortControl::getChart()
{
    return m_Chart;
}

//
// Convert from mouse x-coordinate to image x-coordinate
//
double JViewPortControl::toImageX(int x)
{
    // In this version, no conversion is done. It is assumed the control does not stretch or shrink
    // the image and does not provide any additional margin to offset the image.
    return x;
}

//
// Convert from mouse x-coordinate to image x-coordinate
//
double JViewPortControl::toImageY(int y)
{
    // In this version, no conversion is done. It is assumed the control does not stretch or shrink
    // the image and does not provide any additional margin to offset the image.
    return y;
}

//
// Display the chart
//
void JViewPortControl::paintDisplay()
{   
    BaseChart *c = getChart();

    // Get the image for the chart
    Image image;
 /*   if (0 != c)
    {
        // Output chart as a standard image with file headers
        MemBlock m = c->makeChart(Chart::QTIMG);
        image.loadFromData((uchar *)m.data, (uint)m.len);
        setFixedSize(c->getWidth(), c->getHeight());
    }

    // Set the QPixmap for display
    setPixmap(image);*/
}

//
// Handle the ViewPortChanged event from the associated QChartViewer
//
/*void JViewPortControl::onViewPortChanged()
{
    updateDisplay();
} */

//
// Mouse button down event.
//
void JViewPortControl::mousePressEvent(MouseEvent *event)
{
 /*   if (event->button() != Qt::LeftButton)
        return;

    // Remember current mouse position
    m_mouseDownX = event->x();
    m_mouseDownY = event->y();

    // Get the QChartViewer zoom/scroll state to determine which type of mouse action is allowed
    syncState();

    // Handle the mouse down event
    handleMouseDown(toImageX(event->x()), toImageY(event->y()));

    // Update the chart viewer if the viewport has changed
    updateChartViewerIfNecessary();*/
}

//
// MouseMove event handler
///*
void JViewPortControl::mouseMoveEvent(MouseEvent *event)
{
    // Get the QChartViewer zoom/scroll state to determine which type of mouse action is allowed
   // syncState();

    // Handle the mouse move event
   // handleMouseMove(toImageX(event->x()), toImageY(event->y()), isDrag(event));

    // Update the chart viewer if the viewport has changed
    updateChartViewerIfNecessary();

    // Update the mouse cursor
    //updateCursor(getCursor());

    // Update the display
    //if (needUpdateDisplay())
      //  paintDisplay();
}

//
// Mouse button up event.
//
void JViewPortControl::mouseReleaseEvent(MouseEvent *event)
{
  /*  if (event->button() != LeftButton)
        return;

    // Get the QChartViewer zoom/scroll state to determine which type of mouse action is allowed
    syncState();

    // Handle the mouse up event
    handleMouseUp(toImageX(event->x()), toImageY(event->y()));

    // Update the chart viewer if the viewport has changed
    updateChartViewerIfNecessary();
    */
}

//
// MouseWheel handler
/*
void JViewPortControl::wheelEvent(MouseEvent *event)
{
    // Process the mouse wheel only if the mouse is over the plot area
    if ((0 == m_ChartViewer) || (!isOnPlotArea(event->x(), event->y())))
        event->ignore();
    else
    {
        // Ask the CChartViewer to zoom around the center of the chart
        int x = m_ChartViewer->getPlotAreaLeft() + m_ChartViewer->getPlotAreaWidth() / 2;
        int y = m_ChartViewer->getPlotAreaTop() + m_ChartViewer->getPlotAreaHeight() / 2;
        if (!m_ChartViewer->onMouseWheelZoom(x, y, event->delta()))
            event->ignore();
    }
}*/

//
// Get the QChartViewer zoom/scroll state
//
/*void JViewPortControl::syncState()
{
    JChartViewer *viewer = getViewer();
    if (0 != viewer)
        setZoomScrollDirection(viewer->getZoomDirection(), viewer->getScrollDirection());
} */

//
// Determines if the mouse is dragging.
//
bool JViewPortControl::isDrag(MouseEvent *event)
{
    JChartViewer *viewer = getViewer();
    if (0 == viewer)
        return false;

    int minimumDrag = viewer->getMinimumDrag();
    int moveX = abs(m_mouseDownX - event->getScreenX());
    int moveY = abs(m_mouseDownY - event->getScreenY());
    return (moveX >= minimumDrag) || (moveY >= minimumDrag);
}

//
// Update the display
//
void JViewPortControl::updateDisplay()
{
    //paintViewPort();
    paintDisplay();
}

//
// Update the CChartViewer if the viewport has changed
//
void JViewPortControl::updateChartViewerIfNecessary()
{
    JChartViewer *viewer = getViewer();
    if (0 == viewer)
        return;

    //if (needUpdateChart() || needUpdateImageMap())
    //    viewer->updateViewPort(needUpdateChart(), needUpdateImageMap());
}

//
// Update the mouse cursor
//
void JViewPortControl::updateCursor(int position)
{
 /*   switch (position)
    {
    case Chart::Left:
    case Chart::Right:
        setMouseCursor(SizeHorCursor);
        return;
    case Chart::Top:
    case Chart::Bottom:
        setCursor(Qt::SizeVerCursor);
        return;
    case Chart::TopLeft:
    case Chart::BottomRight:
        setCursor(Qt::SizeFDiagCursor);
        return;
    case Chart::TopRight:
    case Chart::BottomLeft:
        setCursor(Qt::SizeBDiagCursor);
        return;
    default:
        unsetCursor();
        return;
    }*/
}
