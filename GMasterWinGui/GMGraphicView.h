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

#ifndef VIEW_H
#define VIEW_H

#include <QFrame>
#include <QGraphicsView>
#include <QToolButton>

QT_BEGIN_NAMESPACE
class QLabel;
class QSlider;
class QToolButton;
QT_END_NAMESPACE


#define DEFAULT_CANVAS_WIDTH    (800)
#define DEFAULT_CANVAS_HEIGHT   (600)


class MyGraphicView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyGraphicView(QWidget *parent = nullptr);
    
    void setRange(double x_min, double x_max, double y_min, double y_max);

    void setZoomAndRotation(double zoom, double angle);

private:
    void resizeEvent(QResizeEvent *event);
    void updateMatrix();

    double m_x_span;
    double m_y_span;
    double m_zoomRatio;
    double m_rotation;

    void wheelEvent(QWheelEvent *e) override;

signals:
    void zoomedIn();
    void zoomedOut();
};


class View : public QFrame
{
    Q_OBJECT
public:
    explicit View(const QString &name, QWidget *parent = 0);

    QGraphicsView *view() const;
    void setRange(double x_min, double x_max, double y_min, double y_max);
    bool modePathEnabled(int mode) {
        return modeSelect[mode]->isChecked();
    }
    void setDataCount(int count);

public slots:
    void zoomIn(int level = 1);
    void zoomOut(int level = 1);
    void dataPointPlus();
    void dataPointMinus();


private slots:
    void setDataPoint();
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void togglePointerMode();
    void toggleOpenGL();
    void toggleAntialiasing();
    void print();
    void rotateLeft();
    void rotateRight();

    void toggleMode();
    
signals:
    void modeSelectionChanged();
    void dataPointChanged(int point);

private:
    MyGraphicView *m_view;
    QLabel *label;
    QLabel *label2;
    QToolButton *selectModeButton;
    QToolButton *dragModeButton;
    //QToolButton *openGlButton;
    //QToolButton *antialiasButton;
    //QToolButton *printButton;
    QToolButton *resetButton;

    QToolButton *modeSelect[3];

    QSlider *zoomSlider;
    QSlider *rotateSlider;
    QSlider *dataSlider;

};

#endif // VIEW_H
