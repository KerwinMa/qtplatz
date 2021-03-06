/**************************************************************************
** Copyright (C) 2010-2013 Toshinobu Hondo, Ph.D.
** Science Liaison / Advanced Instrumentation Project
*
** Contact: toshi.hondo@scienceliaison.com
**
** Commercial Usage
**
** Licensees holding valid ScienceLiaison commercial licenses may use this file in
** accordance with the ScienceLiaison Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and ScienceLiaison.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.TXT included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**************************************************************************/

#ifndef SPINSLIDER_HPP
#define SPINSLIDER_HPP

#include <QObject>
class QSpinBox;
class QSlider;
class QLabel;

namespace toftune {

    class SpinSlider : public QObject {
        Q_OBJECT
    public:
        explicit SpinSlider(QObject *parent = 0);

    public:
        SpinSlider( QSpinBox *, QSlider *, int minimum, int maximum, QLabel * actual = 0 );
        int value() const;

    signals:
        void valueChanged();

    private slots:
        void onSpinValueChanged( double );
        void onSliderValueChanged( int );
    private:
        QSpinBox * spin_;
        QSlider * slider_;
        QLabel * actual_;
        int minimum_;
        int maximum_;
    };

}

#endif // SPINSLIDER_HPP
