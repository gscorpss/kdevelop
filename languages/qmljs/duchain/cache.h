/*
 * This file is part of qmljs, the QML/JS language support plugin for KDevelop
 * Copyright (c) 2013 Sven Brauch <svenbrauch@googlemail.com>
 * Copyright (c) 2014 Denis Steckelmacher <steckdenis@yahoo.fr>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef __CACHE_H__
#define __CACHE_H__

#include "duchainexport.h"
#include <serialization/indexedstring.h>
#include <util/path.h>

#include <QHash>
#include <QStringList>
#include <QFileInfo>
#include <QList>
#include <QSet>
#include <QMutex>

namespace QmlJS
{

/**
 * Cache for values that may be slow to compute (search paths, things
 * involving QStandardPaths, etc)
 */
class KDEVQMLJSDUCHAIN_EXPORT Cache
{
private:
    Cache();

public:
    static Cache& instance();

    /**
     * Path corresponding to a module name
     *
     * The path returned can either be the one of a module file shipped with
     * kdev-qmljs, or the path of a directory containing the module components
     * (one .qml file per component, and possibly .so files if the module has
     * native components)
     *
     * @param version If not null, the file being looked for is "uri_version.qml".
     *                If null, then the untouched uri is used for searching.
     */
    QString modulePath(const KDevelop::IndexedString& baseFile,
                       const QString& uri,
                       const QString& version = QStringLiteral("1.0"));

    /**
     * Return the list of the paths of the given files.
     *
     * Files having a name ending in ".so" are replaced with the path of their
     * qmlplugindump dump.
     */
    QStringList getFileNames(const QFileInfoList& fileInfos);

    /**
     * Set the custom include directories list of a file
     */
    void setFileCustomIncludes(const KDevelop::IndexedString& file,
                               const KDevelop::Path::List& dirs);

    /**
     * Add a dependency between two URLs
     */
    void addDependency(const KDevelop::IndexedString& file, const KDevelop::IndexedString& dependency);

    /**
     * List of the files that depend on a given URL
     */
    QList<KDevelop::IndexedString> filesThatDependOn(const KDevelop::IndexedString& file);

    /**
     * List of the dependencies of a file
     */
    QList<KDevelop::IndexedString> dependencies(const KDevelop::IndexedString& file);

    /**
     * Return whether a file is up to date (all its dependencies are up to date
     * and the file has been freshly parsed)
     */
    bool isUpToDate(const KDevelop::IndexedString& file);
    void setUpToDate(const KDevelop::IndexedString& file, bool upToDate);

private:
    struct PluginDumpExecutable {
        QString executable;
        QString quickVersion;       // Version of QtQuick that should be imported when this qmlplugindump is used

        PluginDumpExecutable(const QString& e, const QString &v)
        : executable(e), quickVersion(v)
        {}
    };

    QMutex m_mutex;
    QHash<QString, QString> m_modulePaths;
    QList<PluginDumpExecutable> m_pluginDumpExecutables;
    QHash<KDevelop::IndexedString, QSet<KDevelop::IndexedString>> m_dependees;
    QHash<KDevelop::IndexedString, QSet<KDevelop::IndexedString>> m_dependencies;
    QHash<KDevelop::IndexedString, bool> m_isUpToDate;
    QHash<KDevelop::IndexedString, KDevelop::Path::List> m_includeDirs;
};

}

#endif
