#include <WIMOParameters.h>
#include "plotdata.h"
#include "qwtquick2.h"
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qlayout.h>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_legenditem.h>
#include <qwt_legend.h>
#include <qwt_plot_canvas.h>

#include <qwt_plot_renderer.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_widget.h>

#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_date.h>
#include <qwt_date_scale_engine.h>
#include <qwt_date_scale_draw.h>
#include <WIMOClientModBus.h>

#include <QFont>

#include <QDebug>

class Zoomer: public QwtPlotZoomer
{
public:
    Zoomer( int xAxis, int yAxis, QWidget *canvas ):
        QwtPlotZoomer( xAxis, yAxis, canvas )
    {
        setTrackerMode( QwtPicker::AlwaysOff );
        setRubberBand( QwtPicker::NoRubberBand );

        // RightButton: zoom out by 1
        // Ctrl+RightButton: zoom out to full size

        setMousePattern( QwtEventPattern::MouseSelect2,
            Qt::RightButton, Qt::ControlModifier );
        setMousePattern( QwtEventPattern::MouseSelect3,
            Qt::RightButton );
    }
};

class LegendItem: public QwtPlotLegendItem
{
public:
    LegendItem()
    {
        setRenderHint( QwtPlotItem::RenderAntialiased );

        QColor color( Qt::white );

        setTextPen( color );
#if 1
        setBorderPen( color );

        QColor c( Qt::gray );
        c.setAlpha( 200 );

        setBackgroundBrush( c );
#endif
    }
};


QwtQuick2Plot::QwtQuick2Plot(QQuickItem* parent) : QQuickPaintedItem(parent)
    , m_qwtPlot(nullptr), m_timerId(0)
{
    setFlag(QQuickItem::ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);

/*
    QToolBar *toolBar = new QToolBar( this );

    QToolButton *btnExport = new QToolButton( toolBar );
    btnExport->setText( "Export" );
    toolBar->addWidget( btnExport );

    addToolBar( toolBar );
*/
    m_plotTime = 0;
    m_plotData0 = 0;
    m_plotData1 = 0;
    m_plotData2 = 0;
    uliPointNumber = 0;

    connect(this, &QQuickPaintedItem::widthChanged, this, &QwtQuick2Plot::updatePlotSize);
    connect(this, &QQuickPaintedItem::heightChanged, this, &QwtQuick2Plot::updatePlotSize);
}

QwtQuick2Plot::~QwtQuick2Plot()
{
    delete m_qwtPlot;
    m_qwtPlot = nullptr;

    if (m_timerId != 0) {
        killTimer(m_timerId);
    }
}

void QwtQuick2Plot::replotAndUpdate()
{
    m_qwtPlot->replot();
    update();
}

Q_INVOKABLE void QwtQuick2Plot::initQwtPlot()
{
 qDebug( "==initQwtPlot==" );
 QwtAxisId axisY1(QwtPlot::yLeft,0);
 QwtAxisId axisY2(QwtPlot::yLeft,1);
 QwtAxisId axisY3(QwtPlot::yLeft,2);
 // Courbes
 QColor xAxisColor  = QColor("#555555");
 QColor curve1Color = QColor("#ff3333");
 QColor curve2Color = QColor("#03a9f4");
 QColor curve3Color = QColor("#FFB300");

 //%%AP - 2020.08.27 - Réécriture : on va forcer l'init sur 3 voies de mesure
 //unsigned int uiChannelNumber = ( unsigned int )tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber;
 unsigned int uiChannelNumber = ( unsigned int )3;
 m_uliGraphicalDepth = ( TULONGINT )0;

 qDebug("uiChannelNumber = %ld", uiChannelNumber);

 // Création graphique
 m_qwtPlot = new QwtPlot();


 QPalette 	pal2 = m_qwtPlot->palette();
 pal2.setColor( QPalette::Window, Qt::white );
 m_qwtPlot->setPalette( pal2 );
 m_qwtPlot->setAutoFillBackground( true );


 // After replot() we need to call update() - so disable auto replot
 m_qwtPlot->setAutoReplot(false);
 m_qwtPlot->setStyleSheet("background: white");

 m_qwtPlot->setFont( QFont( "Sans Serif", 8, QFont::Normal/*, QFont::Bold*/ ) );

 // Désactivation axes des x
 m_qwtPlot->enableAxis(QwtPlot::xBottom, true);
 //m_qwtPlot->
 // Définition du nombre d'axes
 m_qwtPlot->setAxesCount( QwtPlot::yLeft, ( int )uiChannelNumber );
 if( uiChannelNumber > 0 ) m_qwtPlot->setAxisAutoScale(axisY1,true);
 if( uiChannelNumber > 1 ) m_qwtPlot->setAxisAutoScale(axisY2,true);
 if( uiChannelNumber > 2 ) m_qwtPlot->setAxisAutoScale(axisY3,true);

 //-----------------------------------------------------
 // Legend top
 //-----------------------------------------------------
 m_qwtPlot->insertLegend( new QwtLegend(),
 QwtPlot::LegendPosition( QwtPlot::TopLegend ) );

 //-----------------------------------------------------
 // Legend item
 //-----------------------------------------------------
 /*
 d_legendItem = new LegendItem();
 d_legendItem->attach( m_qwtPlot );

 d_legendItem->setMaxColumns( 1 );
 d_legendItem->setAlignmentInCanvas( Qt::AlignBottom | Qt::AlignLeft );
 d_legendItem->setBackgroundMode(
     QwtPlotLegendItem::BackgroundMode( QwtPlotLegendItem::LegendBackground ) );
 d_legendItem->setBorderRadius( 4 );
 d_legendItem->setMargin( 4 );
 d_legendItem->setSpacing( 4 );
 d_legendItem->setItemMargin( 3 );
 QFont font = d_legendItem->font();
 font.setPointSize( 9 );
 d_legendItem->setFont( font );
 */

 //-----------------------------------------------------
 // Grille
 //------------------------------------------------------
 QwtPlotGrid *grid = new QwtPlotGrid();
 grid->enableX(false);
 grid->setPen( QColor("#55AAAAAA"), 1, Qt::SolidLine );
 grid->attach( m_qwtPlot );

 updatePlotSize();

 //------------------------------------------------------
 // Curve 1
 //------------------------------------------------------
 m_curve1 = new QwtPlotCurve("Temperature");
 m_curve1->setPen(curve1Color, 1.5, Qt::SolidLine);
 m_curve1->setSymbol( new QwtSymbol( QwtSymbol::Ellipse,
                                     QBrush( Qt::white ), QPen( curve1Color, 2 ), QSize( 5.0, 5.0 ) ) );
 m_curve1->setStyle(QwtPlotCurve::Lines);
 m_curve1->setRenderHint(QwtPlotItem::RenderAntialiased);
 m_curve1->setData(new PlotData(&m_curve1_data));
 // Définie l'axe pour chaque courbe
 m_curve1->setYAxis(axisY1);

 //------------------------------------------------------
 // Curve 2
 //------------------------------------------------------
 //if( uiChannelNumber > 1 )
  {
   qDebug("Create m_curve2 = new();");
   m_curve2 = new QwtPlotCurve("Pressure");
   m_curve2->setPen(curve2Color, 1.5, Qt::SolidLine);
   m_curve2->setSymbol( new QwtSymbol( QwtSymbol::Ellipse ,
                                       QBrush( Qt::white ),
                                       QPen( curve2Color, 2 ),
                                       QSize( 5.0, 5.0 ) ) );
   m_curve2->setStyle(QwtPlotCurve::Lines);
   m_curve2->setRenderHint(QwtPlotItem::RenderAntialiased);
   m_curve2->setData(new PlotData(&m_curve2_data));
   // Définie l'axe pour chaque courbe
   m_curve2->setYAxis(axisY2);
  }
 //------------------------------------------------------
 // Curve 3
 //------------------------------------------------------
 //if( uiChannelNumber > 2 )
  {
   m_curve3 = new QwtPlotCurve("pH");
   m_curve3->setPen(curve3Color, 1.5, Qt::SolidLine);
   m_curve3->setSymbol( new QwtSymbol( QwtSymbol::Ellipse ,
                                       QBrush( Qt::white ), QPen( curve3Color, 2 ), QSize( 5.0, 5.0 ) ) );
   m_curve3->setStyle(QwtPlotCurve::Lines);
   m_curve3->setRenderHint(QwtPlotItem::RenderAntialiased);
   m_curve3->setData(new PlotData(&m_curve3_data));
   // Définie l'axe pour chaque courbe
   m_curve3->setYAxis(axisY3);
  }
 //------------------------------------------------------
 // Axis name
 //------------------------------------------------------
 //---------
 // X
 QwtText tXAxisText;
 tXAxisText.setRenderFlags( Qt::AlignRight | Qt::AlignVCenter );
 tXAxisText.setText("t");
 tXAxisText.setColor(xAxisColor);
 tXAxisText.setFont(QFont( "Monospace", 9, QFont::Bold, false ));
 //m_qwtPlot->setAxisTitle(m_qwtPlot->xBottom, tr("t"));
 m_qwtPlot->setAxisTitle(QwtAxisId(QwtAxis::xBottom),tXAxisText);
 QwtScaleWidget * scalewidget = m_qwtPlot->axisWidget(QwtAxisId(QwtAxis::xBottom));
 QPalette pal = scalewidget->palette();
 pal.setColor(QPalette::Text, xAxisColor);
 scalewidget->setPalette(pal);
 m_qwtPlot->setAxisFont(m_qwtPlot->xBottom,QFont( "Monospace", 7, QFont::Light, false ));
 m_qwtPlot->axisWidget(QwtAxisId(QwtAxis::xBottom))->setFont(QFont( "Monospace", 8, QFont::DemiBold, false ));
 //---------
 // Y1
 QwtText tY1AxisText;
 tY1AxisText.setRenderFlags( Qt::AlignRight | Qt::AlignVCenter );
 tY1AxisText.setText("degC");
 tY1AxisText.setColor(curve1Color);
 tY1AxisText.setFont(QFont( "Monospace", 9, QFont::Bold, false ));
 m_qwtPlot->setAxisTitle(axisY1,tY1AxisText);
 scalewidget = m_qwtPlot->axisWidget(axisY1);
 pal = scalewidget->palette();
 pal.setColor(QPalette::Text, curve1Color);
 scalewidget->setPalette(pal);
 m_qwtPlot->axisWidget(axisY1)->setFont(QFont( "Monospace", 8, QFont::DemiBold, false ));
 //---------
 // Y2
 //if( uiChannelNumber > 1 )
  {
   QwtText tY2AxisText;
   tY2AxisText.setRenderFlags( Qt::AlignRight | Qt::AlignVCenter );
   tY2AxisText.setText("dbar");
   tY2AxisText.setColor(curve2Color);
   tY2AxisText.setFont(QFont( "Monospace", 9, QFont::Bold, false ));
   m_qwtPlot->setAxisTitle(QwtAxisId(QwtAxis::yLeft,1),tY2AxisText);
   scalewidget = m_qwtPlot->axisWidget(QwtAxisId(QwtAxis::yLeft, 1));
   pal = scalewidget->palette();
   pal.setColor(QPalette::Text, curve2Color);
   scalewidget->setPalette(pal);
   m_qwtPlot->axisWidget(QwtAxisId(QwtAxis::yLeft,1))->setFont(QFont( "Monospace", 8, QFont::DemiBold, false ));
  }
 //---------
 // Y3
 //if( uiChannelNumber > 2 )
  {
   QwtText tY3AxisText;
   tY3AxisText.setRenderFlags( Qt::AlignRight | Qt::AlignVCenter );
   tY3AxisText.setText("pH");
   tY3AxisText.setColor(curve3Color);
   tY3AxisText.setFont(QFont( "Monospace", 9, QFont::Bold, false ));
   m_qwtPlot->setAxisTitle(QwtAxisId(QwtAxis::yLeft,2),tY3AxisText);
   scalewidget = m_qwtPlot->axisWidget(QwtAxisId(QwtAxis::yLeft, 2));
   pal = scalewidget->palette();
   pal.setColor(QPalette::Text, curve3Color);
   scalewidget->setPalette(pal);
   m_qwtPlot->axisWidget(QwtAxisId(QwtAxis::yLeft,2))->setFont(QFont( "Monospace", 8, QFont::DemiBold, false ));
  }
 //-------------------------------------------------------
 // Link des courbes avec le plot
 //------------------------------------------------------
 m_curve1->attach(m_qwtPlot);
 //if( uiChannelNumber > 1 )
 m_curve2->attach(m_qwtPlot);
 //if( uiChannelNumber > 2 )
 m_curve3->attach(m_qwtPlot);

 // LeftButton for the zooming
 // MidButton for the panning
 // RightButton: zoom out by 1
 // Ctrl+RighButton: zoom out to full size
 /*
 Zoomer* zoomer = new Zoomer( m_qwtPlot->canvas() );
 zoomer->setMousePattern( QwtEventPattern::MouseSelect2,
     Qt::RightButton, Qt::ControlModifier );
 zoomer->setMousePattern( QwtEventPattern::MouseSelect3,
     Qt::RightButton );
     */
/*
 // Test du zoom
 QwtPlotZoomer * zoomer = new QwtPlotZoomer( m_qwtPlot->canvas() );
 //zoomer->setKeyPattern( QwtEventPattern::KeyRedo, Qt::Key_I, Qt::ShiftModifier );
 //zoomer->setKeyPattern( QwtEventPattern::KeyUndo, Qt::Key_O, Qt::ShiftModifier );
 //zoomer->setKeyPattern( QwtEventPattern::KeyHome, Qt::Key_Home );
     // LeftButton for the zooming
    // MidButton for the panning
    // RightButton: zoom out by 1
    // Ctrl+RighButton: zoom out to full size
 zoomer->setRubberBandPen( QColor( Qt::black ) );
 zoomer->setTrackerPen( QColor( Qt::black ) );
 zoomer->setMousePattern( QwtEventPattern::MouseSelect2,
     Qt::RightButton, Qt::ControlModifier );
 zoomer->setMousePattern( QwtEventPattern::MouseSelect3,
     Qt::RightButton );

 //QCoreApplication::sendEvent(m_qwtPlot->canvas(), newEvent);
 //replotAndUpdate();
*/
 QwtPlotPanner *panner = new QwtPlotPanner( m_qwtPlot->canvas() );
 panner->setMouseButton( Qt::MidButton );

 //startTimer(2000);

 replotAndUpdate();
}

Q_INVOKABLE void QwtQuick2Plot::disableChannel(int iChannel)
{
 // Selon la voie utilisée
 switch( iChannel )
  {
   //--------
   // Voie 0
   //--------
   case( 0 ):
    m_curve1->detach();
    m_qwtPlot->setAxisVisible( QwtAxisId(QwtAxis::yLeft,0), false );
   break;
   //--------
   // Voie 1
   //--------
   case( 1 ):
    m_curve2->detach();
    m_qwtPlot->setAxisVisible( QwtAxisId(QwtAxis::yLeft,1), false );
   break;
   //--------
   // Voie 2
   //--------
   case( 2 ):
    m_curve3->detach();
    m_qwtPlot->setAxisVisible( QwtAxisId(QwtAxis::yLeft,2), false );
   break;
   // Default
   default:
   break;
  }
 replotAndUpdate();
}

Q_INVOKABLE void QwtQuick2Plot::enableChannel(int iChannel)
{
 // Selon la voie utilisée
 switch( iChannel )
  {
   //--------
   // Voie 0
   //--------
   case( 0 ):
    m_curve1->attach(m_qwtPlot); 
    m_qwtPlot->setAxisVisible( QwtAxisId(QwtAxis::yLeft,0), true );
   break;
   //--------
   // Voie 1
   //--------
   case( 1 ):
    m_curve2->attach(m_qwtPlot);
    m_qwtPlot->setAxisVisible( QwtAxisId(QwtAxis::yLeft,1), true );
   break;
   //--------
   // Voie 2
   //--------
   case( 2 ):
    m_curve3->attach(m_qwtPlot);
    m_qwtPlot->setAxisVisible( QwtAxisId(QwtAxis::yLeft,2), true );
   break;
   // Default
   default:
   break;
  }
 replotAndUpdate();
}

void QwtQuick2Plot::paint(QPainter* painter)
{
    if (m_qwtPlot) {
        QPixmap picture(boundingRect().size().toSize());

        QwtPlotRenderer renderer;
        renderer.renderTo(m_qwtPlot, picture);

        painter->drawPixmap(QPoint(), picture);
    }
}

void QwtQuick2Plot::mousePressEvent(QMouseEvent* event)
{
    qDebug() << Q_FUNC_INFO;
    routeMouseEvents(event);
}

void QwtQuick2Plot::mouseReleaseEvent(QMouseEvent* event)
{
    qDebug() << Q_FUNC_INFO;
    routeMouseEvents(event);
}

void QwtQuick2Plot::mouseMoveEvent(QMouseEvent* event)
{
    routeMouseEvents(event);
}

void QwtQuick2Plot::mouseDoubleClickEvent(QMouseEvent* event)
{
    qDebug() << Q_FUNC_INFO;
    routeMouseEvents(event);
}

void QwtQuick2Plot::wheelEvent(QWheelEvent* event)
{
    routeWheelEvents(event);
}

void QwtQuick2Plot::timerEvent(QTimerEvent* /*event*/)
{
 unsigned int uiChannelNumber = ( TUINT )tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber;

 m_plotData0 = (static_cast<double>(qrand()) / RAND_MAX) * 5;

 static unsigned long int uliCpt = 0;

 if( ++uliCpt > 31 )
 m_curve1_data.removeFirst();
 m_curve1_data.append(QPointF(m_plotTime, m_plotData0));
 // Si deux voies de mesure
 if( uiChannelNumber > 1 )
  {
   if( uliCpt > 31 )
   m_curve2_data.removeFirst();
   m_curve2_data.append(QPointF(m_plotTime, m_plotData1));
  }
 // Si trois voies de mesure
 if( uiChannelNumber > 2 )
  {
   if( uliCpt > 31 )
   m_curve3_data.removeFirst();
   m_curve3_data.append(QPointF(m_plotTime, m_plotData2));
  }
 m_plotTime += 2;
 replotAndUpdate();
}

Q_INVOKABLE void QwtQuick2Plot::updatePlotInformation1( int iChannelNumber,
                                                       QString sCh0Name,
                                                       QString sCh0Unit,
                                                       QColor tCh0Color )
{
 qDebug( "TARGET : updatePlotInformation" );
 QwtAxisId axisY1(QwtPlot::yLeft,0);
 QwtScaleWidget * scalewidget;
 QPalette pal;

 //----------------------
 // Désactivation des voies pour modification
 //----------------------
 this->disableChannel( 0 );
 this->disableChannel( 1 );
 this->disableChannel( 2 );

 // Clear des datas
 m_curve1_data.clear();
 m_curve2_data.clear();
 m_curve3_data.clear();

 // Définition du nombre d'axes
 m_qwtPlot->setAxesCount( QwtPlot::yLeft, ( int )iChannelNumber );
 if( iChannelNumber > 0 ) m_qwtPlot->setAxisAutoScale(axisY1,true);

 //------------------------------------------------------
 // Curve 1
 //------------------------------------------------------
 m_curve1->setTitle( sCh0Name );
 m_curve1->setPen(tCh0Color, 1.5, Qt::SolidLine);
 m_curve1->setSymbol( new QwtSymbol( QwtSymbol::Ellipse,
                                     QBrush( Qt::white ), QPen( tCh0Color, 2 ), QSize( 5.0, 5.0 ) ) );

 //---------
 // Y1 - Ch0
 QwtText tY1AxisText;
 tY1AxisText.setRenderFlags( Qt::AlignRight | Qt::AlignVCenter );
 tY1AxisText.setText(sCh0Unit);
 tY1AxisText.setColor(tCh0Color);
 m_qwtPlot->setAxisTitle(axisY1,tY1AxisText);
 scalewidget = m_qwtPlot->axisWidget(axisY1);
 pal = scalewidget->palette();
 pal.setColor(QPalette::Text, tCh0Color);
 scalewidget->setPalette(pal);

 //----------------------
 // Activation/Désactivation des voies utilisées/non-utilisées
 //----------------------
 this->enableChannel( 0 );
 this->disableChannel( 1 );
 this->disableChannel( 2 );

 replotAndUpdate();
}


Q_INVOKABLE void QwtQuick2Plot::updatePlotInformation2( int iChannelNumber,
                                                       QString sCh0Name,
                                                       QString sCh1Name,
                                                       QString sCh0Unit,
                                                       QString sCh1Unit,
                                                       QColor tCh0Color,
                                                       QColor tCh1Color )
{
 qDebug( "TARGET : updatePlotInformation" );
 QwtAxisId axisY1(QwtPlot::yLeft,0);
 QwtAxisId axisY2(QwtPlot::yLeft,1);
 QwtScaleWidget * scalewidget;
 QPalette pal;

 //----------------------
 // Désactivation des voies pour modification
 //----------------------
 this->disableChannel( 0 );
 this->disableChannel( 1 );
 this->disableChannel( 2 );

 // Clear des datas
 m_curve1_data.clear();
 m_curve2_data.clear();
 m_curve3_data.clear();

 // Définition du nombre d'axes
 m_qwtPlot->setAxesCount( QwtPlot::yLeft, ( int )iChannelNumber );
 if( iChannelNumber > 0 ) m_qwtPlot->setAxisAutoScale(axisY1,true);
 if( iChannelNumber > 1 ) m_qwtPlot->setAxisAutoScale(axisY2,true);

 //------------------------------------------------------
 // Curve 1
 //------------------------------------------------------
 m_curve1->setTitle( sCh0Name );
 m_curve1->setPen(tCh0Color, 1.5, Qt::SolidLine);
 m_curve1->setSymbol( new QwtSymbol( QwtSymbol::Ellipse,
                                     QBrush( Qt::white ), QPen( tCh0Color, 2 ), QSize( 5.0, 5.0 ) ) );

 //------------------------------------------------------
 // Curve 2
 //------------------------------------------------------
 if( iChannelNumber > 1 )
  {
   if( m_curve2 != nullptr )
    {
     m_curve2->setTitle( sCh1Name );
     m_curve2->setPen( tCh1Color, 1.5, Qt::SolidLine);
     m_curve2->setSymbol( new QwtSymbol( QwtSymbol::Ellipse,
                                         QBrush( Qt::white ),
                                         QPen( tCh1Color, 2 ),
                                         QSize( 5.0, 5.0 ) ) );
    }
  }

 //---------
 // Y1 - Ch0
 QwtText tY1AxisText;
 tY1AxisText.setRenderFlags( Qt::AlignRight | Qt::AlignVCenter );
 tY1AxisText.setText(sCh0Unit);
 tY1AxisText.setColor(tCh0Color);
 m_qwtPlot->setAxisTitle(axisY1,tY1AxisText);
 scalewidget = m_qwtPlot->axisWidget(axisY1);
 pal = scalewidget->palette();
 pal.setColor(QPalette::Text, tCh0Color);
 scalewidget->setPalette(pal);
 //---------
 // Y2 - Ch1
 QwtText tY2AxisText;
 tY2AxisText.setRenderFlags( Qt::AlignRight | Qt::AlignVCenter );
 tY2AxisText.setText(sCh1Unit);
 tY2AxisText.setColor(tCh1Color);
 m_qwtPlot->setAxisTitle(axisY2,tY2AxisText);
 scalewidget = m_qwtPlot->axisWidget(axisY2);
 pal = scalewidget->palette();
 pal.setColor(QPalette::Text, tCh1Color);
 scalewidget->setPalette(pal);

 this->enableChannel( 0 );
 this->enableChannel( 1 );
 this->disableChannel( 2 );
 replotAndUpdate();
}

Q_INVOKABLE void QwtQuick2Plot::updatePlotInformation3( int iChannelNumber,
                                                       QString sCh0Name,
                                                       QString sCh1Name,
                                                       QString sCh2Name,
                                                       QString sCh0Unit,
                                                       QString sCh1Unit,
                                                       QString sCh2Unit,
                                                       QColor tCh0Color,
                                                       QColor tCh1Color,
                                                       QColor tCh2Color )
{
 qDebug( "TARGET : updatePlotInformation" );
 QwtAxisId axisY1(QwtPlot::yLeft,0);
 QwtAxisId axisY2(QwtPlot::yLeft,1);
 QwtAxisId axisY3(QwtPlot::yLeft,2);
 QwtScaleWidget * scalewidget;
 QPalette pal;

 //----------------------
 // Désactivation des voies pour modification
 //----------------------
 this->disableChannel( 0 );
 this->disableChannel( 1 );
 this->disableChannel( 2 );

 // Clear des datas
 m_curve1_data.clear();
 m_curve2_data.clear();
 m_curve3_data.clear();

 // Définition du nombre d'axes
 m_qwtPlot->setAxesCount( QwtPlot::yLeft, ( int )iChannelNumber );
 if( iChannelNumber > 0 ) m_qwtPlot->setAxisAutoScale(axisY1,true);
 if( iChannelNumber > 1 ) m_qwtPlot->setAxisAutoScale(axisY2,true);
 if( iChannelNumber > 2 ) m_qwtPlot->setAxisAutoScale(axisY3,true);

 //------------------------------------------------------
 // Curve 1
 //------------------------------------------------------
 m_curve1->setTitle( sCh0Name );
 m_curve1->setPen(tCh0Color, 1.5, Qt::SolidLine);
 m_curve1->setSymbol( new QwtSymbol( QwtSymbol::Ellipse,
                                     QBrush( Qt::white ), QPen( tCh0Color, 2 ), QSize( 5.0, 5.0 ) ) );
 //------------------------------------------------------
 // Curve 2
 //------------------------------------------------------
 m_curve2->setTitle( sCh1Name );
 m_curve2->setPen( tCh1Color, 1.5, Qt::SolidLine);
 m_curve2->setSymbol( new QwtSymbol( QwtSymbol::Ellipse,
                                     QBrush( Qt::white ), QPen( tCh1Color, 2 ), QSize( 5.0, 5.0 ) ) );
 //------------------------------------------------------
 // Curve 3
 //------------------------------------------------------
 m_curve3->setTitle( sCh2Name );
 m_curve3->setPen(tCh2Color, 1.5, Qt::SolidLine);
 m_curve3->setSymbol( new QwtSymbol( QwtSymbol::Ellipse,
                                     QBrush( Qt::white ), QPen( tCh2Color, 2 ), QSize( 5.0, 5.0 ) ) );

 //---------
 // Y1 - Ch0
 QwtText tY1AxisText;
 tY1AxisText.setRenderFlags( Qt::AlignRight | Qt::AlignVCenter );
 tY1AxisText.setText(sCh0Unit);
 tY1AxisText.setColor(tCh0Color);
 m_qwtPlot->setAxisTitle(axisY1,tY1AxisText);
 scalewidget = m_qwtPlot->axisWidget(axisY1);
 pal = scalewidget->palette();
 pal.setColor(QPalette::Text, tCh0Color);
 scalewidget->setPalette(pal);
 //---------
 // Y2 - Ch1
 QwtText tY2AxisText;
 tY2AxisText.setRenderFlags( Qt::AlignRight | Qt::AlignVCenter );
 tY2AxisText.setText(sCh1Unit);
 tY2AxisText.setColor(tCh1Color);
 m_qwtPlot->setAxisTitle(axisY2,tY2AxisText);
 scalewidget = m_qwtPlot->axisWidget(axisY2);
 pal = scalewidget->palette();
 pal.setColor(QPalette::Text, tCh1Color);
 scalewidget->setPalette(pal);
 //---------
 // Y3 - Ch2
 QwtText tY3AxisText;
 tY3AxisText.setRenderFlags( Qt::AlignRight | Qt::AlignVCenter );
 tY3AxisText.setText(sCh2Unit);
 tY3AxisText.setColor(tCh2Color);
 m_qwtPlot->setAxisTitle(axisY3,tY3AxisText);
 scalewidget = m_qwtPlot->axisWidget(axisY3);
 pal = scalewidget->palette();
 pal.setColor(QPalette::Text, tCh2Color);
 scalewidget->setPalette(pal);

 //----------------------
 // Activation/Désactivation des voies utilisées/non-utilisées
 //----------------------
 this->enableChannel( 0 );
 this->enableChannel( 1 );
 this->enableChannel( 2 );

 replotAndUpdate();
}


Q_INVOKABLE void QwtQuick2Plot::displayNewSamples( int iChannelNumber,
                                        double dCh0,
                                        double dCh1,
                                        double dCh2,
                                        double dCadence )
{
 if( iChannelNumber <= 0 ) return;
 //static double t, U, U2, U3;
 m_plotData0 = dCh0;
 m_plotData1 = dCh1;
 m_plotData2 = dCh2;

 //static unsigned long int uliPointNumber = 0;
 m_plotTime += dCadence;

 if( iChannelNumber > 0 ){
   uliPointNumber++;
   if( m_uliGraphicalDepth && ( uliPointNumber > m_uliGraphicalDepth ) )
    m_curve1_data.removeFirst();
   m_curve1_data.append(QPointF(m_plotTime, m_plotData0));
  }
 // Remis
 if( iChannelNumber > 1 ){
  if( m_uliGraphicalDepth && ( uliPointNumber > m_uliGraphicalDepth ) )
   m_curve2_data.removeFirst();
  m_curve2_data.append(QPointF(m_plotTime, m_plotData1));
 }
 if( iChannelNumber > 2 ){
  if( m_uliGraphicalDepth && ( uliPointNumber > m_uliGraphicalDepth ) )
   m_curve3_data.removeFirst();
  m_curve3_data.append(QPointF(m_plotTime, m_plotData2));
 }

 replotAndUpdate();
}

void QwtQuick2Plot::clearGraphic( void )
{
    m_plotTime = 0;
    uliPointNumber = 0;
    m_curve1_data.clear();
    m_curve2_data.clear();
    m_curve3_data.clear();

    tAcquisitionStartTime = QDateTime();

    replotAndUpdate();
}

void QwtQuick2Plot::routeMouseEvents(QMouseEvent* event)
{
    if (m_qwtPlot) {
        QMouseEvent* newEvent = new QMouseEvent(event->type(), event->localPos(),
                                                event->button(), event->buttons(),
                                                event->modifiers());
        QCoreApplication::postEvent(m_qwtPlot, newEvent);
    }
}

void QwtQuick2Plot::routeWheelEvents(QWheelEvent* event)
{
    if (m_qwtPlot) {
        QWheelEvent* newEvent = new QWheelEvent(event->pos(), event->delta(),
                                                event->buttons(), event->modifiers(),
                                                event->orientation());
        QCoreApplication::postEvent(m_qwtPlot, newEvent);
    }
}

void QwtQuick2Plot::updatePlotSize()
{
    if (m_qwtPlot) {
        m_qwtPlot->setGeometry(0, 0, static_cast<int>(width()), static_cast<int>(height()));
    }
}

//------------------------------------------------------------
// Export en pdf du graphique
//------------------------------------------------------------
Q_INVOKABLE void QwtQuick2Plot::exportQwtPlot( void )
{
 QDateTime qDateTime;
 QwtPlotRenderer renderer;
 char tcBuffer[ 64 ]={0};

 // Get du datetime courant
 qDateTime = QDateTime::currentDateTime();
 // Ouverture fichier
 snprintf( tcBuffer,
           sizeof( tcBuffer ),
           "%02d%05d_data_%04d%02d%02d_%02d%02d%02d",
           tWIMOParametersSensor.tHeader.tProduct.ucSerialNumberY,
           tWIMOParametersSensor.tHeader.tProduct.uiSerialNumberN,
           qDateTime.date().year(), qDateTime.date().month(), qDateTime.date().day(),
           qDateTime.time().hour(), qDateTime.time().minute(), qDateTime.time().second() );
 renderer.exportTo( m_qwtPlot, tcBuffer );
}

//------------------------------------------------------------
// Modification de la profondeur d'affichage du graphique
//------------------------------------------------------------
Q_INVOKABLE void QwtQuick2Plot::updateGraphicalDepth( int uiGraphicalDepth )
{
 m_uliGraphicalDepth = ( unsigned long int )uiGraphicalDepth;
}

