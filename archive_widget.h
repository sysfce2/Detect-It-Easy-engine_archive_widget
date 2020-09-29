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
#ifndef ARCHIVE_WIDGET_H
#define ARCHIVE_WIDGET_H

#include <QWidget>
#include <QMenu>
#include "xarchives.h"
#include <QStandardItemModel>
#include "dialogentropy.h"
#include "dialoghash.h"
#include "dialogsearchstrings.h"
#include "dialoghex.h"
#include "dialogstaticscan.h"
#include "dialogcreateviewmodel.h"
#include "dialogunpackfile.h"

namespace Ui {
class Archive_widget;
}

class Archive_widget : public QWidget
{
    Q_OBJECT

    enum ACTION
    {
        ACTION_SCAN=0,
        ACTION_HEX,
        ACTION_STRINGS,
        ACTION_ENTROPY,
        ACTION_HASH,
        ACTION_DUMP
    };

public:
    explicit Archive_widget(QWidget *pParent=nullptr);
    void setData(QString sFileName);
    ~Archive_widget();

private slots:
    void on_treeViewArchive_customContextMenuRequested(const QPoint &pos);

    void scanRecord();
    void hexRecord();
    void stringsRecord();
    void entropyRecord();
    void hashRecord();
    void dumpRecord();

    void handleAction(ACTION action);
    void _handleAction(ACTION action,QIODevice *pDevice);

private:
    Ui::Archive_widget *ui;
    QString g_sFileName;
    QList<XArchive::RECORD> g_listRecords;
};

#endif // ARCHIVE_WIDGET_H
