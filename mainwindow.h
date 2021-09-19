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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT;
    Q_DISABLE_COPY(MainWindow);
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::MainWindow *m_ui;

private slots:
    void on_checkBoxDrawIsophotes_clicked(bool checked);
    void on_checkBoxDrawCurvature_clicked(bool checked);
    void on_pushButtonNormals_clicked();
    void on_pushButtonSurface_clicked();
    void on_radioButtonRotate_clicked(bool checked);
    void on_radioButtonZoom_clicked(bool checked);
    void on_radioButtonMove_clicked(bool checked);
    void on_checkBoxDrawPoints_clicked(bool checked);
    void on_checkBoxDrawLines_clicked(bool checked);
    void on_checkBoxDrawObject_clicked(bool checked);
    void on_action_Open_triggered();
};

#endif // MAINWINDOW_H
