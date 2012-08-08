/*  To copy this you need to follow this license http://creativecommons.org/licenses/by-sa/2.5/
* Jacek Surazski doesn't endorse this work, but it's a copy of his Qt Quarterly submission.

*/
#pragma once
#include "A2GASSERT.h"
#include "FOLDERSONLY.h"
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QListWidget.h>
#include <QDir.h>
#include <QFileInfo.h>
#include <QSplitter>
#include <QTextBrowser>
#include <QGraphicsScene>
#include <QListWidgetItem>
#include <QGraphicsView>
#include "IdGenerator.h"
#include <QList>

/*! @class FileBrowser
*/ class FileBrowser : public QSplitter
{
    Q_OBJECT
    QListWidget listWidget;
    //QTextBrowser textBrowser;
    QGraphicsView view;
    QGraphicsScene scene;

public:
    FileBrowser(const QString& filter, QWidget* parent = 0)

        : QSplitter(parent)
        , listWidget(this)
        , view(this)
    {
        scene.setBackgroundBrush( QBrush(Qt::lightGray, Qt::CrossPattern));
        view.setScene(&scene);
        nameFilter = filter;
        

        setDir(QDir::currentPath());
        bool res1 = connect(&listWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(onItemChanged(QListWidgetItem*,QListWidgetItem*)));
        bool res2 = connect(&listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onSelectionChanged ()));
        A2GASSERT(res1);
        A2GASSERT(res2);
    }


    void setDir(const QString &path)
    {
        QDir dir(path);
        if (!dir.isReadable())
            return;
        listWidget.clear();


        QStringList entries = dir.entryList(QDir::Dirs);
        QStringList::ConstIterator it = entries.constBegin();
        while (it != entries.constEnd()) 
        {
            QListWidgetItem *newItem = new QListWidgetItem;
            newItem->setText(*it);
            listWidget.addItem(newItem);
            ++it;
        }
        basePath = dir.canonicalPath();
    }

signals:
    void picked(const QString &fileName);

    private slots:
        void onItemChanged (QListWidgetItem* /*current*/, QListWidgetItem* /*previous*/)
        {
            if(listWidget.currentItem()!=NULL)
            {
                QString seg = listWidget.currentItem()->text();
                QString path = basePath + "/" + seg;

                QStringList foldersOfMaxFile = QDir(path).entryList(FOLDERSONLY);
                for(QStringList::iterator maxFileSeg=foldersOfMaxFile.begin();maxFileSeg <foldersOfMaxFile.end();maxFileSeg++)
                {
                    if(*maxFileSeg =="_320x180" || *maxFileSeg=="_640x360")
                    {
                        scene.clear();
                        QString pixelFolder = basePath + "/" + seg + "/" + *maxFileSeg;

                        QStringList objectFolders = QDir(pixelFolder).entryList(FOLDERSONLY);
                        for(QStringList::iterator objectSeg=objectFolders.begin();objectSeg!=objectFolders.end();objectSeg++)
                        {
                            QString objectFolder = pixelFolder + "/" + *objectSeg;
                            QString possibleImage = objectFolder + "/initial/orig_0000.png";
                            if(QFile::exists(possibleImage))
                            {

                                //QGraphicsPixmapItem* item = scene.addPixmap(possibleImage);
                                //               scene.update();

                            }
                        }
                        break;
                    }
                }
            }
        }
        void onSelectionChanged()
        {
            QList<QListWidgetItem *> selectedItems = listWidget.selectedItems();
            if(selectedItems.size()==1)
            {
                QString seg = listWidget.selectedItems().at(0)->text();
                if (seg =="_320x180" || seg=="_640x360")
                {

                }
                else
                {
                    QString path = basePath + "/" + seg;
                    setDir(path);
                }
            }
        }

private:
    QString nameFilter;
    QString basePath;
};
