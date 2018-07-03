/***************************************************************************
 **                                                                        **
 **  QCustomPlot, an easy to use, modern plotting widget for Qt            **
 **  Copyright (C) 2011-2018 Emanuel Eichhammer                            **
 **                                                                        **
 **  This program is free software: you can redistribute it and/or modify  **
 **  it under the terms of the GNU General Public License as published by  **
 **  the Free Software Foundation, either version 3 of the License, or     **
 **  (at your option) any later version.                                   **
 **                                                                        **
 **  This program is distributed in the hope that it will be useful,       **
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
 **  GNU General Public License for more details.                          **
 **                                                                        **
 **  You should have received a copy of the GNU General Public License     **
 **  along with this program.  If not, see http://www.gnu.org/licenses/.   **
 **                                                                        **
 ****************************************************************************
 **           Author: Emanuel Eichhammer                                   **
 **  Website/Contact: http://www.qcustomplot.com/                          **
 **             Date: 25.06.18                                             **
 **          Version: 2.0.1                                                **
 ****************************************************************************
 ** Adapted to Strip Chart: Douglas Teeple                                 **
 **                   Date: July 2 2018                                    **
 **                Version: 1.0.0                                          **
 ****************************************************************************/

#include <iostream>   // std::cout
#include <string>     // std::string, std::stof

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mPlot(0),
    mTag1(0),
    mTag2(0),
    mTag3(0),
    mTag4(0),
    mTag5(0),
    mTag6(0),
    mTag7(0),
    mTag8(0),
    verbose(false)
{
    ui->setupUi(this);
    
    mPlot = new QCustomPlot(this);
    setCentralWidget(mPlot);
    QWidget::setWindowTitle("Strip Chart");
#if ADD_CHART_TITLE
    mPlot->plotLayout()->insertRow(0);
    BQCPTextElement *title = new QCPTextElement(mPlot, "Strip Chart", QFont("sans", 12, QFont::Bold));
    mPlot->plotLayout()->addElement(0, 0, title);
#endif
    mPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
    mPlot->plotLayout()->setColumnStretchFactor(0, 0.01);
    mPlot->legend->setVisible(true);
    mPlot->legend->setRowSpacing(0);
    mPlot->legend->setColumnSpacing(0);
    mPlot->legend->setBrush(Qt::transparent);
    mPlot->xAxis->setRange(0, 100);
    mPlot->yAxis->setRange(0, 100);
    mPlot->yAxis->setTickLabels(false);
    mPlot->xAxis->setTickLabels(false);
    mPlot->xAxis2->setTickLabels(false);
    mPlot->yAxis2->setTickLabels(true);
    mPlot->xAxis->setVisible(true);
    mPlot->xAxis2->setVisible(true);
    mPlot->yAxis2->setVisible(true);
    
    // create graphs:
    mGraph1 = mPlot->addGraph(mPlot->xAxis, mPlot->axisRect()->axis(QCPAxis::atRight, 0));
    mGraph2 = mPlot->addGraph(mPlot->xAxis, mPlot->axisRect()->axis(QCPAxis::atRight, 0));
    mGraph3 = mPlot->addGraph(mPlot->xAxis, mPlot->axisRect()->axis(QCPAxis::atRight, 0));
    mGraph4 = mPlot->addGraph(mPlot->xAxis, mPlot->axisRect()->axis(QCPAxis::atRight, 0));
    mGraph5 = mPlot->addGraph(mPlot->xAxis, mPlot->axisRect()->axis(QCPAxis::atRight, 0));
    mGraph6 = mPlot->addGraph(mPlot->xAxis, mPlot->axisRect()->axis(QCPAxis::atRight, 0));
    mGraph7 = mPlot->addGraph(mPlot->xAxis, mPlot->axisRect()->axis(QCPAxis::atRight, 0));
    mGraph8 = mPlot->addGraph(mPlot->xAxis, mPlot->axisRect()->axis(QCPAxis::atRight, 0));
    mGraph1->setPen(QPen(QColor(124,252,0)));       // lawn green
    mGraph2->setPen(QPen(QColor(255,255,0)));       // yellow
    mGraph3->setPen(QPen(QColor(184,134,11)));      // dark goldenrod
    mGraph4->setPen(QPen(QColor(0,255,255)));       // aqua
    mGraph5->setPen(QPen(QColor(65,105,225)));      // royal blue
    mGraph6->setPen(QPen(QColor(238,130,238)));     // violet
    mGraph7->setPen(QPen(QColor(139,69,19)));       // saddle brown
    mGraph8->setPen(QPen(QColor(255,0,0)));         // red
    mGraph1->setName("Avg");
    mGraph2->setName("cpu1");
    mGraph3->setName("cpu2");
    mGraph4->setName("cpu3");
    mGraph5->setName("cpu4");
    mGraph6->setName("cpu5");
    mGraph7->setName("cpu6");
    mGraph8->setName("gpu0");

    connect(&mDataTimer, SIGNAL(timeout()), this, SLOT(readInAndPlot()));
    mDataTimer.start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * read data from stdin and advnace the graph
 */
void MainWindow::readInAndPlot() {
    QTextStream qtin(stdin);
    QTextStream qtout(stdout);
    //QString line = qtin.readLine();
    QString cpuavg, cpu1, cpu2, cpu3, cpu4, cpu5, cpu6, gpu0;
    qtin >> cpuavg;
    qtin >> cpu1;
    qtin >> cpu2;
    qtin >> cpu3;
    qtin >> cpu4;
    qtin >> cpu5;
    qtin >> cpu6;
    qtin >> gpu0;
    if (verbose) {
        qtout << cpuavg << " " << cpu1 << " " << cpu2 << " " << cpu3 << " " << cpu4 << " " << cpu5 << " " << cpu6 << " " << gpu0 << endl;
    }
    if (cpuavg != "Avg") {  // skip the header line
        // calculate and add a new data point to each graph:
        mGraph1->addData(mGraph1->dataCount(), std::strtof(cpuavg.toLatin1().data(),NULL));
        mGraph2->addData(mGraph2->dataCount(), std::strtof(cpu1.toLatin1().data(),NULL));
        mGraph3->addData(mGraph3->dataCount(), std::strtof(cpu2.toLatin1().data(),NULL));
        mGraph4->addData(mGraph4->dataCount(), std::strtof(cpu3.toLatin1().data(),NULL));
        mGraph5->addData(mGraph5->dataCount(), std::strtof(cpu4.toLatin1().data(),NULL));
        mGraph6->addData(mGraph6->dataCount(), std::strtof(cpu5.toLatin1().data(),NULL));
        mGraph7->addData(mGraph7->dataCount(), std::strtof(cpu6.toLatin1().data(),NULL));
        mGraph8->addData(mGraph8->dataCount(), std::strtof(gpu0.toLatin1().data(),NULL));

        // make key axis range scroll with the data:
        mPlot->xAxis->rescale();
        mGraph1->rescaleValueAxis(true, true);
        mGraph2->rescaleValueAxis(true, true);
        mGraph3->rescaleValueAxis(true, true);
        mGraph4->rescaleValueAxis(true, true);
        mGraph5->rescaleValueAxis(true, true);
        mGraph6->rescaleValueAxis(true, true);
        mGraph7->rescaleValueAxis(true, true);
        mGraph8->rescaleValueAxis(true, true);

        mPlot->xAxis->setRange(mPlot->xAxis->range().upper, 100, Qt::AlignRight);
        mPlot->replot();
    }
}
