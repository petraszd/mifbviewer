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
#include "surfaceoptions.h"
#include "defaults.h"
#include <QLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>

SurfaceOptions::SurfaceOptions(const QList<QString> & groups, QWidget *parent) :
        QDialog(parent)
{
    m_groups = groups;
    setMinimumWidth(300);

    QVBoxLayout * layout = new QVBoxLayout();

    QSpinBox * spin;
    ColorSelector * colorSelector;
    foreach(QString group, m_groups) {
        spin = new QSpinBox();
        spin->setMinimum(5);
        spin->setMaximum(500);
        spin->setValue(DENSITY);
        layout->addWidget(new QLabel(group));
        layout->addWidget(spin);
        m_spinBoxes[group] = spin;

        colorSelector = new ColorSelector(SURFACE_COLOR, this);
        layout->addWidget(colorSelector);
        m_colorSelectors[group] = colorSelector;

        layout->addSpacing(40);
    }

    QDialogButtonBox * buttonBox = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    layout->addWidget(buttonBox);

    setLayout(layout);
}

SurfaceOptions::~SurfaceOptions()
{
    foreach(QSpinBox *box, m_spinBoxes)
        delete box;

    foreach(ColorSelector *selector, m_colorSelectors)
        delete selector;
}

void SurfaceOptions::accept()
{
    foreach(QString group, m_groups) {
        emit groupOptions(group, m_spinBoxes[group]->value(), m_colorSelectors[group]->color());
    }
    close();
}
