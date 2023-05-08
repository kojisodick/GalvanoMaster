/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "GMGraphicView.h"

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif
#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#else
#include <QtWidgets>
#endif
#include <qmath.h>

MyGraphicView::MyGraphicView(QWidget *parent)
    : QGraphicsView(parent)
{
    m_x_span = 0.0;
    m_y_span = 0.0;

    viewport()->installEventFilter(this);
    setMouseTracking(true);

    m_zoomRatio = 250;
    m_rotation = 0.0;

    updateMatrix();
}

void MyGraphicView::setRange(double x_min, double x_max, double y_min, double y_max)
{
    double x_span = x_max - x_min;
    double y_span = y_max - y_min;
    double span = fmax(x_span, y_span);

    m_x_span = x_span;
    m_y_span = y_span;

    updateMatrix();
}

void MyGraphicView::setZoomAndRotation(double zoom, double angle)
{
    m_zoomRatio = zoom;
    m_rotation = angle;

    updateMatrix();
}

void MyGraphicView::updateMatrix()
{

    // ------------------------------------------------
    // calculate ratio according to widget size
    QRect r = rect();

    double w = r.width();
    double h = r.height();

    double sx = m_x_span != 0 ? w / m_x_span : 1.0;
    double sy = m_y_span != 0 ? h / m_y_span : 1.0;

    double scale_r = fmin(sx, sy);

    scale_r *= 0.9;

    // ------------------------------------------------
    double scale_z = qPow(qreal(2), (m_zoomRatio - 250) / qreal(50));

    // ------------------------------------------------
    double scale = scale_r * scale_z;

    QMatrix matrix;
    matrix.scale(scale, scale);
    matrix.rotate(m_rotation);

    setMatrix(matrix);

}

void MyGraphicView::resizeEvent(QResizeEvent *event)
{
    updateMatrix();

    QGraphicsView::resizeEvent(event);
}

#if QT_CONFIG(wheelevent)
void MyGraphicView::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier) {
        if (e->delta() > 0) {
            emit zoomedIn();
        }
        else {
            emit zoomedOut();
        }
        e->accept();
    }
    else {
        QGraphicsView::wheelEvent(e);
    }
}
#endif









View::View(const QString &name, QWidget *parent)
    : QFrame(parent)
{

    setFrameStyle(Sunken | StyledPanel);
    m_view = new MyGraphicView(this);
    //m_view->setRenderHint(QPainter::Antialiasing, false);
    m_view->setRenderHint(QPainter::Antialiasing, true);
    m_view->setDragMode(QGraphicsView::RubberBandDrag);
    m_view->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    m_view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    m_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
#ifndef QT_NO_OPENGL
    m_view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
#else
    m_view->setViewport(new QWidget());
#endif

    connect(m_view, &MyGraphicView::zoomedIn, [=]() { zoomIn(6);  });
    connect(m_view, &MyGraphicView::zoomedOut, [=]() { zoomOut(6);  });


    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

    QToolButton *zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);
    zoomInIcon->setIcon(QPixmap(":/GMasterWinGui/Resource/zoom_in_FILL0_wght400_GRAD0_opsz48.png"));
    zoomInIcon->setIconSize(iconSize);
    QToolButton *zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap(":/GMasterWinGui/Resource/zoom_out_FILL0_wght400_GRAD0_opsz48.png"));
    zoomOutIcon->setIconSize(iconSize);
    zoomSlider = new QSlider;
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(500);
    zoomSlider->setValue(250);
    zoomSlider->setTickPosition(QSlider::TicksRight);
    // Zoom slider layout
    QVBoxLayout *zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(zoomSlider);
    zoomSliderLayout->addWidget(zoomOutIcon);


    // Data Slider
    dataSlider = new QSlider;
    dataSlider->setMinimum(0);
    dataSlider->setMaximum(0);
    dataSlider->setValue(0);
    dataSlider->setTickPosition(QSlider::TicksLeft);
    QToolButton *dataInIcon = new QToolButton;
    dataInIcon->setAutoRepeat(true);
    dataInIcon->setAutoRepeatInterval(33);
    dataInIcon->setAutoRepeatDelay(0);
    dataInIcon->setIcon(QPixmap(":/GMasterWinGui/Resource/fast_rewind_FILL0_wght400_GRAD0_opsz48.png"));
    dataInIcon->setIconSize(iconSize);
    QToolButton *dataOutIcon = new QToolButton;
    dataOutIcon->setAutoRepeat(true);
    dataOutIcon->setAutoRepeatInterval(33);
    dataOutIcon->setAutoRepeatDelay(0);
    dataOutIcon->setIcon(QPixmap(":/GMasterWinGui/Resource/fast_forward_FILL0_wght400_GRAD0_opsz48.png"));
    dataOutIcon->setIconSize(iconSize);
    // Zoom slider layout
    QVBoxLayout *dataSliderLayout = new QVBoxLayout;
    dataSliderLayout->addWidget(dataInIcon);
    dataSliderLayout->addWidget(dataSlider);
    dataSliderLayout->addWidget(dataOutIcon);


    QToolButton *rotateLeftIcon = new QToolButton;
    rotateLeftIcon->setIcon(QPixmap(":/GMasterWinGui/Resource/rotate_left_FILL0_wght400_GRAD0_opsz48.png"));
    rotateLeftIcon->setIconSize(iconSize);
    QToolButton *rotateRightIcon = new QToolButton;
    rotateRightIcon->setIcon(QPixmap(":/GMasterWinGui/Resource/rotate_right_FILL0_wght400_GRAD0_opsz48.png"));
    rotateRightIcon->setIconSize(iconSize);
    rotateSlider = new QSlider;
    rotateSlider->setOrientation(Qt::Horizontal);
    rotateSlider->setMinimum(-360);
    rotateSlider->setMaximum(360);
    rotateSlider->setValue(0);
    rotateSlider->setTickPosition(QSlider::TicksBelow);

    // Rotate slider layout
    QHBoxLayout *rotateSliderLayout = new QHBoxLayout;
    rotateSliderLayout->addWidget(rotateLeftIcon);
    rotateSliderLayout->addWidget(rotateSlider);
    rotateSliderLayout->addWidget(rotateRightIcon);

    resetButton = new QToolButton;
    //resetButton->setText(tr("0"));
    resetButton->setEnabled(false);
    resetButton->setIcon(QPixmap(":/GMasterWinGui/Resource/restart_alt_FILL0_wght400_GRAD0_opsz48.png"));


    // Label layout
    QHBoxLayout *labelLayout = new QHBoxLayout;
    //label = new QLabel(name);
    
    QStringList modeName = {"Mark","Jump","Vsc"};

    for (int i = 0; i < 3; i++) {
        modeSelect[i] = new QToolButton;
        modeSelect[i]->setCheckable(true);
        modeSelect[i]->setChecked(true);
        modeSelect[i]->setText(modeName[i]);
        connect(modeSelect[i], &QAbstractButton::toggled, this, &View::toggleMode);
    }

    
    label2 = new QLabel(tr("Pointer Mode"));
    selectModeButton = new QToolButton;
    selectModeButton->setText(tr("Select"));
    selectModeButton->setCheckable(true);
    selectModeButton->setChecked(true);
    selectModeButton->setIcon(QPixmap(":/GMasterWinGui/Resource/arrow_selector_tool_FILL0_wght400_GRAD0_opsz48.png"));
    dragModeButton = new QToolButton;
    dragModeButton->setText(tr("Drag"));
    dragModeButton->setCheckable(true);
    dragModeButton->setChecked(false);
    dragModeButton->setIcon(QPixmap(":/GMasterWinGui/Resource/drag_pan_FILL0_wght400_GRAD0_opsz48.png"));



//    antialiasButton = new QToolButton;
//    antialiasButton->setText(tr("Antialiasing"));
//    antialiasButton->setCheckable(true);
//    antialiasButton->setChecked(false);
//    openGlButton = new QToolButton;
//    openGlButton->setText(tr("OpenGL"));
//    openGlButton->setCheckable(true);
//#ifndef QT_NO_OPENGL
//    openGlButton->setEnabled(QGLFormat::hasOpenGL());
//#else
//    openGlButton->setEnabled(false);
//#endif
    //printButton = new QToolButton;
    //printButton->setIcon(QIcon(QPixmap(":/GMasterWinGui/Resource/print_FILL0_wght400_GRAD0_opsz48.png")));

    QButtonGroup *pointerModeGroup = new QButtonGroup(this);
    pointerModeGroup->setExclusive(true);
    pointerModeGroup->addButton(selectModeButton);
    pointerModeGroup->addButton(dragModeButton);

    //labelLayout->addWidget(label);
    labelLayout->addWidget(modeSelect[0]);
    labelLayout->addWidget(modeSelect[1]);
    labelLayout->addWidget(modeSelect[2]);
    labelLayout->addStretch();
    labelLayout->addWidget(label2);
    labelLayout->addWidget(selectModeButton);
    labelLayout->addWidget(dragModeButton);
    //labelLayout->addStretch();
    //labelLayout->addWidget(antialiasButton);
    //labelLayout->addWidget(openGlButton);
    //labelLayout->addWidget(printButton);

    QGridLayout *topLayout = new QGridLayout;
    topLayout->addLayout(labelLayout, 0, 1);
    
    topLayout->addLayout(dataSliderLayout, 1, 0);
    topLayout->addWidget(m_view, 1, 1);
    topLayout->addLayout(zoomSliderLayout, 1, 2);
    topLayout->addLayout(rotateSliderLayout, 2, 1);
    topLayout->addWidget(resetButton, 2, 2);
    setLayout(topLayout);

    connect(resetButton, &QAbstractButton::clicked, this, &View::resetView);
    connect(zoomSlider, &QAbstractSlider::valueChanged, this, &View::setupMatrix);
    connect(rotateSlider, &QAbstractSlider::valueChanged, this, &View::setupMatrix);
    connect(m_view->verticalScrollBar(), &QAbstractSlider::valueChanged,
        this, &View::setResetButtonEnabled);
    connect(m_view->horizontalScrollBar(), &QAbstractSlider::valueChanged,
        this, &View::setResetButtonEnabled);
    connect(selectModeButton, &QAbstractButton::toggled, this, &View::togglePointerMode);
    connect(dragModeButton, &QAbstractButton::toggled, this, &View::togglePointerMode);
    //connect(antialiasButton, &QAbstractButton::toggled, this, &View::toggleAntialiasing);
    //connect(openGlButton, &QAbstractButton::toggled, this, &View::toggleOpenGL);
    connect(rotateLeftIcon, &QAbstractButton::clicked, this, &View::rotateLeft);
    connect(rotateRightIcon, &QAbstractButton::clicked, this, &View::rotateRight);
    connect(zoomInIcon, &QAbstractButton::clicked, this, &View::zoomIn);
    connect(zoomOutIcon, &QAbstractButton::clicked, this, &View::zoomOut);
    //connect(printButton, &QAbstractButton::clicked, this, &View::print);

    connect(dataInIcon, &QAbstractButton::clicked, this, &View::dataPointPlus);
    connect(dataOutIcon, &QAbstractButton::clicked, this, &View::dataPointMinus);
    connect(dataSlider, &QAbstractSlider::valueChanged, this, &View::setDataPoint);

    setupMatrix();
}

QGraphicsView *View::view() const
{
    return static_cast<QGraphicsView *>(m_view);
}

void View::resetView()
{
    zoomSlider->setValue(250);
    rotateSlider->setValue(0);
    setupMatrix();
    m_view->ensureVisible(QRectF(0, 0, 0, 0));

    resetButton->setEnabled(false);
}

void View::setResetButtonEnabled()
{
    resetButton->setEnabled(true);
}

void View::setupMatrix()
{
    m_view->setZoomAndRotation(zoomSlider->value(), rotateSlider->value());

    setResetButtonEnabled();
}

void View::setRange(double x_min, double x_max, double y_min, double y_max)
{
    m_view->setRange(x_min, x_max, y_min, y_max);
}

void View::setDataCount(int count)
{
    dataSlider->setRange(0, count);
    dataSlider->setValue(count);
    dataSlider->setTickInterval(count/20);
}

void View::setDataPoint()
{
    int value = dataSlider->value();
    emit dataPointChanged(value);
}

void View::togglePointerMode()
{
    m_view->setDragMode(selectModeButton->isChecked()
        ? QGraphicsView::RubberBandDrag
        : QGraphicsView::ScrollHandDrag);
    m_view->setInteractive(selectModeButton->isChecked());
}

void View::toggleMode()
{
       
    emit modeSelectionChanged();
}

void View::toggleOpenGL()
{
//#ifndef QT_NO_OPENGL
//    m_view->setViewport(openGlButton->isChecked() ? new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
//#endif
}

void View::toggleAntialiasing()
{
    //m_view->setRenderHint(QPainter::Antialiasing, antialiasButton->isChecked());
}

void View::print()
{
#if QT_CONFIG(printdialog)
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        m_view->render(&painter);
    }
#endif
}


void View::dataPointPlus()
{
    dataSlider->setValue(dataSlider->value() + 10);
}

void View::dataPointMinus()
{
    dataSlider->setValue(dataSlider->value() - 10);
}


void View::zoomIn(int level)
{
    level = 1;
    zoomSlider->setValue(zoomSlider->value() + level);
}

void View::zoomOut(int level)
{
    level = 1;
    zoomSlider->setValue(zoomSlider->value() - level);
}

void View::rotateLeft()
{
    rotateSlider->setValue(rotateSlider->value() - 10);
}

void View::rotateRight()
{
    rotateSlider->setValue(rotateSlider->value() + 10);
}

