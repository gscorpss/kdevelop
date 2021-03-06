/*
 * This file is part of KDevelop
 *
 * Copyright 2007 David Nolden <david.nolden.kdevelop@art-master.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef CPP_QUICKOPEN_H
#define CPP_QUICKOPEN_H

#include <language/interfaces/quickopendataprovider.h>
#include <language/interfaces/quickopenfilter.h>
#include <QUrl>
#include <language/duchain/duchainpointer.h>
#include <serialization/indexedstring.h>
#include <language/util/includeitem.h>

class IncludeFileData : public KDevelop::QuickOpenDataBase {
  public:
    /**
     * includedFrom is zero if the file is not included into the current document
     * In case of importers(marked by item.pathNumber == -1), includedFrom should be the context of the current open document.
     * */
  IncludeFileData( const KDevelop::IncludeItem& item, const KDevelop::TopDUContextPointer& includedFrom );
    
    virtual QString text() const override;
    virtual QString htmlDescription() const override;

    bool execute( QString& filterText ) override;

    virtual bool isExpandable() const override;
    virtual QWidget* expandingWidget() const override;

    virtual QList<QVariant> highlighting() const override;
    
    virtual QIcon icon() const override;
  private:
    KDevelop::IncludeItem m_item;
    KDevelop::TopDUContextPointer m_includedFrom;
};

/**
 * A QuickOpenDataProvider for file-completion using include-paths.
 * It provides all files from the whole include-path, filters them by the text, and
 * also searches sub-directories if the typed text wants it.
 * */

class IncludeFileDataProvider: public KDevelop::QuickOpenDataProviderBase, public KDevelop::PathFilter<KDevelop::IncludeItem, IncludeFileDataProvider>, public KDevelop::QuickOpenFileSetInterface {
  public:
    IncludeFileDataProvider();
    virtual void setFilterText( const QString& text ) override;
    virtual void reset() override;
    virtual uint itemCount() const override;
    virtual uint unfilteredItemCount() const override;
    virtual KDevelop::QuickOpenDataPointer data( uint row ) const override;
    virtual void enableData( const QStringList& items, const QStringList& scopes ) override;

    ///Returns all scopes supported by this data-provider
    static QStringList scopes();
    
    virtual QSet<KDevelop::IndexedString> files() const override;

    //For PathFilter<..>
    //FIXME: port this properly
    KDevelop::Path itemPath( const KDevelop::IncludeItem& data ) const;

  private slots:
    void documentDestroyed( QObject* obl );

  private:

    QUrl m_baseUrl;
    QString m_lastSearchedPrefix;

    QList<KDevelop::IncludeItem> m_baseItems;
    
    bool m_allowImports, m_allowPossibleImports, m_allowImporters;

    ///Cache for all documents that import the current one
    QList<KDevelop::IndexedString> m_importers;
  
    KDevelop::TopDUContextPointer m_duContext;
};

#endif
