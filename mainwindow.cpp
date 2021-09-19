/******************************************************************************
 *   Copyright (C) 2009 by Petras Zdanavičius                                 *
 *                                                                            *
 *   This program is free software: you can redistribute it and/or modify     *
 *   the Free Software Foundation, either version 3 of the License, or        *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   This program is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
 *   GNU Lesser General Public License for more details.                      *
 *                                                                            *
 *   You should have received a copy of the GNU Lesser General Public License *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 ******************************************************************************/
#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    showMaximized();

    if(!m_ui->canvas->isCg())
        m_ui->checkBoxDrawIsophotes->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    switch (e->type()) {
                case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
                default:
        break;
    }
}

void MainWindow::on_action_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Patch File"), "",
                                                    tr("Bezier Viewer Files (*.bv);;Text files(*.txt);;All files (*)"));
    if(fileName.isNull())
        return;

    m_ui->canvas->setFileName(fileName);
    m_ui->pushButtonSurface->setEnabled(true);
}

void MainWindow::on_checkBoxDrawObject_clicked(bool checked)
{
    m_ui->canvas->setDrawObject(checked);
}

void MainWindow::on_checkBoxDrawLines_clicked(bool checked)
{
    m_ui->canvas->setDrawLines(checked);
}

void MainWindow::on_checkBoxDrawPoints_clicked(bool checked)
{
    m_ui->canvas->setDrawPoints(checked);
}

void MainWindow::on_radioButtonMove_clicked(bool checked)
{
    if(checked)
        m_ui->canvas->setMoveState(BViewer::MOVE);
}

void MainWindow::on_radioButtonZoom_clicked(bool checked)
{
    if(checked)
        m_ui->canvas->setMoveState(BViewer::ZOOM);
}

void MainWindow::on_radioButtonRotate_clicked(bool checked)
{
    if(checked)
        m_ui->canvas->setMoveState(BViewer::ROTATE);
}

void MainWindow::on_pushButtonSurface_clicked()
{
    m_ui->canvas->openOptionts();
}

void MainWindow::on_pushButtonNormals_clicked()
{
    m_ui->canvas->renormalize();
}

void MainWindow::on_checkBoxDrawCurvature_clicked(bool checked)
{
    m_ui->canvas->setDrawCurvature(checked);
}

void MainWindow::on_checkBoxDrawIsophotes_clicked(bool checked)
{
    m_ui->canvas->setDrawIsophotes(checked);
}
