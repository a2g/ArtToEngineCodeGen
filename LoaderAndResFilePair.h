/*
 * Copyright 2012 Anthony Cassidy
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

#pragma once

#include <QFile>
#include <QTextStream>
#include <QPoint>
#include <QVector>
#include <iostream>
#include <QDir>
#include <QMap>
#include "FIRST.h"
#include "Crop.h"
#include "GetObjectPlusAnim.h"
#include <QPair>
#include <algorithm>
#include "isDummyRun.h"

namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class LoaderAndResFilePair 
                {
                    mutable QString package;
                    mutable QString psdFileSeg;
                    mutable QString sceneFolder;
                    mutable QString animFolder;

                    QString theJavaClassNamePrefix;
                    QString javaClassName;

                    QString casesConcatenated;
                    QString resourcesConcatenated;


                    QVector<QString> caseStatements;
                    QVector<QPair<QString, QString> > resourcePairs;
                    QVector<QPair<QString, int> > animImages;
                    QVector<QPair<QString, int> > invImages;
                    QString firstResource;
                    static const bool IS_SWITCH = false;

                    bool isGwt;
                    QString lineThatSetsResolution;
                public:
                    LoaderAndResFilePair();
                    LoaderAndResFilePair(const QString package, const QString maxFileSeg,  const QString& sceneFolder,  QString animFolder);
                    ~LoaderAndResFilePair();

                    void setJavaClassNamePrefix(QString theJavaClassNamePrefix);

                    QString getJavaClassNamePrefix();

                    QString getJavaClassName();

                    QString makePath(QString fullOfSlashes);

                    void addAnimImage(QString pngPath, int idForObj);

                    void addInvImage(QString pngPath, int idForInv);

                    void cropImagesAndConstructDeclarations(QString find, QString replace);
                    bool isEmpty();
                private: 
                    bool isNeeded();
                    bool isInventory();
                    void addCaseStatementForAnim(QString objectSeg, QRect offset, QString realObjectSeg, QString animSeg, int idForObj, QString objectPlusAnim, QString resourceName);
                    void addCaseStatementForInv(QString invSeg, int idForInv,QString resourceName);
                    void removeAllButOne();
                    void writeGwtBundle(QTextStream& f, QString bundleJavaClassName, int start, int end, bool isDummyRun);
                    bool writeGwtLoader(QTextStream& f, QString loaderJavaClassName, const std::vector<std::pair<int,QString> >&  list);
                    void writeSwingBundle(QTextStream& f, QString bundleJavaClassName, int start, int end, bool isDummyRun);
                    bool writeSwingLoader(QTextStream& f, QString loaderJavaClassName, const std::vector<std::pair<int,QString> >&  list);
                    bool writeToFile(QString find, QString replace, bool isGwt, bool isDummyRun);
                    friend class OutputFiles;
                };
            }
        }
    }
}

