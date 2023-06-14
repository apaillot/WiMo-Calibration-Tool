#ifndef QMLPLOT_H
#define QMLPLOT_H

#include <QtQuick>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

class Settings;
class LegendItem;
class QwtLegend;

class QwtQuick2Plot : public QQuickPaintedItem
{
    Q_OBJECT

public:
    QwtQuick2Plot(QQuickItem* parent = nullptr);
    virtual ~QwtQuick2Plot();

    void paint(QPainter* painter);

    Q_INVOKABLE void initQwtPlot();
    // Ajout perso
    Q_INVOKABLE void exportQwtPlot();
    // Désactivation channel
    Q_INVOKABLE void disableChannel(int iChannel);
    Q_INVOKABLE void enableChannel(int iChannel);
    Q_INVOKABLE void clearGraphic( void );

    Q_INVOKABLE void displayNewSamples( int iChannelNumber,
                                        double dCh0,
                                        double dCh1,
                                        double dCh2,
                                        double dCadence );

    Q_INVOKABLE void updatePlotInformation1( int iChannelNumber,
                                            QString sCh0Name,
                                            QString sCh0Unit,
                                            QColor tCh0Color );

    Q_INVOKABLE void updatePlotInformation2( int iChannelNumber,
                                            QString sCh0Name,
                                            QString sCh1Name,
                                            QString sCh0Unit,
                                            QString sCh1Unit,
                                            QColor tCh0Color,
                                            QColor tCh1Color );
                                            /*
    Q_INVOKABLE void updatePlotInformation2_init( int iChannelNumber,
                                            QString sCh0Name,
                                            QString sCh1Name,
                                            QString sCh0Unit,
                                            QString sCh1Unit,
                                            QColor tCh0Color,
                                            QColor tCh1Color );
                                            */
    Q_INVOKABLE void updatePlotInformation3( int iChannelNumber,
                                            QString sCh0Name,
                                            QString sCh1Name,
                                            QString sCh2Name,
                                            QString sCh0Unit,
                                            QString sCh1Unit,
                                            QString sCh2UNit,
                                            QColor tCh0Color,
                                            QColor tCh1Color,
                                            QColor tCh2Color );
   // Modification de la profondeur d'affichage du graphique
   Q_INVOKABLE void updateGraphicalDepth( int uiGraphicalDepth );

protected:
    void routeMouseEvents(QMouseEvent* event);
    void routeWheelEvents(QWheelEvent* event);

    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent *event);

    virtual void timerEvent(QTimerEvent *event);

private:
    QwtPlot*         m_qwtPlot;
    QwtPlotCurve*    m_curve1;
    QVector<QPointF> m_curve1_data;
    QwtPlotCurve*    m_curve2;
    QVector<QPointF> m_curve2_data;
    QwtPlotCurve*    m_curve3;
    QVector<QPointF> m_curve3_data;
    int              m_timerId;
    // Legend item
    QwtLegend *d_externalLegend;
    LegendItem * d_legendItem;

    double m_plotTime, m_plotData0, m_plotData1, m_plotData2;
    unsigned long int uliPointNumber;
    unsigned long int m_uliGraphicalDepth;

    void replotAndUpdate();

private slots:
    void updatePlotSize();

};

#endif // QMLPLOT_H
