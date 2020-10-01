// copyright (c) 2020 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "createviewmodelprocess.h"

CreateViewModelProcess::CreateViewModelProcess(QObject *pParent) : QObject(pParent)
{
    bIsStop=false;
}

void CreateViewModelProcess::setData(QString sFileName, QList<XArchive::RECORD> *pListArchiveRecords, QStandardItemModel **ppModel)
{
    this->sFileName=sFileName;
    this->pListArchiveRecords=pListArchiveRecords;
    this->ppModel=ppModel;
}

void CreateViewModelProcess::stop()
{
     bIsStop=true;
}

void CreateViewModelProcess::process()
{
    QElapsedTimer scanTimer;
    scanTimer.start();

    *pListArchiveRecords=XArchives::getRecords(sFileName);

    qint64 nFileSize=XBinary::getSize(sFileName);

    int nNumberOfRecords=pListArchiveRecords->count();

    *ppModel=new QStandardItemModel;
    (*ppModel)->setColumnCount(2);

    QString sBaseName=QFileInfo(sFileName).fileName();

    QStandardItem *pRootItemName=new QStandardItem;
    pRootItemName->setText(sBaseName);
    pRootItemName->setData(sFileName,Qt::UserRole+UR_PATH);
    pRootItemName->setData(nFileSize,Qt::UserRole+UR_SIZE);
    pRootItemName->setData(true,Qt::UserRole+UR_ISROOT);

    (*ppModel)->setItem(0,0,pRootItemName);

    QStandardItem *pRootItemSize=new QStandardItem;
    pRootItemSize->setText(QString::number(nFileSize));
    pRootItemSize->setTextAlignment(Qt::AlignRight);

    (*ppModel)->setItem(0,1,pRootItemSize);

    QMap<QString,QStandardItem *> mapItems;

    for(int i=0;i<nNumberOfRecords;i++)
    {
        XArchive::RECORD record=pListArchiveRecords->at(i);

        QString sRecordFileName=record.sFileName;

        int nNumberOfParts=sRecordFileName.count("/");

        for(int j=0;j<=nNumberOfParts;j++)
        {
            QString sPart=sRecordFileName.section("/",j,j);
            QString sRelPart;

            if(j!=nNumberOfParts)
            {
                sRelPart=sRecordFileName.section("/",0,j);
            }
            else
            {
                sRelPart=sRecordFileName;
            }

            if(!mapItems.contains(sRelPart))
            {
                QStandardItem *pItemName=new QStandardItem;
                pItemName->setText(sPart);

                if(j==(nNumberOfParts))
                {
                    pItemName->setData(Qt::UserRole+1);

                    pItemName->setData(sRecordFileName,Qt::UserRole+UR_PATH);
                    pItemName->setData(record.nUncompressedSize,Qt::UserRole+UR_SIZE);
                    pItemName->setData(false,Qt::UserRole+UR_ISROOT);
                }

                QStandardItem *pParent=0;

                if(j==0)
                {
                    pParent=pRootItemName;
                }
                else
                {
                    pParent=mapItems.value(sRecordFileName.section("/",0,j-1));
                }

                QList<QStandardItem *> listItems;

                listItems.append(pItemName);

                if(j==(nNumberOfParts))
                {
                    QStandardItem *pItemSize=new QStandardItem;
                    pItemSize->setText(QString::number(record.nUncompressedSize));
                    pItemSize->setTextAlignment(Qt::AlignRight);

                    listItems.append(pItemSize);
                }

                pParent->appendRow(listItems);

                mapItems.insert(sRelPart,pItemName);
            }
        }
    }

    (*ppModel)->setHeaderData(0,Qt::Horizontal,tr("File"));
    (*ppModel)->setHeaderData(1,Qt::Horizontal,tr("Size"));

    bIsStop=false;

    emit completed(scanTimer.elapsed());
}
