// This is a -*- C++ -*- header.
/**************************************************************************
** Copyright (C) 2010-2013 Toshinobu Hondo, Ph.D.
** Copyright (C) 2013 MS-Cheminformatics LLC
*
** Contact: info@ms-cheminfo.com
**
** Commercial Usage
**
** Licensees holding valid MS-Cheminformatics commercial licenses may use this file in
** accordance with the MS-Cheminformatics Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and MS-Cheminformatics.
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

#ifndef DATAPROCESSORFACTORY_H
#define DATAPROCESSORFACTORY_H

#include <coreplugin/editormanager/ieditorfactory.h>
#include <QStringList>

namespace Core {
    class IEditor;
}

namespace dataproc {

    class DataprocPlugin;
        
    class DataprocessorFactory : public Core::IEditorFactory {
        Q_OBJECT
    public:
        ~DataprocessorFactory();
        explicit DataprocessorFactory( QObject * owner, const QStringList& );

        // void setEditor( QWidget * );

        // implement IEditorFactory
        virtual Core::IEditor *createEditor(QWidget *parent);

        // implement IFileFactory
        virtual QStringList mimeTypes() const;
        virtual QString kind() const;
        virtual Core::IFile * open(const QString& filename );
        // <---
    signals:

    public slots:

    private:
        QString kind_;
        QStringList mimeTypes_;
    };

}

#endif // DATAPROCESSORFACTORY_H
